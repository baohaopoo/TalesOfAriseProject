#include "..\Public\SubModel.h"
#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Texture.h"
#include "Shader.h"
#include "Animation.h"

CSubModel::CSubModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CSubModel::CSubModel(const CSubModel& rhs)
	: CComponent(rhs)
	, m_pScene(rhs.m_pScene)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Animations(rhs.m_Animations)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eType(rhs.m_eType)
{
	for (auto& pMeshContainer : m_MeshContainers)
	{
		Safe_AddRef(pMeshContainer);
	}
	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(pMaterial.pMaterials[i]);
		}
	}
	for (auto& pAnimation : m_Animations)
	{
		Safe_AddRef(pAnimation);
	}
}

HRESULT CSubModel::NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);	

	m_eType = eType;

	_uint iFlag;
	if (TYPE_NONANIM == m_eType)
	{
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	}
	else if (TYPE_EFFECT == m_eType)
	{
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	}
	else
	{
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	}
	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> NativeConstruct_Prototype -> nullptr == m_pScene");
		return E_FAIL;
	}
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct_Prototype -> Ready_HierarchyNodes");
		return E_FAIL;
	}
	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});
	if (FAILED(Ready_MeshContainers()))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct_Prototype -> Ready_MeshContainers");
		return E_FAIL;
	}
	if (FAILED(Ready_Materials(pModelFilePath)))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct_Prototype -> Ready_Materials");
		return E_FAIL;
	}
	if (FAILED(Ready_Animations()))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct_Prototype -> Ready_Animations");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSubModel::NativeConstruct(void* pArg)
{
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct -> Ready_HierarchyNodes");
		return E_FAIL;
	}
	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Clone_MeshContainers()))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct -> Clone_MeshContainers");
		return E_FAIL;
	}
	if (FAILED(Clone_Animations()))
	{
		MSG_BOX(L"CSubModel -> NativeConstruct -> Clone_Animations");
		return E_FAIL;
	}
	return S_OK;
}

void CSubModel::Update(_double TimeDelta, _bool bCut)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
	{
		return;
	}
	m_Animations[m_iCurrentAnimIndex]->Update(TimeDelta, bCut);
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}
}

void CSubModel::Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex)
{
	if (iNextAnimIndex >= m_iNumAnimations)
	{
		return;
	}
	vector<CChannel*> vChannel = m_Animations[iNextAnimIndex]->Get_Channels();
	m_Animations[m_iCurrentAnimIndex]->Update_NextAnim(TimeDelta, Duration, iNextAnimIndex, pCurrentAnimIndex, &m_iCurrentAnimIndex, vChannel);
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}
}

HRESULT CSubModel::Render(CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
	{
		MSG_BOX(L"CSubModel -> Render -> iMeshContainerIndex >= m_iNumMeshContainers");
		return E_FAIL;
	}
	if (TYPE_ANIM == m_eType)
	{	
		_float4x4 BoneMatrices[256];
		ZeroMemory(BoneMatrices, sizeof(_float4x4) * 256);
		m_MeshContainers[iMeshContainerIndex]->Get_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));
		pShader->Set_RawValue(pBoneMatricesName, BoneMatrices, sizeof(_float4x4) * 256);
	}
	pShader->Begin(iPassIndex);
	if (nullptr != m_MeshContainers[iMeshContainerIndex])
	{
		m_MeshContainers[iMeshContainerIndex]->Render();
	}
	return S_OK;
}

_uint CSubModel::Get_NumMeshContainer() const
{
	return m_iNumMeshContainers;
}

CAnimation* CSubModel::Get_Animation(_uint iIndex)
{
	return m_Animations[iIndex];
}

void CSubModel::Set_AnimationIndex(_uint iAnimIndex)
{
	m_iCurrentAnimIndex = iAnimIndex;
}

HRESULT CSubModel::Bind_Material_OnShader(CShader * pShader, aiTextureType eType, const char * pConstantName, _uint iMeshContainerIndex)
{
	_uint iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_NumMaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_BOX(L"CSubModel -> Bind_Material_OnShader -> iMaterialIndex >= m_iNumMaterials");
		return E_FAIL;
	}
	return m_Materials[iMaterialIndex].pMaterials[eType]->SetUp_ShaderResourceView(pShader, pConstantName, 0);	
}

