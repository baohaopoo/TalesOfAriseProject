#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Texture.h"
#include "Shader.h"
#include "Animation.h"

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
	, m_isBinary(rhs.m_isBinary)
	, m_ModelDatadesc(rhs.m_ModelDatadesc)
	, m_vHierachyData(rhs.m_vHierachyData)
	, m_vMeshConData(rhs.m_vMeshConData)
	, m_vMatData(rhs.m_vMatData)
	, m_vAnimData(rhs.m_vAnimData)
	, m_iNumNodes(rhs.m_iNumNodes)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(pMaterial.pMaterials[i]);
		}
	}

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);
}

HRESULT CModel::NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix)
{
	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	//dat파일이 존재하는지 체크
	char szFileName[MAX_PATH] = "";
	char szEXT[MAX_PATH] = "";
	char szresult[MAX_PATH] = "";
	strcat_s(szresult, sizeof(char)*MAX_PATH, pModelFilePath);

	_splitpath_s(szFullPath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);
	strcat_s(szFileName, sizeof(char) * MAX_PATH, ".dat");
	strcat_s(szresult, sizeof(char)*MAX_PATH, szFileName);

	_tchar		datFullPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szresult, (int)strlen(szresult), datFullPath, MAX_PATH);

	m_eType = eType;

	HANDLE		hFile = CreateFile(datFullPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	_uint		iFlag;

	if (TYPE_NONANIM == m_eType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality;
	else if (TYPE_EFFECT == m_eType)
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	if (INVALID_HANDLE_VALUE == hFile || TYPE_EFFECT == m_eType) {
		CloseHandle(hFile);
		//2진화 파일이 없으면

		m_pScene = m_Importer.ReadFile(szFullPath, iFlag);
		if (nullptr == m_pScene) {
			MSG_BOX(L"Model Component -> m_pScene = nullptr !!");
			return E_FAIL;
		}

		if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode))) {
			MSG_BOX(L"Failed Ready_HierarchyNodes");
			return E_FAIL;
		}

		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

		if (FAILED(Ready_MeshContainers())) {
			MSG_BOX(L"Failed Ready_MeshContainer");
			return E_FAIL;
		}

		if (FAILED(Ready_Materials(pModelFilePath))) {
			MSG_BOX(L"Failed Ready_Materials");
			return E_FAIL;
		}

		if (FAILED(Set_HierarchyNodes())) {
			return E_FAIL;
		}

		if (FAILED(Ready_Animations())) {
			MSG_BOX(L"Failed Ready_Animations");
			return E_FAIL;
		}
		if (m_eType != TYPE_EFFECT)
		{
			if (FAILED(Save_Data(datFullPath))) {
				MSG_BOX(L"Failed to Save FBX at .dat File");
				return E_FAIL;
			}

		}
	}
	else { //2진화 파일이 있으면
		CloseHandle(hFile);
		m_isBinary = true;

		if (FAILED(Load_Data(datFullPath))) {
			MSG_BOX(L"Failed to Load Fbx !!");
			return E_FAIL;
		}

		if (FAILED(Ready_HierarchyNodes())) {
			MSG_BOX(L"Failed Binary Ready_HierarchyNodes !!");
			return E_FAIL;
		}

		sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

		if (FAILED(Ready_MeshContainers())) {
			MSG_BOX(L"Failed Binary Ready_MeshContainer");
			return E_FAIL;
		}

		if (FAILED(Ready_Materials())) {
			MSG_BOX(L"Failed Ready_Materials");
			return E_FAIL;
		}

		if (FAILED(Ready_Animations())) {
			MSG_BOX(L"Failed Ready_Animations");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel::NativeConstruct(void* pArg)
{
	if (m_isBinary == false) {
		if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode, 0)))
			return E_FAIL;
	}
	else {
		if (FAILED(Ready_HierarchyNodes()))
			return E_FAIL;
	}

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Clone_MeshContainers()))
		return E_FAIL;

	if (FAILED(Clone_Animations()))
		return E_FAIL;

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

