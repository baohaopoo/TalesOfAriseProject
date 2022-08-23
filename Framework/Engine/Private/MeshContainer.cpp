#include "..\Public\MeshContainer.h"

#include "HierarchyNode.h"
CMeshContainer::CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CVIBuffer(pDevice, pDeviceContext)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer& rhs)
	: CVIBuffer(rhs)
	, m_eType(rhs.m_eType)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_isBinary(rhs.m_isBinary)
	, m_MeshContainerDesc(rhs.m_MeshContainerDesc)
	, m_pNonAnimVertices(rhs.m_pNonAnimVertices)
	, m_pAnimVertices(rhs.m_pAnimVertices)
	, m_pIndices(rhs.m_pIndices)
{
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*>	HierarchyNodes)
{
	m_eType = eType;
	m_PivotMatrix = PivotMatrix;
	m_pAIMesh = pAIMesh;

	m_MeshContainerDesc = new MESHCONTAINERDESC;
	ZeroMemory(m_MeshContainerDesc, sizeof(MESHCONTAINERDESC));

#pragma region VERTEX_BUFFER
	HRESULT			hr = 0;

	if (CModel::TYPE_NONANIM == eType || CModel::TYPE_EFFECT == eType) {
		hr = Create_VertexBuffer_NonAnim(pAIMesh);
		int flag = 0;
		auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			flag++;
			return !strcmp(m_pAIMesh->mName.data, pNode->Get_Name());
		});

		if (iter != HierarchyNodes.end()) {
			strcpy_s(m_MeshContainerDesc->MeshName, sizeof(m_MeshContainerDesc->MeshName), m_pAIMesh->mName.data);
			m_MeshContainerDesc->NumMeshNode = new int;
			m_MeshContainerDesc->NumMeshNode[0] = flag - 1;
		}
	}
	else
		hr = Create_VertexBuffer_Anim(pAIMesh, HierarchyNodes);
#pragma endregion

#pragma region INDEX_BUFFER
	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;

	m_pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		m_pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		m_pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		m_pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;


	Save_Data();

#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct(void* pArg)
{
	vector<CHierarchyNode*>		HierarchyNodes = *(vector<CHierarchyNode*>*)pArg;

	if (0 == m_iNumBones)
	{
		if (1 == HierarchyNodes.size())
			return S_OK;
		if (m_isBinary == false) {
			auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
			{
				return !strcmp(m_pAIMesh->mName.data, pNode->Get_Name());
			});

			if (iter == HierarchyNodes.end())
				return E_FAIL;

			m_pHierarchyNode = *iter;

			Safe_AddRef(*iter);
		}
		else {
			m_pHierarchyNode = HierarchyNodes[m_MeshContainerDesc->NumMeshNode[0]];

			Safe_AddRef(m_pHierarchyNode);
		}

		return S_OK;
	}

	if (m_isBinary == false) {
		for (_uint i = 0; i < m_iNumBones; ++i)
		{
			aiBone*		pAIBone = m_pAIMesh->mBones[i];

			auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
			{
				return !strcmp(pAIBone->mName.data, pNode->Get_Name());
			});

			if (iter == HierarchyNodes.end())
				return E_FAIL;

			_float4x4	OffsetMatrix;
			memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

			(*iter)->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

			m_MeshContainerNodes.push_back((*iter));

			Safe_AddRef(*iter);
		}
	}
	else {
		for (_uint i = 0; i < m_iNumBones; ++i)
		{
			_float4x4	OffsetMatrix;
			memcpy(&OffsetMatrix, &m_MeshContainerDesc->BoneOffsetMatrix[i], sizeof(_float4x4));
			HierarchyNodes[m_MeshContainerDesc->NumMeshNode[i]]->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

			m_MeshContainerNodes.push_back(HierarchyNodes[m_MeshContainerDesc->NumMeshNode[i]]);

			Safe_AddRef(HierarchyNodes[m_MeshContainerDesc->NumMeshNode[i]]);
		}

	}
	return S_OK;
}

_uint CMeshContainer::Get_NumMaterialIndex() const
{
	return m_iMaterialIndex;
}