HRESULT CSubModel::Ready_MeshContainers()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Ready_MeshContainers -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMeshContainers = m_pScene->mNumMeshes;
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, (CModel::TYPE)m_eType, m_pScene->mMeshes[i], m_PivotMatrix, m_HierarchyNodes);
	
		if (nullptr == pMeshContainer)
		{
			MSG_BOX(L"CSubModel -> Ready_MeshContainers -> nullptr == pMeshContainer");
			return E_FAIL;
		}
		m_MeshContainers.push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CSubModel::Clone_MeshContainers()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Clone_MeshContainers -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMeshContainers = m_pScene->mNumMeshes;
	vector<CMeshContainer*> MeshContainer;
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = (CMeshContainer*)m_MeshContainers[i]->Clone(&m_HierarchyNodes);
		if (nullptr == pMeshContainer)
		{
			MSG_BOX(L"CSubModel -> Clone_MeshContainers -> nullptr == pMeshContainer");
			return E_FAIL;
		}
		Safe_Release(m_MeshContainers[i]);
		MeshContainer.push_back(pMeshContainer);
	}
	m_MeshContainers.clear();
	m_MeshContainers = MeshContainer;
	return S_OK;	
}

HRESULT CSubModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Ready_Materials -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMaterials = m_pScene->mNumMaterials;
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pScene->mMaterials[i];
		if (nullptr == pAIMaterial)
		{
			MSG_BOX(L"CSubModel -> Ready_Materials -> nullptr == pAIMaterial");
			return E_FAIL;
		}
		MODELMATERIAL Materials;
		ZeroMemory(&Materials, sizeof(MODELMATERIAL));		
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char szTextureFilePath[MAX_PATH] = "";
			aiString	strPath;
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
			{
				continue;
			}
			char szFileName[MAX_PATH] = "";
			char szEXT[MAX_PATH] = "";
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			strcpy_s(szTextureFilePath, pModelFilePath);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szEXT);

			_tchar	 szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, _int(strlen(szTextureFilePath)), szFullPath, MAX_PATH);

			Materials.pMaterials[j] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);
			if (nullptr == Materials.pMaterials[j])
			{
				MSG_BOX(L"CSubModel -> Ready_Materials -> nullptr == Materials.pMaterials");
				return E_FAIL;
			}
		}
		m_Materials.push_back(Materials);
	}
	return S_OK;
}

HRESULT CSubModel::Ready_Animations()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Ready_Animations -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumAnimations = m_pScene->mNumAnimations;
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation* pAIAnimation = m_pScene->mAnimations[i];		
		CAnimation* pAnimation = CAnimation::Create(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
		{
			MSG_BOX(L"CSubModel -> Ready_Animations -> nullptr == pAnimation");
			return E_FAIL;
		}
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CSubModel::Clone_Animations()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Clone_Animations -> nullptr == m_pScene");
		return E_FAIL;
	}
	vector<CAnimation*> Animations;
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];
		CAnimation* pAnimation = m_Animations[i]->Clone(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
		{
			MSG_BOX(L"CSubModel -> Clone_Animations -> nullptr == pAnimation");
			return E_FAIL;
		}
		Safe_Release(m_Animations[i]);
		Animations.push_back(pAnimation);
	}
	m_Animations = Animations;
	return S_OK;
}

HRESULT CSubModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CSubModel -> Ready_HierarchyNodes -> nullptr == m_pScene");
		return E_FAIL;
	}
	_uint iNumChildren = pNode->mNumChildren;
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth);
	if (nullptr == pHierarchyNode)
	{
		MSG_BOX(L"CSubModel -> Ready_HierarchyNodes -> nullptr == pHierarchyNode");
		return E_FAIL;
	}
	for (_uint i = 0; i < iNumChildren; ++i)
	{
		if (FAILED(Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1)))
		{
			MSG_BOX(L"CSubModel -> Ready_HierarchyNodes -> Ready_HierarchyNodes");
			return E_FAIL;
		}
	}
	m_HierarchyNodes.push_back(pHierarchyNode);
	return S_OK;
}

CSubModel* CSubModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	CSubModel*	pInstance = new CSubModel(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(L"CSubModel -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CSubModel::Clone(void* pArg)
{
	if (m_eType == TYPE_EFFECT)
	{
		AddRef();
		return this;
	}
	else
	{
		CSubModel*	pInstance = new CSubModel(*this);
		if (FAILED(pInstance->NativeConstruct(pArg)))
		{
			MSG_BOX(L"CSubModel -> Clone -> pInstance->NativeConstruct");
			Safe_Release(pInstance);
		}
		return pInstance;
	}

}

void CSubModel::Free()
{
	__super::Free();
	for (auto& pAnimation : m_Animations)
	{
		Safe_Release(pAnimation);
	}
	m_Animations.clear();
	for (auto& pMeshContainer : m_MeshContainers)
	{
		Safe_Release(pMeshContainer);
	}
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		Safe_Release(pHierarchyNode);
	}
	m_HierarchyNodes.clear();

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_Release(pMaterial.pMaterials[i]);
		}		
	}
	m_Materials.clear();
	m_Importer.FreeScene();
}