void CModel::Monster_Update(_double TimeDelta, _bool bRoof)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
	{
		return;
	}
	m_Animations[m_iCurrentAnimIndex]->MonsterUpdate(TimeDelta, bRoof);
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

void CModel::Monster_Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex)
{
	if (iNextAnimIndex >= m_iNumAnimations)
	{
		return;
	}
	vector<CChannel*> vChannel = m_Animations[iNextAnimIndex]->Get_Channels();
	m_Animations[m_iCurrentAnimIndex]->MonsterUpdate_NextAnim(TimeDelta, Duration, iNextAnimIndex, pCurrentAnimIndex, &m_iCurrentAnimIndex, vChannel);
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

HRESULT CModel::Save_Data(const _tchar * FilePath)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(FilePath, GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile) {
		MSG_BOX(L"Failed Model Save !!");
		return false;
	}
	WriteFile(hFile, &m_ModelDatadesc, sizeof(MODELDATADESC), &dwByte, nullptr);

	for (int i = 0; i < m_HierarchyNodes.size(); i++) {
		WriteFile(hFile, &m_HierarchyNodes[i]->Get_Data(), sizeof(HIERARCHYDESC), &dwByte, nullptr);
	}

	for (int i = 0; i < m_vMatData.size(); i++) {
		WriteFile(hFile, &m_vMatData[i], sizeof(MATERIALSDESC), &dwByte, nullptr);
	}


	for (auto& iter : m_MeshContainers) {
		MESHCONTAINERDESC MeshData = iter->Get_Data();
		WriteFile(hFile, &MeshData.eType, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &MeshData.NumVertices, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &MeshData.NumPrimitive, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &MeshData.MaterialIndex, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &MeshData.Pivot, sizeof(_float4x4), &dwByte, nullptr);

		for (int i = 0; i < MeshData.NumVertices; i++) {
			WriteFile(hFile, &MeshData.VtxPosition[i], sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &MeshData.VtxNormal[i], sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &MeshData.VtxTexUV[i], sizeof(_float2), &dwByte, nullptr);
			WriteFile(hFile, &MeshData.Tangent[i], sizeof(_float3), &dwByte, nullptr);
		}

		for (int i = 0; i < MeshData.NumPrimitive; i++)
			for (int j = 0; j < 3; j++)
				WriteFile(hFile, &MeshData.Indices[i * 3 + j], sizeof(int), &dwByte, nullptr);

		if (MeshData.eType == 1) {
			WriteFile(hFile, &MeshData.NumBone, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &MeshData.MeshName, sizeof(MeshData.MeshName), &dwByte, nullptr);

			for (int i = 0; i < MeshData.NumBone; i++) {
				WriteFile(hFile, &MeshData.NumMeshNode[i], sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &MeshData.NumWeights[i], sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &MeshData.BoneOffsetMatrix[i], sizeof(_float4x4), &dwByte, nullptr);
			}

			for (int i = 0; i < MeshData.NumVertices; i++) {
				WriteFile(hFile, &MeshData.BlendIndex[i], sizeof(XMUINT4), &dwByte, nullptr);
				WriteFile(hFile, &MeshData.BlendWeight[i], sizeof(XMFLOAT4), &dwByte, nullptr);
			}
		}
		else {
			WriteFile(hFile, &MeshData.MeshName, sizeof(MeshData.MeshName), &dwByte, nullptr);
			WriteFile(hFile, &MeshData.NumMeshNode[0], sizeof(_int), &dwByte, nullptr);
		}
	}

	for (auto& iter : m_Animations) {
		ANIMATIONDESC Data = iter->Get_Data();

		WriteFile(hFile, &Data.Name, sizeof(Data.Name), &dwByte, nullptr);
		WriteFile(hFile, &Data.Duration, sizeof(double), &dwByte, nullptr);
		WriteFile(hFile, &Data.TickPerSecond, sizeof(double), &dwByte, nullptr);
		WriteFile(hFile, &Data.NumChannels, sizeof(double), &dwByte, nullptr);

		for (int i = 0; i < Data.NumChannels; i++) {
			WriteFile(hFile, &Data.NumChannelNode[i], sizeof(int), &dwByte, nullptr);

			// ChannelDesc받아야함
			CHANNELDESC ChannelData = Data.ChannelDesc[i];
			WriteFile(hFile, &ChannelData.NodeName, sizeof(ChannelData.NodeName), &dwByte, nullptr);
			WriteFile(hFile, &ChannelData.NumKeyFrames, sizeof(int), &dwByte, nullptr);

			for (int j = 0; j < ChannelData.NumKeyFrames; j++) {
				KEYFRAME KeyData = ChannelData.KeyFrame[j];
				WriteFile(hFile, &KeyData, sizeof(KEYFRAME), &dwByte, nullptr);
			}
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Load_Data(const _tchar * FilePath)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(FilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile) {
		MSG_BOX(L"Failed Model LoadData !!");
		return false;
	}

	if (INVALID_HANDLE_VALUE == hFile) {
		MSG_BOX(L"FAILED !!!");
		return false;
	}
	ReadFile(hFile, &m_ModelDatadesc, sizeof(MODELDATADESC), &dwByte, nullptr);

	for (int i = 0; i < m_ModelDatadesc.NumNodes; i++) {
		HIERARCHYDESC temp;
		ReadFile(hFile, &temp, sizeof(HIERARCHYDESC), &dwByte, nullptr);
		m_vHierachyData.push_back(temp);
	}

	for (int i = 0; i < m_ModelDatadesc.NumMatData; i++) {
		MATERIALSDESC temp;
		ReadFile(hFile, &temp, sizeof(MATERIALSDESC), &dwByte, nullptr);
		m_vMatData.push_back(temp);
	}

	//여기까진 OK

	for (int i = 0; i < m_ModelDatadesc.NumMeshContainers; i++) {
		MESHCONTAINERDESC temp;
		ZeroMemory(&temp, sizeof(MESHCONTAINERDESC));

		ReadFile(hFile, &temp.eType, sizeof(int), &dwByte, nullptr);

		ReadFile(hFile, &temp.NumVertices, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &temp.NumPrimitive, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &temp.MaterialIndex, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &temp.Pivot, sizeof(XMFLOAT4X4), &dwByte, nullptr);

		temp.VtxPosition = new _float3[temp.NumVertices];
		temp.VtxNormal = new _float3[temp.NumVertices];
		temp.VtxTexUV = new _float2[temp.NumVertices];
		temp.Tangent = new _float3[temp.NumVertices];
		temp.Indices = new int[temp.NumPrimitive * 3];

		for (int i = 0; i < temp.NumVertices; i++) {
			ReadFile(hFile, &temp.VtxPosition[i], sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &temp.VtxNormal[i], sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &temp.VtxTexUV[i], sizeof(_float2), &dwByte, nullptr);
			ReadFile(hFile, &temp.Tangent[i], sizeof(_float3), &dwByte, nullptr);
		}

		for (int i = 0; i < temp.NumPrimitive; i++)
			for (int j = 0; j < 3; j++) {
				ReadFile(hFile, &temp.Indices[i * 3 + j], sizeof(int), &dwByte, nullptr);
			}

		if (temp.eType == 1) {
			ReadFile(hFile, &temp.NumBone, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &temp.MeshName, sizeof(temp.MeshName), &dwByte, nullptr);
			temp.NumMeshNode = new int[temp.NumBone];
			temp.NumWeights = new int[temp.NumBone];
			temp.BoneOffsetMatrix = new _float4x4[temp.NumBone];
			temp.BlendIndex = new XMUINT4[temp.NumVertices];
			temp.BlendWeight = new XMFLOAT4[temp.NumVertices];

			for (int i = 0; i < temp.NumBone; i++) {
				ReadFile(hFile, &temp.NumMeshNode[i], sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, &temp.NumWeights[i], sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, &temp.BoneOffsetMatrix[i], sizeof(_float4x4), &dwByte, nullptr);
			}

			for (int i = 0; i < temp.NumVertices; i++) {
				ReadFile(hFile, &temp.BlendIndex[i], sizeof(XMUINT4), &dwByte, nullptr);
				ReadFile(hFile, &temp.BlendWeight[i], sizeof(XMFLOAT4), &dwByte, nullptr);
			}
		}
		else {
			temp.NumMeshNode = new int;
			ReadFile(hFile, &temp.MeshName, sizeof(temp.MeshName), &dwByte, nullptr);
			ReadFile(hFile, &temp.NumMeshNode[0], sizeof(int), &dwByte, nullptr);
			temp.NumBone = 0;
			temp.NumWeights = nullptr;
			temp.BoneOffsetMatrix = nullptr;
			temp.BlendIndex = nullptr;
			temp.BlendWeight = nullptr;
		}

		m_vMeshConData.push_back(temp);
	}


	for (int i = 0; i < m_ModelDatadesc.NumAnimations; i++) {
		ANIMATIONDESC Data;
		ReadFile(hFile, &Data.Name, sizeof(Data.Name), &dwByte, nullptr);
		ReadFile(hFile, &Data.Duration, sizeof(double), &dwByte, nullptr);
		ReadFile(hFile, &Data.TickPerSecond, sizeof(double), &dwByte, nullptr);
		ReadFile(hFile, &Data.NumChannels, sizeof(double), &dwByte, nullptr);

		Data.NumChannelNode = new int[Data.NumChannels];
		Data.ChannelDesc = new CHANNELDESC[Data.NumChannels];

		for (int j = 0; j < Data.NumChannels; j++) {
			ReadFile(hFile, &Data.NumChannelNode[j], sizeof(int), &dwByte, nullptr);

			CHANNELDESC ChannelData;
			ReadFile(hFile, &ChannelData.NodeName, sizeof(ChannelData.NodeName), &dwByte, nullptr);
			ReadFile(hFile, &ChannelData.NumKeyFrames, sizeof(int), &dwByte, nullptr);
			ChannelData.KeyFrame = new KEYFRAME[ChannelData.NumKeyFrames];

			for (int k = 0; k < ChannelData.NumKeyFrames; k++) {
				KEYFRAME KeyData;
				ReadFile(hFile, &KeyData, sizeof(KEYFRAME), &dwByte, nullptr);
				ChannelData.KeyFrame[k] = KeyData;
			}

			Data.ChannelDesc[j] = ChannelData;
		}

		m_vAnimData.push_back(Data);
	}


	CloseHandle(hFile);

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

	if (nullptr == m_Materials[iMaterialIndex].pMaterials[eType])
	{
		return pShader->Set_ShaderResourceView(pConstantName, nullptr);
	}

	return m_Materials[iMaterialIndex].pMaterials[eType]->SetUp_ShaderResourceView(pShader, pConstantName, 0);
}

HRESULT CModel::Ready_MeshContainers()
{
	if (m_isBinary == false) {
		if (nullptr == m_pScene)
			return E_FAIL;

		m_iNumMeshContainers = m_pScene->mNumMeshes;
		m_ModelDatadesc.NumMeshContainers = m_iNumMeshContainers;

		for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
		{
			CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_eType, m_pScene->mMeshes[i], m_PivotMatrix, m_HierarchyNodes);
			if (nullptr == pMeshContainer)
				return E_FAIL;

			m_MeshContainers.push_back(pMeshContainer);
		}
	}
	else {
		m_iNumMeshContainers = m_ModelDatadesc.NumMeshContainers;

		for (_uint i = 0; i < m_iNumMeshContainers; i++) {
			CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, &m_vMeshConData[i], m_HierarchyNodes);
			if (nullptr == pMeshContainer)
				return E_FAIL;

			m_MeshContainers.push_back(pMeshContainer);

		}
	}


	return S_OK;
}

