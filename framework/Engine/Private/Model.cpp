#include "..\Public\Model.h"
#include "MeshContainer.h"

// binary
_uint CModel::m_iIdNum = 0;

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CModel::CModel(const CModel& rhs)
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

	// binary
	, m_pClonedHierarchyNodes(&rhs.m_HierarchyNodes)
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

HRESULT CModel::NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	m_eType = eType;

	// binary

	// 만약 같은 경로의 같은 이름의 dat파일을 열었을 경우
	string str{ szFullPath };
	_uint lastDotPos = str.find_last_of(".");
	string datFilePath = str.substr(0, lastDotPos) + string{ ".dat" };

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, datFilePath.c_str(), datFilePath.length(), szCurPath, MAX_PATH);

	// dat파일 파일을 Read 하는 방식으로 연다.
	HANDLE		hReadFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// 해당 파일이 없는 경우
	if (INVALID_HANDLE_VALUE == hReadFile) {

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
			MSG_BOX(L"CModel -> NativeConstruct_Prototype -> nullptr == m_pScene");
			return E_FAIL;
		}
		XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
		if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
		{
			MSG_BOX(L"CModel -> NativeConstruct_Prototype -> Ready_HierarchyNodes");
			return E_FAIL;
		}
		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});
		if (FAILED(Ready_MeshContainers()))
		{
			MSG_BOX(L"CModel -> NativeConstruct_Prototype -> Ready_MeshContainers");
			return E_FAIL;
		}
		if (FAILED(Ready_Materials(pModelFilePath)))
		{
			MSG_BOX(L"CModel -> NativeConstruct_Prototype -> Ready_Materials");
			return E_FAIL;
		}
		if (FAILED(Ready_Animations()))
		{
			MSG_BOX(L"CModel -> NativeConstruct_Prototype -> Ready_Animations");
			return E_FAIL;
		}
		
		// 로드가 끝났다면 
		// dat파일 파일을 Read 하는 방식으로 연다.
		HANDLE		hWriteFile = CreateFile(szCurPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// 해당 파일이 없는 경우
		if (INVALID_HANDLE_VALUE == hWriteFile) {

		}
		else {
			m_iIdNum = 0;

			if (FAILED(Save_FBX(hWriteFile))) {
				MSG_BOX(TEXT("Failed to Save FBX at .dat File"));
				return E_FAIL;
			}
		}

		CloseHandle(hWriteFile);
	}
	// dat 파일이 존재하는 경우
	else {
		// 피봇 행렬을 저장한다.
		XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

		// Dat 파일로 모델을 연다.
		if (FAILED(Load_FBX(hReadFile)))
			return E_FAIL;
	}

	CloseHandle(hReadFile);

	return S_OK;
}

HRESULT CModel::NativeConstruct(void* pArg)
{
	// binary
	if (nullptr == m_pScene) {
		if (FAILED(Clone_Dat_HierarchyNodes()))
			return E_FAIL;

		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});
		m_pRootNode = m_HierarchyNodes.front();
		Safe_AddRef(m_pRootNode);

		if (FAILED(Clone_Dat_MeshContainers()))
			return E_FAIL;

		if (FAILED(Clone_Dat_Animations()))
			return E_FAIL;
	}
	else {
		if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
		{
			MSG_BOX(L"CModel -> NativeConstruct -> Ready_HierarchyNodes");
			return E_FAIL;
		}
		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});
		m_pRootNode = m_HierarchyNodes.front();
		Safe_AddRef(m_pRootNode);

		if (FAILED(Clone_MeshContainers()))
		{
			MSG_BOX(L"CModel -> NativeConstruct -> Clone_MeshContainers");
			return E_FAIL;
		}
		if (FAILED(Clone_Animations()))
		{
			MSG_BOX(L"CModel -> NativeConstruct -> Clone_Animations");
			return E_FAIL;
		}
	}

	return S_OK;
}


void CModel::Update(_double TimeDelta, _bool bRoof)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
	{
		return;
	}
	m_Animations[m_iCurrentAnimIndex]->Update(TimeDelta, bRoof);
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}
}

void CModel::Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex)
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