HRESULT CMeshContainer::Create_VertexBuffer_NonAnim(aiMesh* pAIMesh)
{
	//9@매쉬 만들러 왔따.pAIMesh에 있는 정보들을 맴버변수에 담아준다
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iStride = sizeof(VTXNONANIMMODEL);
	m_iNumVBuffers = 1;

	//각종 플래그 설정
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	m_pNonAnimVertices = new VTXNONANIMMODEL[m_iNumVertices];

	//버티스갯수만ㄷ큼 포지션, 노말, UV 만들어준다
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&m_pNonAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_pNonAnimVertices[i].vPosition), XMLoadFloat4x4(&m_PivotMatrix)));

		memcpy(&m_pNonAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_pNonAnimVertices[i].vNormal), XMLoadFloat4x4(&m_PivotMatrix))));

		memcpy(&m_pNonAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vTangent, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_pNonAnimVertices[i].vTangent), XMLoadFloat4x4(&m_PivotMatrix))));

		memcpy(&m_pNonAnimVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	//만든 버티스 데이터 저장해주기..
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = m_pNonAnimVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;



	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_Anim(aiMesh* pAIMesh, vector<CHierarchyNode*>	HierarchyNodes)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(m_pAnimVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&m_pAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	if (FAILED(Create_SkinnedInfo(pAIMesh, m_pAnimVertices, HierarchyNodes)))
		return E_FAIL;

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = m_pAnimVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer::Create_SkinnedInfo(aiMesh* pAIMesh, VTXANIMMODEL* pVertices, vector<CHierarchyNode*>	HierarchyNodes)
{
	m_iNumBones = pAIMesh->mNumBones;
	m_MeshContainerDesc->NumMeshNode = new int[m_iNumBones];
	ZeroMemory(m_MeshContainerDesc->NumMeshNode, sizeof(int)*m_iNumBones);

	if (0 == m_iNumBones)
	{
		int flag = 0;
		auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			flag++;
			return !strcmp(pAIMesh->mName.data, pNode->Get_Name());
		});

		if (iter == HierarchyNodes.end())
			return E_FAIL;

		m_pHierarchyNode = *iter;
		m_MeshContainerDesc->NumMeshNode[0] = flag - 1;
		Safe_AddRef(*iter);

		return S_OK;
	}

	strcpy_s(m_MeshContainerDesc->MeshName, pAIMesh->mName.data);
	m_MeshContainerDesc->NumBone = m_iNumBones;
	m_MeshContainerDesc->BoneOffsetMatrix = new _float4x4[m_iNumBones];
	m_MeshContainerDesc->NumWeights = new _int[m_iNumBones];

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];
		int flag = 0;
		auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			flag++;
			return !strcmp(pAIBone->mName.data, pNode->Get_Name());
		});

		if (iter == HierarchyNodes.end())
			return E_FAIL;
		m_MeshContainerDesc->NumMeshNode[i] = flag - 1;  //계층노드에서 이걸 통해 바로 인덱스접근 할 예정 이름필요없음

		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		memcpy(&m_MeshContainerDesc->BoneOffsetMatrix[i], &OffsetMatrix, sizeof(_float4x4));
		(*iter)->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_MeshContainerNodes.push_back((*iter));

		m_MeshContainerDesc->NumWeights[i] = pAIBone->mNumWeights;

		Safe_AddRef(*iter);

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}



	return S_OK;
}