HRESULT CModel::Clone_MeshContainers()
{
	if (m_isBinary == false) {
		if (nullptr == m_pScene)
			return E_FAIL;

		m_iNumMeshContainers = m_pScene->mNumMeshes;
	}
	else
		m_iNumMeshContainers = m_ModelDatadesc.NumMeshContainers;

	vector<CMeshContainer*>		MeshContainer;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = (CMeshContainer*)m_MeshContainers[i]->Clone(&m_HierarchyNodes);
		if (nullptr == pMeshContainer)
			return E_FAIL;

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
		return E_FAIL;

	m_iNumMaterials = m_pScene->mNumMaterials;
	m_ModelDatadesc.NumMataerails = m_iNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*			pAIMaterial = m_pScene->mMaterials[i];
		if (nullptr == pAIMaterial)
			return E_FAIL;

		MODELMATERIAL		Materials;
		ZeroMemory(&Materials, sizeof(MODELMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char		szTextureFilePath[MAX_PATH] = "";

			aiString	strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath))) {
				continue;
			}

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			strcpy_s(szTextureFilePath, pModelFilePath);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szEXT);

			_tchar		szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, (int)strlen(szTextureFilePath), szFullPath, MAX_PATH);
			Materials.pMaterials[j] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);
			if (nullptr == Materials.pMaterials[j])
				return E_FAIL;

			MATERIALSDESC matdesc;
			strcpy_s(matdesc.FilePath, szTextureFilePath);
			matdesc.MatIndex = i;
			matdesc.TexType = j;
			m_vMatData.push_back(matdesc);
		}
		m_Materials.push_back(Materials);
	}

	m_ModelDatadesc.NumMatData = (int)m_vMatData.size();

	return S_OK;
}