HRESULT CModel::Render(CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
	{
		MSG_BOX(L"CModel -> Render -> iMeshContainerIndex >= m_iNumMeshContainers");
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

_float4x4 * CModel::Get_CombinedTransformationMatrix(const char * pNodeName)
{
	CHierarchyNode*		pNode = nullptr;

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		if (!strcmp(pNodeName, pHierarchyNode->Get_Name()))
		{
			pNode = pHierarchyNode;
			break;
		}
	}

	if (nullptr == pNode)
		return nullptr;

	return pNode->Get_CombinedFloat4x4Ptr();
}

_uint CModel::Get_NumMeshContainer() const
{
	return m_iNumMeshContainers;
}

CAnimation* CModel::Get_Animation(_uint iIndex)
{
	return m_Animations[iIndex];
}

CAnimation * CModel::Get_CurAnimation()
{
	return m_Animations[m_iCurrentAnimIndex];
}

_uint CModel::Get_NumAnimation()
{
	return m_iNumAnimations;
}

void CModel::Set_AnimationIndex(_uint iAnimIndex)
{
	m_iCurrentAnimIndex = iAnimIndex;
}

HRESULT CModel::Bind_Material_OnShader(CShader * pShader, aiTextureType eType, const char * pConstantName, _uint iMeshContainerIndex)
{
	_uint iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_NumMaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_BOX(L"CModel -> Bind_Material_OnShader -> iMaterialIndex >= m_iNumMaterials");
		return E_FAIL;
	}
	return m_Materials[iMaterialIndex].pMaterials[eType]->SetUp_ShaderResourceView(pShader, pConstantName, 0);
}

HRESULT CModel::Ready_MeshContainers()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Ready_MeshContainers -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMeshContainers = m_pScene->mNumMeshes;
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_eType, m_pScene->mMeshes[i], m_PivotMatrix, m_HierarchyNodes);
		if (nullptr == pMeshContainer)
		{
			MSG_BOX(L"CModel -> Ready_MeshContainers -> nullptr == pMeshContainer");
			return E_FAIL;
		}
		m_MeshContainers.push_back(pMeshContainer);
	}
	return S_OK;
}

