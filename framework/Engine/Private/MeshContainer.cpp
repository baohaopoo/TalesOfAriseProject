#include "..\Public\MeshContainer.h"

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

	// binary
	, m_vHierarchyIndexes(rhs.m_vHierarchyIndexes)
	, m_MeshContainerNodes(rhs.m_MeshContainerNodes)
{
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*>	HierarchyNodes)
{
	m_eType = eType;
	m_PivotMatrix = PivotMatrix;
	m_pAIMesh = pAIMesh;

#pragma region VERTEX_BUFFER
	HRESULT hr = 0;

	if (CModel::TYPE_NONANIM == eType || CModel::TYPE_EFFECT == eType)
	{
		hr = Create_VertexBuffer_NonAnim(pAIMesh);
	}
	else
	{
		hr = Create_VertexBuffer_Anim(pAIMesh, HierarchyNodes);
	}
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

	FACELISTINDICES32* pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
	{
		MSG_BOX(L"CMeshContainer -> NativeConstruct_Prototype -> Create_IndexBuffer");
		return E_FAIL;
	}
	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct(void* pArg)
{
	vector<CHierarchyNode*> HierarchyNodes = *(vector<CHierarchyNode*>*)pArg;

	if (0 == m_iNumBones)
	{
		if (1 == HierarchyNodes.size())
		{
			return S_OK;
		}
		auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(m_pAIMesh->mName.data, pNode->Get_Name());
		});

		if (iter == HierarchyNodes.end())
		{
			MSG_BOX(L"CMeshContainer -> NativeConstruct -> iter == HierarchyNodes.end");
			return E_FAIL;
		}
		m_pHierarchyNode = *iter;
		Safe_AddRef(*iter);

		// binary
		m_vHierarchyIndexes.push_back(distance(HierarchyNodes.begin(), iter));

		return S_OK;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];
		auto iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIBone->mName.data, pNode->Get_Name());
		});

		if (iter == HierarchyNodes.end())
		{
			MSG_BOX(L"CMeshContainer -> NativeConstruct -> iter == HierarchyNodes.end");
			return E_FAIL;
		}
		_float4x4 OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		(*iter)->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		m_MeshContainerNodes.push_back((*iter));
		Safe_AddRef(*iter);

		// binary
		m_vHierarchyIndexes.push_back(distance(HierarchyNodes.begin(), iter));
	}
	return S_OK;
}

_uint CMeshContainer::Get_NumMaterialIndex() const
{
	return m_iMaterialIndex;
}

HRESULT CMeshContainer::Create_VertexBuffer_NonAnim(aiMesh* pAIMesh)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), XMLoadFloat4x4(&m_PivotMatrix)));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), XMLoadFloat4x4(&m_PivotMatrix))));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSG_BOX(L"CMeshContainer -> Create_VertexBuffer_NonAnim -> Create_VertexBuffer");
		return E_FAIL;
	}
	Safe_Delete_Array(pVertices);
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

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	if (FAILED(Create_SkinnedInfo(pAIMesh, pVertices, HierarchyNodes)))
	{
		MSG_BOX(L"CMeshContainer -> Create_VertexBuffer_Anim -> Create_SkinnedInfo");
		return E_FAIL;
	}
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
	{
		MSG_BOX(L"CMeshContainer -> Create_VertexBuffer_Anim -> Create_VertexBuffer");
		return E_FAIL;
	}
	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CMeshContainer::Create_SkinnedInfo(aiMesh* pAIMesh, VTXANIMMODEL* pVertices, vector<CHierarchyNode*>	HierarchyNodes)
{
	m_iNumBones = pAIMesh->mNumBones;
	if (0 == m_iNumBones)
	{
		auto	iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIMesh->mName.data, pNode->Get_Name());
		});
		if (iter == HierarchyNodes.end())
		{
			MSG_BOX(L"CMeshContainer -> Create_SkinnedInfo -> iter == HierarchyNodes.end");
			return E_FAIL;
		}

		// binary
		m_vHierarchyIndexes.push_back(distance(HierarchyNodes.begin(), iter));

		m_pHierarchyNode = *iter;
		Safe_AddRef(*iter);
		return S_OK;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];
		auto iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIBone->mName.data, pNode->Get_Name());
		});

		if (iter == HierarchyNodes.end())
		{
			MSG_BOX(L"CMeshContainer -> Create_SkinnedInfo -> iter == HierarchyNodes.end");
			return E_FAIL;
		}
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		(*iter)->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		// binary
		m_vHierarchyIndexes.push_back(distance(HierarchyNodes.begin(), iter));

		m_MeshContainerNodes.push_back((*iter));
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
	_uint iIndex = 0;
	for (auto& pHierarchyNode : m_MeshContainerNodes)
	{
		XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * PivotMatrix));
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
	Safe_Release(m_pHierarchyNode);
	for (auto& pHierarchyNode : m_MeshContainerNodes)
	{
		Safe_Release(pHierarchyNode);
	}
	m_MeshContainerNodes.clear();
}