HRESULT CModel::Ready_Materials()
{
	m_iNumMaterials = m_ModelDatadesc.NumMataerails;

	int k = 0;

	for (_uint i = 0; i < m_iNumMaterials; i++) {
		MODELMATERIAL		Materials;
		ZeroMemory(&Materials, sizeof(MODELMATERIAL));

		for (int j = k; j < m_vMatData.size(); j++) {
			if (m_vMatData[j].MatIndex == i) {
				_tchar		szFullPath[MAX_PATH] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, m_vMatData[j].FilePath, (int)strlen(m_vMatData[j].FilePath), szFullPath, MAX_PATH);
				Materials.pMaterials[m_vMatData[j].TexType] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);

			}
			else {
				k = j;
				break;
			}
		}

		m_Materials.push_back(Materials);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	if (m_eType == CModel::TYPE_NONANIM)
		return S_OK;

	if (m_eType == CModel::TYPE_EFFECT)
		return S_OK;


	if (m_isBinary == false) {
		if (nullptr == m_pScene)
			return E_FAIL;

		m_iNumAnimations = m_pScene->mNumAnimations;
		m_ModelDatadesc.NumAnimations = m_iNumAnimations;

		for (_uint i = 0; i < m_iNumAnimations; ++i)
		{
			aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];

			CAnimation*		pAnimation = CAnimation::Create(pAIAnimation, m_HierarchyNodes);
			if (nullptr == pAnimation)
				return E_FAIL;

			m_Animations.push_back(pAnimation);
		}
	}
	else {
		m_iNumAnimations = m_ModelDatadesc.NumAnimations;

		for (_uint i = 0; i < m_iNumAnimations; i++) {
			CAnimation*		pAnimation = CAnimation::Create(&m_vAnimData[i], m_HierarchyNodes);
			if (nullptr == pAnimation)
				return E_FAIL;

			m_Animations.push_back(pAnimation);
		}
	}

	return S_OK;
}