HRESULT CModel::Clone_MeshContainers()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Clone_MeshContainers -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMeshContainers = m_pScene->mNumMeshes;
	vector<CMeshContainer*> MeshContainer;
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = (CMeshContainer*)m_MeshContainers[i]->Clone(&m_HierarchyNodes);
		if (nullptr == pMeshContainer)
		{
			MSG_BOX(L"CModel -> Clone_MeshContainers -> nullptr == pMeshContainer");
			return E_FAIL;
		}
		Safe_Release(m_MeshContainers[i]);
		MeshContainer.push_back(pMeshContainer);
	}
	m_MeshContainers.clear();
	m_MeshContainers = MeshContainer;
	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Ready_Materials -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumMaterials = m_pScene->mNumMaterials;
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pScene->mMaterials[i];
		if (nullptr == pAIMaterial)
		{
			MSG_BOX(L"CModel -> Ready_Materials -> nullptr == pAIMaterial");
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
				MSG_BOX(L"CModel -> Ready_Materials -> nullptr == Materials.pMaterials");
				return E_FAIL;
			}
		}
		m_Materials.push_back(Materials);
	}
	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Ready_Animations -> nullptr == m_pScene");
		return E_FAIL;
	}
	m_iNumAnimations = m_pScene->mNumAnimations;
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation* pAIAnimation = m_pScene->mAnimations[i];
		CAnimation* pAnimation = CAnimation::Create(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
		{
			MSG_BOX(L"CModel -> Ready_Animations -> nullptr == pAnimation");
			return E_FAIL;
		}
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CModel::Clone_Animations()
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Clone_Animations -> nullptr == m_pScene");
		return E_FAIL;
	}
	vector<CAnimation*> Animations;
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];
		CAnimation* pAnimation = m_Animations[i]->Clone(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
		{
			MSG_BOX(L"CModel -> Clone_Animations -> nullptr == pAnimation");
			return E_FAIL;
		}
		Safe_Release(m_Animations[i]);
		Animations.push_back(pAnimation);
	}
	m_Animations = Animations;
	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	if (nullptr == m_pScene)
	{
		MSG_BOX(L"CModel -> Ready_HierarchyNodes -> nullptr == m_pScene");
		return E_FAIL;
	}
	_uint iNumChildren = pNode->mNumChildren;
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth);
	if (nullptr == pHierarchyNode)
	{
		MSG_BOX(L"CModel -> Ready_HierarchyNodes -> nullptr == pHierarchyNode");
		return E_FAIL;
	}
	for (_uint i = 0; i < iNumChildren; ++i)
	{
		if (FAILED(Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1)))
		{
			MSG_BOX(L"CModel -> Ready_HierarchyNodes -> Ready_HierarchyNodes");
			return E_FAIL;
		}
	}

	// binary
	pHierarchyNode->Set_HierarchyNumber(++m_iIdNum);

	m_HierarchyNodes.push_back(pHierarchyNode);
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(L"CModel -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	if (m_eType == TYPE_EFFECT)
	{
		AddRef();
		return this;
	}
	else
	{
		CModel*	pInstance = new CModel(*this);

		if (FAILED(pInstance->NativeConstruct(pArg)))
		{
			MSG_BOX(L"CModel -> Clone -> pInstance->NativeConstruct");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
}

void CModel::Free()
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

	Safe_Release(m_pRootNode);
	m_Importer.FreeScene();
}

HRESULT CModel::Save_FBX(HANDLE& hFile)
{
	// 파일이 존재하지 않는 경우
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 메쉬의 타입 저장
	dwStrByte = m_eType;
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	// HierarchyNodes 정보 저장 - 갯수, 내부내용
	dwStrByte = m_HierarchyNodes.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	for (auto& Node : m_HierarchyNodes) {
		// 자신의 정보를 저장한다.
		Node->SaveDatInfo(hFile);
	}

	// m_HierarchyNodes의 갯수만큼 반복한다.
	for (auto& pNode : m_HierarchyNodes) {
		// 부모 노드가 없다면
		if (nullptr == pNode->GetParent()) {
			// -1을 저장한다.
			_uint iParentNum = -1;
			WriteFile(hFile, &iParentNum, sizeof(_uint), &dwByte, nullptr);
		}
		// 부모 노드가 존재한다면 - 반드시 HierarchyNumber는 존재한다.
		else {
			// 부모의 HierarchyNumber를 가져온다.
			_uint iParentNum = pNode->GetParent()->Get_HierarchyNumber();

			// 부모의 HierarchyNumber를 가진 인덱스를 찾아 해당 인덱스 번호를 저장한다.
			auto& iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&iParentNum](CHierarchyNode* pNode) {
				return pNode->Get_HierarchyNumber() == iParentNum;
			});

			// 거리를 구해 넣는다. 이것이 인덱스 번호이다. 
			_uint iIndex = distance(m_HierarchyNodes.begin(), iter);
			pNode->Set_ParentIndex(iIndex);

			// 부모 노드의 인덱스 번호를 저장한다.
			WriteFile(hFile, &iIndex, sizeof(_uint), &dwByte, nullptr);
		}
	}



	// HierarchyNodes에 대한 정보 저장이 끝났으니, 메쉬 컨테이너로 가서 메쉬 컨테이너의 갯수를 저장한다.
	dwStrByte = m_MeshContainers.size();
	WriteFile(hFile, &dwStrByte, sizeof(_uint), &dwByte, nullptr);

	// 메쉬 컨테이너의 갯수만큼 for문을 돌며 모델의 타입에 따라 다른 값을 저장한다.
	// TYPE_NONANIM : 초기 필요한 정보 + Indices + Vertices
	// TYPE_ANIM : 초기 필요한 정보 + Indices + Vertices + 애니메이션의 채널정보(키프레임)
	for (int i = 0; i < m_MeshContainers.size(); ++i) {
		// TYPE_NONANIM 일 경우
		if (TYPE_NONANIM == m_eType ||
			TYPE_EFFECT == m_eType) {
			// Vertices 저장
			m_MeshContainers[i]->Save_VertexBuffer_NoAnim(hFile, m_pScene->mMeshes[i]);
		}
		// TYPE_ANIM 경우
		else if (TYPE_ANIM == m_eType) {
			// 버티시즈 및 인디시즈 저장
			m_MeshContainers[i]->Save_VertexBuffer_Anim(hFile, m_pScene->mMeshes[i], m_HierarchyNodes);
		}

		// Indices 저장
		m_MeshContainers[i]->Save_IndexBuffer(hFile, m_pScene->mMeshes[i]);
	}



	dwStrByte = 9871;
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	// 마테리얼 정보 저장

	// 마테리얼의 총갯수 저장
	dwStrByte = m_Materials.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	vector<MATERIALTEXTURE> vMTInfo;

	// 텍스쳐와 관련된 정보를 저장
	for (int i = 0; i < m_Materials.size(); ++i) {
		for (int j = 0; j < AI_TEXTURE_TYPE_MAX; ++j) {
			if (nullptr == m_Materials[i].pMaterials[j])
				continue;

			_tchar strUnicode[MAX_PATH] = TEXT("");

			char strMultibyte[MAX_PATH];
			ZeroMemory(strMultibyte, MAX_PATH);

			wcscpy_s(strUnicode, MAX_PATH, m_Materials[i].pMaterials[j]->Get_TextureFilePath());

			int len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL);

			MATERIALTEXTURE MT;
			MT.iMaterialIndex = i;
			MT.iAI_TEXTURE_TYPE = j;
			strcpy_s(MT.strFilePath, strMultibyte);
			vMTInfo.push_back(MT);
		}
	}

	// 존재하는 텍스쳐 정보를 담고 있는 vMTInfo의 사이즈 저장
	dwStrByte = vMTInfo.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	// vMTInfo의 정보 저장
	WriteFile(hFile, vMTInfo.data(), sizeof(MATERIALTEXTURE) * vMTInfo.size(), &dwByte, nullptr);






	// 애니메이션 정보 저장

	// 전체 애니메이션의 갯수를 저장한다.
	dwStrByte = m_Animations.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	for (auto& Animation : m_Animations) {
		Animation->Save_AnimationInfo(hFile);
	}

	return S_OK;
}