HRESULT CMeshContainer::Save_VertexBuffer_NoAnim(HANDLE & hFile, aiMesh * pAIMesh)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// VertexBuffer 정보 설정
	WriteFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumVBuffers, sizeof(_uint), &dwByte, nullptr);

	// Vertices 할당 및 읽어오기
	VTXNONANIMMODEL*		pVertices = new VTXNONANIMMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), XMLoadFloat4x4(&m_PivotMatrix)));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), XMLoadFloat4x4(&m_PivotMatrix))));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), XMLoadFloat4x4(&m_PivotMatrix))));
	}

	// Vertices 정보 저장
	WriteFile(hFile, pVertices, sizeof(VTXNONANIMMODEL) * m_iNumVertices, &dwByte, nullptr);

	// 할당한 Vertices 정보 할당 해제
	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Save_VertexBuffer_Anim(HANDLE & hFile, aiMesh * pAIMesh, vector<CHierarchyNode*> HierarchyNodes)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// VertexBuffer 정보 설정
	WriteFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumVBuffers, sizeof(_uint), &dwByte, nullptr);

	VTXANIMMODEL*		pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	m_MeshContainerNodes.clear();
	m_vHierarchyIndexes.clear();

	if (FAILED(Create_SkinnedInfo(pAIMesh, pVertices, HierarchyNodes)))
		return E_FAIL;

	// Create_SkinnedInfo함수를 1번 더 부르면서 사이즈의 값이 2배가 되어 버렸다. 그래서 반으로 만들어준다. -> 위에서 clear를 불러줘 할당을 해제했다. 
	dwStrByte = m_vHierarchyIndexes.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	_uint iSize = dwStrByte;

	for (int i = 0; i < iSize; ++i) {
		WriteFile(hFile, &m_vHierarchyIndexes[i], sizeof(int), &dwByte, nullptr);
	}

	// 버텍스 버퍼 저장
	WriteFile(hFile, pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices, &dwByte, nullptr);


	// m_MeshContainerNodes들에 있는 노드들 순서대로 정보 넣어주기

	// 뼈 정보 저장
	WriteFile(hFile, &m_iNumBones, sizeof(_uint), &dwByte, nullptr);

	// 뼈의 갯수가 0개라면
	if (0 == m_iNumBones)
	{
		// m_pHierarchyNode의 노드 번호의 인덱스 저장
		auto& iter = find_if(HierarchyNodes.begin(), HierarchyNodes.end(), [&](CHierarchyNode* pNode) {
			return pNode->Get_HierarchyNumber() == m_pHierarchyNode->Get_HierarchyNumber();
		});

		_uint iIndex = distance(HierarchyNodes.begin(), iter);
		WriteFile(hFile, &iIndex, sizeof(_uint), &dwByte, nullptr);
	}
	// 뼈의 갯수가 0개가 아니라면
	else {
		// m_MeshContainerNodes의 노드 번호들의 인덱스를 저장

		// 이번에 2번째로 Create_SkinnedInfo함수가 불려졌기 때문에 값이 2번 들어가있다. 그래서 1번으로 줄어주기 위해 반으로 줄인다.
		//  -> 위에서 clear를 불러줘 할당을 해제했다. 원상태로 복귀한다.
		_uint iSize = m_MeshContainerNodes.size();
		WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iSize; ++i) {
			_uint iNumber = m_MeshContainerNodes[i]->Get_HierarchyNumber();
			WriteFile(hFile, &iNumber, sizeof(_uint), &dwByte, nullptr);
		}

		for (int i = 0; i < iSize; ++i) {
			_float4x4 OffsetMatrix;
			XMStoreFloat4x4(&OffsetMatrix, m_MeshContainerNodes[i]->Get_OffsetMatrix());

			WriteFile(hFile, &OffsetMatrix.m, sizeof(_float4x4), &dwByte, nullptr);
		}
	}

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Save_IndexBuffer(HANDLE & hFile, aiMesh * pAIMesh)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// Indices 메모리 할당
	FACELISTINDICES32*		pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	// Indices 읽어오기
	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	// Indices 정보 저장
	WriteFile(hFile, pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive, &dwByte, nullptr);

	// 할당한 Indices 정보 할당 해제
	Safe_Delete_Array(pIndices);

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, CModel::TYPE eType, HANDLE & hFile, _float4x4 PivotMatrix, vector<class CHierarchyNode*> HierarchyNodes)
{
	CMeshContainer*	pInstance = new CMeshContainer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, hFile, PivotMatrix, HierarchyNodes)))
	{
		MSG_BOX(TEXT("Failed to Created CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel::TYPE eType, HANDLE & hFile, _float4x4 PivotMatrix, vector<CHierarchyNode*> HierarchyNodes)
{
	if (nullptr == hFile)
		return E_FAIL;

	m_eType = eType;
	m_PivotMatrix = PivotMatrix;

	if (CModel::TYPE_NONANIM == m_eType || 
		CModel::TYPE_EFFECT == m_eType) {
		Load_VertexBuffer_NonAnim(hFile);
	}
	else if (CModel::TYPE_ANIM == m_eType) {
		Load_VertexBuffer_Anim(hFile, HierarchyNodes);
	}

	Load_IndexBuffer(hFile);

	return S_OK;
}

CComponent * CMeshContainer::Clone_Dat(void * pArg)
{
	CMeshContainer*	pInstance = new CMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct_Dat(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMeshContainer::NativeConstruct_Dat(void * pArg)
{
	vector<CHierarchyNode*>		HierarchyNodes = *(vector<CHierarchyNode*>*)pArg;

	if (CModel::TYPE_NONANIM == m_eType ||
		CModel::TYPE_EFFECT == m_eType)
		return S_OK;

	if (0 == m_iNumBones)
	{
		if (1 == HierarchyNodes.size())
			return S_OK;

		m_pHierarchyNode = HierarchyNodes[m_vHierarchyIndexes[0]];

		return S_OK;
	}

	vector<CHierarchyNode*> vHierarchyNodes;
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		CHierarchyNode* pHierarchyNode = HierarchyNodes[m_vHierarchyIndexes[i]];

		pHierarchyNode->Set_OffsetMatrix(m_MeshContainerNodes[i]->Get_OffsetMatrix());
		vHierarchyNodes.push_back(pHierarchyNode);
	}

	m_MeshContainerNodes.clear();
	m_MeshContainerNodes = vHierarchyNodes;

	return S_OK;
}

HRESULT CMeshContainer::Load_VertexBuffer_NonAnim(HANDLE & hFile)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 필요한 정보 읽어오기
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumVBuffers, sizeof(_uint), &dwByte, nullptr);

	// m_VBDesc 정보 세팅
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXNONANIMMODEL*		pVertices = new VTXNONANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNONANIMMODEL) * m_iNumVertices);

	ReadFile(hFile, pVertices, sizeof(VTXNONANIMMODEL) * m_iNumVertices, &dwByte, nullptr);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Load_VertexBuffer_Anim(HANDLE & hFile, vector<CHierarchyNode*> HierarchyNodes)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 필요한 정보 읽어오기
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumVBuffers, sizeof(_uint), &dwByte, nullptr);


	// m_vHierarchyIndexes의 갯수를 가져온다.
	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	int iIndexSize = dwStrByte;
	for (int i = 0; i < iIndexSize; ++i) {
		int iIndex = 0;
		ReadFile(hFile, &iIndex, sizeof(int), &dwByte, nullptr);

		m_vHierarchyIndexes.push_back(iIndex);
	}

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXANIMMODEL*		pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	// 버티시즈 정보 읽어오기
	ReadFile(hFile, pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices, &dwByte, nullptr);

	// 뼈 갯수 읽어오기
	ReadFile(hFile, &m_iNumBones, sizeof(_uint), &dwByte, nullptr);

	// 뼈의 갯수가 0개라면
	if (0 == m_iNumBones)
	{
		// 메쉬 루트 노드 인덱스 정보 읽어오기
		int iIndex = 0;
		ReadFile(hFile, &iIndex, sizeof(_uint), &dwByte, nullptr);

		m_pHierarchyNode = HierarchyNodes[iIndex];
	}
	// 뼈의 갯수가 0개가 아니라면
	else {
		// 메쉬를 이루는 노드들 정보 읽어오기
		int iHierarchyNodeNum = 0;
		ReadFile(hFile, &iHierarchyNodeNum, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iHierarchyNodeNum; ++i) {
			int iNumber = 0;
			ReadFile(hFile, &iNumber, sizeof(_uint), &dwByte, nullptr);

			m_MeshContainerNodes.push_back(HierarchyNodes[iNumber]);
		}

		for_each(m_MeshContainerNodes.begin(), m_MeshContainerNodes.end(), [&](CHierarchyNode* pNode) {
			_float4x4 OffsetMatrix;
			ReadFile(hFile, &OffsetMatrix.m, sizeof(_float4x4), &dwByte, nullptr);

			pNode->Set_OffsetMatrix(XMLoadFloat4x4(&OffsetMatrix));
		});
	}

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Load_IndexBuffer(HANDLE & hFile)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 필요한 정보 세팅
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

	// Indices 동적할당
	FACELISTINDICES32*		pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	ReadFile(hFile, pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive, &dwByte, nullptr);

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}