HRESULT CModel::Clone_Animations()
{
	if (m_isBinary == false) {
		if (nullptr == m_pScene)
			return E_FAIL;

		vector<CAnimation*>		Animations;

		for (_uint i = 0; i < m_iNumAnimations; ++i)
		{
			aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];

			CAnimation*		pAnimation = m_Animations[i]->Clone(pAIAnimation, m_HierarchyNodes);
			if (nullptr == pAnimation)
				return E_FAIL;

			Safe_Release(m_Animations[i]);

			Animations.push_back(pAnimation);
		}

		m_Animations = Animations;
	}
	else {
		vector<CAnimation*>		Animations;

		for (_uint i = 0; i < m_iNumAnimations; ++i)
		{
			CAnimation*		pAnimation = m_Animations[i]->Clone(&m_vAnimData[i], m_HierarchyNodes);
			if (nullptr == pAnimation)
				return E_FAIL;

			Safe_Release(m_Animations[i]);

			Animations.push_back(pAnimation);
		}

		m_Animations = Animations;
	}
	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	_uint iNumChildren = pNode->mNumChildren;
	m_ModelDatadesc.NumChildren = m_iNumChildren;

	CHierarchyNode*			pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	for (_uint i = 0; i < iNumChildren; ++i)
	{
		if (FAILED(Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1)))
			return E_FAIL;
	}

	m_HierarchyNodes.push_back(pHierarchyNode);
	m_iNumNodes = (int)m_HierarchyNodes.size();
	m_ModelDatadesc.NumNodes = m_iNumNodes;

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes()
{
	m_iNumChildren = m_ModelDatadesc.NumChildren;
	m_iNumNodes = m_ModelDatadesc.NumNodes;

	for (int i = 0; i < m_vHierachyData.size(); i++) {
		CHierarchyNode*			pHierarchyNode = CHierarchyNode::Create(&m_vHierachyData[i]);
		m_HierarchyNodes.push_back(pHierarchyNode);
	}

	if (m_eType == CModel::TYPE_ANIM) {
		for (int i = 0; i < m_HierarchyNodes.size(); i++) {
			int parentindex = m_HierarchyNodes[i]->Get_ParentIndex();
			if (parentindex == -1)
				continue;
			m_HierarchyNodes[i]->Set_HierarchyParent(m_HierarchyNodes[parentindex]);
		}
	}
	if (m_iNumNodes != m_HierarchyNodes.size())
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Index() < pDest->Get_Index();
	});

	return S_OK;
}