HRESULT CMeshContainer::Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{

		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixTranspose(m_pHierarchyNode->Get_CombinedMatrix() * PivotMatrix));
	}

	_uint		iIndex = 0;
	for (auto& pHierarchyNode : m_MeshContainerNodes)
	{
		XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * PivotMatrix));
	}

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct_Prototype(MESHCONTAINERDESC * pData, vector<CHierarchyNode*> HierarchyNodes)
{
	m_eType = (CModel::TYPE)pData->eType;
	m_PivotMatrix = pData->Pivot;
	m_MeshContainerDesc = pData;
	m_isBinary = true;

#pragma region VERTEX_BUFFER
	HRESULT			hr = 0;

	if (CModel::TYPE_NONANIM == m_eType || CModel::TYPE_EFFECT == m_eType)
		hr = Data_VtxBuff_NonAnim(pData);
	else
		hr = Data_VtxBuff_Anim(pData, HierarchyNodes);
#pragma endregion

#pragma region INDEX_BUFFER
	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;

	m_pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		m_pIndices[i]._0 = pData->Indices[i * 3];
		m_pIndices[i]._1 = pData->Indices[(i * 3) + 1];
		m_pIndices[i]._2 = pData->Indices[(i * 3) + 2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::Data_VtxBuff_NonAnim(MESHCONTAINERDESC * pData)
{
	//9@매쉬 만들러 왔따.pAIMesh에 있는 정보들을 맴버변수에 담아준다
	m_iMaterialIndex = pData->MaterialIndex;
	m_iNumVertices = pData->NumVertices;
	m_iNumPrimitive = pData->NumPrimitive;
	m_iStride = sizeof(VTXNONANIMMODEL);
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	m_pNonAnimVertices = new VTXNONANIMMODEL[m_iNumVertices];

	//버티스갯수만ㄷ큼 포지션, 노말, UV 만들어준다
	for (int i = 0; i < (_int)m_iNumVertices; i++) {
		m_pNonAnimVertices[i].vPosition = pData->VtxPosition[i];
		m_pNonAnimVertices[i].vNormal = pData->VtxNormal[i];
		m_pNonAnimVertices[i].vTexUV = pData->VtxTexUV[i];
		m_pNonAnimVertices[i].vTangent = pData->Tangent[i];
	}

	_float2 temp = m_pNonAnimVertices[2].vTexUV;

	//만든 버티스 데이터 저장해주기..
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = m_pNonAnimVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;



	return S_OK;
}

HRESULT CMeshContainer::Data_VtxBuff_Anim(MESHCONTAINERDESC * pData, vector<CHierarchyNode*> HierarchyNodes)
{
	m_iMaterialIndex = pData->MaterialIndex;
	m_iNumVertices = pData->NumVertices;
	m_iNumPrimitive = pData->NumPrimitive;
	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(m_pAnimVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i++) {
		m_pAnimVertices[i].vPosition = pData->VtxPosition[i];
		m_pAnimVertices[i].vNormal = pData->VtxNormal[i];
		m_pAnimVertices[i].vTexUV = pData->VtxTexUV[i];
		m_pAnimVertices[i].vBlendIndex = pData->BlendIndex[i];
		m_pAnimVertices[i].vBlendWeight = pData->BlendWeight[i];
		m_pAnimVertices[i].vTangent = pData->Tangent[i];
	}

	if (FAILED(Data_SkinnedInfo(pData, m_pAnimVertices, HierarchyNodes)))
		return E_FAIL;


	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = m_pAnimVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer::Data_SkinnedInfo(MESHCONTAINERDESC * pData, VTXANIMMODEL * pVertices, vector<CHierarchyNode*> HierarchyNodes)
{
	m_iNumBones = pData->NumBone;


	if (0 == m_iNumBones)
	{
		m_pHierarchyNode = HierarchyNodes[pData->NumMeshNode[0]];

		Safe_AddRef(HierarchyNodes[pData->NumMeshNode[0]]);

		return S_OK;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pData->BoneOffsetMatrix[i], sizeof(_float4x4));
		HierarchyNodes[pData->NumMeshNode[i]]->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		m_MeshContainerNodes.push_back(HierarchyNodes[pData->NumMeshNode[i]]);

		Safe_AddRef(HierarchyNodes[pData->NumMeshNode[i]]);

	}

	return S_OK;
}

HRESULT CMeshContainer::Save_Data()
{
	m_MeshContainerDesc->eType = m_eType;
	m_MeshContainerDesc->Pivot = m_PivotMatrix;
	m_MeshContainerDesc->Indices = new int[m_iNumPrimitive * 3];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		m_MeshContainerDesc->Indices[i * 3] = m_pIndices[i]._0;
		m_MeshContainerDesc->Indices[i * 3 + 1] = m_pIndices[i]._1;
		m_MeshContainerDesc->Indices[i * 3 + 2] = m_pIndices[i]._2;
	}

	m_MeshContainerDesc->MaterialIndex = m_iMaterialIndex;
	m_MeshContainerDesc->NumVertices = m_iNumVertices;
	m_MeshContainerDesc->NumPrimitive = m_iNumPrimitive;
	m_MeshContainerDesc->VtxPosition = new _float3[m_iNumVertices];
	m_MeshContainerDesc->VtxNormal = new _float3[m_iNumVertices];
	m_MeshContainerDesc->VtxTexUV = new _float2[m_iNumVertices];
	m_MeshContainerDesc->Tangent = new _float3[m_iNumVertices];

	if (m_eType == CModel::TYPE_ANIM) {  //Create_SkinnedInfo
		m_MeshContainerDesc->BlendIndex = new XMUINT4[m_iNumVertices];
		m_MeshContainerDesc->BlendWeight = new XMFLOAT4[m_iNumVertices];
		for (int i = 0; i < (_int)m_iNumVertices; i++) {
			m_MeshContainerDesc->VtxPosition[i] = m_pAnimVertices[i].vPosition;
			m_MeshContainerDesc->VtxNormal[i] = m_pAnimVertices[i].vNormal;
			m_MeshContainerDesc->VtxTexUV[i] = m_pAnimVertices[i].vTexUV;
			m_MeshContainerDesc->BlendIndex[i] = m_pAnimVertices[i].vBlendIndex;
			m_MeshContainerDesc->BlendWeight[i] = m_pAnimVertices[i].vBlendWeight;
		}
	}
	else {
		for (int i = 0; i < (_int)m_iNumVertices; i++) {
			m_MeshContainerDesc->VtxPosition[i] = m_pNonAnimVertices[i].vPosition;
			m_MeshContainerDesc->VtxNormal[i] = m_pNonAnimVertices[i].vNormal;
			m_MeshContainerDesc->VtxTexUV[i] = m_pNonAnimVertices[i].vTexUV;
			m_MeshContainerDesc->Tangent[i] = m_pNonAnimVertices[i].vTangent;
		}
	}

	return S_OK;
}

CMeshContainer* CMeshContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*> HierarchyNodes)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pAIMesh, PivotMatrix, HierarchyNodes)))
	{
		MSG_BOX(L"CMeshContainer -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, MESHCONTAINERDESC * pData, vector<class CHierarchyNode*> HierarchyNodes)
{
	CMeshContainer*	pInstance = new CMeshContainer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pData, HierarchyNodes)))
	{
		MSG_BOX(TEXT("Failed to Created CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMeshContainer::Clone(void* pArg)
{
	CMeshContainer* pInstance = new CMeshContainer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"CMeshContainer -> Clone -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshContainer::Free()
{
	__super::Free();
	if (m_isCloned == false) {
		Safe_Delete_Array(m_pIndices);

		if (m_MeshContainerDesc != nullptr && m_isBinary == false) {
			Safe_Delete_Array(m_MeshContainerDesc->Indices);
			Safe_Delete_Array(m_MeshContainerDesc->VtxPosition);
			Safe_Delete_Array(m_MeshContainerDesc->VtxNormal);
			Safe_Delete_Array(m_MeshContainerDesc->VtxTexUV);
			Safe_Delete_Array(m_MeshContainerDesc->Tangent);

			if (m_eType == CModel::TYPE_ANIM) {
				if (m_iNumBones > 0) {
					Safe_Delete_Array(m_MeshContainerDesc->BoneOffsetMatrix);
					Safe_Delete_Array(m_MeshContainerDesc->NumWeights);
					Safe_Delete_Array(m_MeshContainerDesc->BlendIndex);
					Safe_Delete_Array(m_MeshContainerDesc->BlendWeight);
				}
				Safe_Delete_Array(m_MeshContainerDesc->NumMeshNode);
			}
			Safe_Delete(m_MeshContainerDesc);
		}
		Safe_Delete_Array(m_pNonAnimVertices);
		Safe_Delete_Array(m_pAnimVertices);
	}

	if (m_eType == CModel::TYPE_NONANIM || m_eType == CModel::TYPE_EFFECT)
		Safe_Release(m_pHierarchyNode);

	for (auto& pHierarchyNode : m_MeshContainerNodes)
		Safe_Release(pHierarchyNode);

	m_MeshContainerNodes.clear();
}