HRESULT CModel::Load_FBX(HANDLE & hFile)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 모델의 타입을 가져온다.
	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	m_eType = (TYPE)dwStrByte;

	// 전체 HierarchyNode의 갯수를 가져온다.
	_uint iHierarchyNodesCnt = 0;
	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	iHierarchyNodesCnt = (_uint)dwStrByte;

	char szName[MAX_PATH];	// 이름
	char temp[MAX_PATH];	// 줄 건너뛸 때 사용하는 값
	_uint iDepth = 0;
	_uint iParentNum = 0;

	_float4x4 TransformationMatrixTP;

	// 가져온 노드의 갯수만큼 반복하며 노드를 생성한다.
	for (_uint i = 0; i < iHierarchyNodesCnt; ++i) {
		// 이름 정보
		// 깊이, 부모 번호의 정보를 읽는다.

		// 해당 문자열의 사이즈 저장
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// 문자열의 메모리를 문자열의 사이즈만큼 저장
		ReadFile(hFile, szName, dwStrByte, &dwByte, nullptr);


		// 깊이 정보 저장
		ReadFile(hFile, &iDepth, sizeof(_uint), &dwByte, nullptr);

		// 부모 노드 정보 저장
		ReadFile(hFile, &iParentNum, sizeof(_uint), &dwByte, nullptr);

		// 행렬 정보 저장
		ReadFile(hFile, &TransformationMatrixTP, sizeof(_float4x4), &dwByte, nullptr);

		m_HierarchyNodes.push_back(CHierarchyNode::Create(szName, XMLoadFloat4x4(&TransformationMatrixTP), iDepth));
	}



	// HierarchyNode 부모 노드 설정
	for (_uint i = 0; i < iHierarchyNodesCnt; ++i) {
		int iIndex = 0;
		ReadFile(hFile, &iIndex, sizeof(_uint), &dwByte, nullptr);

		if (-1 != iIndex) {
			m_HierarchyNodes[i]->SetParent(m_HierarchyNodes[iIndex]);
			m_HierarchyNodes[i]->Set_ParentIndex(iIndex);
		}
	}



	// 메쉬 컨테이너 정보 로드
	ReadFile(hFile, &m_iNumMeshContainers, sizeof(_uint), &dwByte, nullptr);

	for (int i = 0; i < m_iNumMeshContainers; ++i) {
		Load_VertexBuffer(hFile);
	}




	// dwStrByte = 9871;
	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);


	// 마테리얼 정보 로드
	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	// 모델이 사용하는 마테리얼의 개수
	int iMTInfoSize = 0;
	ReadFile(hFile, &iMTInfoSize, sizeof(_uint), &dwByte, nullptr);


	vector<MATERIALTEXTURE> vMTInfo;
	MATERIALTEXTURE MT;

	vMTInfo.resize(iMTInfoSize);
	ReadFile(hFile, vMTInfo.data(), sizeof(MATERIALTEXTURE) * iMTInfoSize, &dwByte, nullptr);

	for (int i = 0; i < m_iNumMaterials; ++i) {
		MODELMATERIAL* MM = new MODELMATERIAL;
		ZeroMemory(MM, sizeof(MODELMATERIAL));
		m_Materials.push_back(*MM);
	}

	for (auto& MT : vMTInfo) {
		_tchar		szFullPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, MT.strFilePath, strlen(MT.strFilePath), szFullPath, MAX_PATH);
		m_Materials[MT.iMaterialIndex].pMaterials[MT.iAI_TEXTURE_TYPE] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);
	}




	// 애니메이션 정보 저장

	// 전체 애니메이션의 갯수를 저장한다.
	ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	m_iNumAnimations = dwStrByte;

	for (int i = 0; i < m_iNumAnimations; ++i) {
		Load_Animation(hFile);
	}

	return S_OK;
}