HRESULT CModel::Set_HierarchyNodes()
{
	for (int i = 0; i < m_HierarchyNodes.size(); i++) {
		m_HierarchyNodes[i]->Set_Index(i);
	}

	for (int i = 0; i < m_HierarchyNodes.size(); i++) {
		if (m_HierarchyNodes[i]->Get_HierarchyParent() == nullptr) {
			m_HierarchyNodes[i]->Set_ParentIndex(-1);
			continue;
		}
		m_HierarchyNodes[i]->Set_ParentIndex(m_HierarchyNodes[i]->Get_HierarchyParent()->Get_Index());
	}

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
	if (m_isCloned == false) {
		for (auto MeshConData : m_vMeshConData) {
			Safe_Delete_Array(MeshConData.VtxPosition);
			Safe_Delete_Array(MeshConData.VtxNormal);
			Safe_Delete_Array(MeshConData.VtxTexUV);
			Safe_Delete_Array(MeshConData.Indices);
			Safe_Delete_Array(MeshConData.Tangent);
			Safe_Delete_Array(MeshConData.NumMeshNode);

			if (m_eType == 1) {
				Safe_Delete_Array(MeshConData.NumWeights);
				Safe_Delete_Array(MeshConData.BoneOffsetMatrix);
				Safe_Delete_Array(MeshConData.BlendIndex);
				Safe_Delete_Array(MeshConData.BlendWeight);
			}

		}
		for (auto& iter : m_vAnimData) {
			Safe_Delete_Array(iter.NumChannelNode);

			for (int i = 0; i < iter.NumChannels; i++) {
				for (int j = 0; j < iter.ChannelDesc[i].NumKeyFrames; j++) {
					Safe_Delete_Array(iter.ChannelDesc[i].KeyFrame);
				}
			}
			Safe_Delete_Array(iter.ChannelDesc);
		}
	}

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

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