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