HRESULT CModel::Load_VertexBuffer(HANDLE & hFile)
{
	CMeshContainer* pContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_eType, hFile, m_PivotMatrix, m_HierarchyNodes);

	if (nullptr == pContainer)
		return E_FAIL;

	m_MeshContainers.push_back(pContainer);

	return S_OK;
}

HRESULT CModel::Load_Animation(HANDLE & hFile)
{
	CAnimation* pAnimation = CAnimation::Create(hFile, m_HierarchyNodes);

	if (nullptr == pAnimation)
		return E_FAIL;

	m_Animations.push_back(pAnimation);

	return S_OK;
}

HRESULT CModel::Clone_Dat_HierarchyNodes()
{
	int* iIndex = new int[m_pClonedHierarchyNodes->size()];

	// 루트 노드를 제외하기에 1부터 시작
	iIndex[0] = 0;

	for (int i = 1; i < m_pClonedHierarchyNodes->size(); ++i) {
		iIndex[i] = m_pClonedHierarchyNodes->at(i)->Get_ParentIndex();
	}

	for (int i = 0; i < m_pClonedHierarchyNodes->size(); ++i) {
		m_HierarchyNodes.push_back(m_pClonedHierarchyNodes->at(i)->Clone());
	}

	// 루트 노드를 제외하기에 1부터 시작
	for (int i = 1; i < m_pClonedHierarchyNodes->size(); ++i) {
		m_HierarchyNodes[i]->Set_ParentIndex(iIndex[i]);
		m_HierarchyNodes[i]->SetParent(m_HierarchyNodes[iIndex[i]]);
	}

	Safe_Delete_Array(iIndex);

	return S_OK;
}

HRESULT CModel::Clone_Dat_MeshContainers()
{
	vector<CMeshContainer*>		MeshContainer;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = (CMeshContainer*)m_MeshContainers[i]->Clone_Dat(&m_HierarchyNodes);

		if (nullptr == pMeshContainer)
			return E_FAIL;


		MeshContainer.push_back(pMeshContainer);
	}
	m_MeshContainers.clear();

	m_MeshContainers = MeshContainer;

	return S_OK;
}

HRESULT CModel::Clone_Dat_Animations()
{
	vector<CAnimation*>		Animations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation*		pAnimation = m_Animations[i]->Clone(m_HierarchyNodes);
		if (nullptr == pAnimation)
			return E_FAIL;

		Safe_Release(m_Animations[i]);

		Animations.push_back(pAnimation);
	}

	m_Animations = Animations;

	return S_OK;
}
