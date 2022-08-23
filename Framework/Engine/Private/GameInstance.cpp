
#include "../Public/GameInstance.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pPhysX(CPhysX::GetInstance())
	, m_pPicking_Manager(CPicking_Manager::GetInstance())
	, m_pPostProcessing(CPostProcessing::GetInstance())

#ifdef _DEBUG
	, m_pConsole_Debug(CConsole_Debug::GetInstance())
#endif // _DEBUG

{
	Safe_AddRef(m_pPicking_Manager);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pPhysX);
	Safe_AddRef(m_pPostProcessing);
#ifdef _DEBUG
	Safe_AddRef(m_pConsole_Debug);
#endif // _DEBUG
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->InitDevice(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pLight_Manager->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pFrustum->NativeConstruct()))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pPhysX->Initialize()))
		return E_FAIL;

	if (FAILED(m_pPicking_Manager->InitPicking(GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY))) {
		return E_FAIL;
	}

	if (FAILED(m_pPostProcessing->Init_PostProcessing(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;


	LoadDataFile_Effect();

	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pInput_Device)
		return;

	// 키 입력 관련 처리
	m_pInput_Device->SetUp_InputDeviceState();

	
	// ObjectMgr Tick and LateTick
	m_pObject_Manager->Tick(TimeDelta);
	m_pPhysX->Tick(TimeDelta);

	m_pPipeLine->Tick();
	m_pFrustum->Transform_ToWorldSpace();

	m_pObject_Manager->LateTick(TimeDelta);

	// LevelMgr Tick
	m_pLevel_Manager->Tick(TimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_Begin(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

_char CGameInstance::Get_DIKeyState(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_char CGameInstance::Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMButtonState(eDIMBState);
}

_long CGameInstance::Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eDIMMState);
}

HRESULT CGameInstance::Add_Timers(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timers(pTimerTag);
}

_double CGameInstance::Compute_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

_bool CGameInstance::Key_Pressing(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Pressing(byKeyID);
}

_bool CGameInstance::Key_Down(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Up(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Up(byKeyID);
}

_bool CGameInstance::Button_Pressing(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Button_Pressing(eDIMBState);
}

_bool CGameInstance::Button_Down(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Button_Down(eDIMBState);
}

_bool CGameInstance::Button_Up(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Button_Up(eDIMBState);
}

_bool CGameInstance::Picking(_float3 * pVerticesPos, _fmatrix WorldMatrix, _vector * pOut)
{
	if (nullptr == m_pPicking_Manager)
		return false;

	return m_pPicking_Manager->Picking(pVerticesPos, WorldMatrix, pOut);
}

_bool CGameInstance::Picking(_fvector vPos1, _fvector vPos2, _fvector vPos3, _fmatrix WorldMatrix, _vector * pOut)
{
	if (nullptr == m_pPicking_Manager)
		return false;

	return m_pPicking_Manager->Picking(vPos1, vPos2, vPos3, WorldMatrix, pOut);
}

_bool CGameInstance::Picking(_fvector vPos1, _fvector vPos2, _fvector vPos3, _fmatrix WorldMatrix, _vector * pPos, _vector * pDistance)
{
	if (nullptr == m_pPicking_Manager)
		return false;

	return m_pPicking_Manager->Picking(vPos1, vPos2, vPos3, WorldMatrix, pPos, pDistance);
}

const CPicking_Manager::RAYDESC CGameInstance::Get_vRayDesc(void)
{
	if (nullptr == m_pPicking_Manager)
		return CPicking_Manager::RAYDESC{};

	return m_pPicking_Manager->Get_vRayDesc();
}

void CGameInstance::Set_RadialBlur(_bool bRadialBlur)
{
	if (nullptr == m_pPostProcessing)
		return;

	return m_pPostProcessing->Set_RadialBlur(bRadialBlur);
}

void CGameInstance::Set_RadialBlurLength(_float fLength)
{
	if (nullptr == m_pPostProcessing)
		return;

	return m_pPostProcessing->Set_RadialBlurLength(fLength);
}

void CGameInstance::Set_RadialNlurStrength(_float fStrength)
{
	if (nullptr == m_pPostProcessing)
		return;

	return m_pPostProcessing->Set_RadialNlurStrength(fStrength);
}

//HRESULT CGameInstance::Init_Sound(const _tchar* pSoundFilePath)
//{
//	if (nullptr == m_pSound_Manager)
//	{
//		MSG_BOX(L"Failed To CGameInstance : Init_Sound : nullptr == m_pSound_Manager");
//		return E_FAIL;
//	}
//	return m_pSound_Manager->NativeConstruct(pSoundFilePath);
//}
//
//void CGameInstance::Play_Sound(TCHAR* pSoundKey, CSound_Manager::CHANNELID eID, _float _vol)
//{
//	if (nullptr == m_pSound_Manager)
//	{
//		MSG_BOX(L"Failed To CGameInstance : Play_Sound : nullptr == m_pSound_Manager");
//		return;
//	}
//	m_pSound_Manager->Play_Sound(pSoundKey, eID, _vol);
//}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_iLevel = iLevelIndex;

	return m_pLevel_Manager->OpenLevel(iLevelIndex, pLevel);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CGameObject* CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_GameObjectToLayer : nullptr == m_pObject_Manager");
		return nullptr;
	}
	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

void CGameInstance::Delete_Component(const _tchar * pComponentTag)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Delete_Component(pComponentTag);
}

CLayer * CGameInstance::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Layer(iLevelIndex, pLayerTag);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
}

CLayer * CGameInstance::Add_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_Layer(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_TP(eState);
}

_bool CGameInstance::isIn_LocalSpace(CTransform* pTransform, _float fRange)
{
	if (nullptr == m_pFrustum)
	{
		MSG_BOX(L"Failed To CGameInstance : isIn_LocalSpace : nullptr == m_pFrustum");
		return false;
	}
	_vector vLocalPos = XMVector3TransformCoord(pTransform->Get_State(CTransform::STATE_POSITION), pTransform->Get_WorldMatrixInverse());
	return m_pFrustum->isIn_WorldSpace(vLocalPos, fRange);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorSet(0.0f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

_float4 CGameInstance::Get_CamPositionFloat4()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.0f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPositionFloat4();
}

_float CGameInstance::Get_CamFar()
{
	if (nullptr == m_pPipeLine)
		return 0.f;

	return m_pPipeLine->Get_CamFar();
}

_matrix CGameInstance::Get_LightTransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_LightTransformMatrix(eState);
}

_float4x4 CGameInstance::Get_LightTransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_LightTransformFloat4x4_TP(eState);
}

_vector CGameInstance::Get_LightPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorSet(0.0f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_LightPosition();
}

_float4 CGameInstance::Get_LightPositionFloat4()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.0f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_LightPositionFloat4();
}

_float CGameInstance::Get_LightFar()
{
	if (nullptr == m_pPipeLine)
		return 0.f;

	return m_pPipeLine->Get_LightFar();
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Lights(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Lights(pDevice, pDeviceContext, LightDesc);
}

HRESULT CGameInstance::Add_Fonts(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Fonts(pDevice, pDeviceContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Fonts(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor, _float fSize)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Fonts(pFontTag, pText, vPosition, vColor, fSize);
}

_bool CGameInstance::isInFrustum_WorldSpace(_vector vWorldPos, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}


ID3D11ShaderResourceView * CGameInstance::Get_RenderTargetSRV(const _tchar * pTargetTag) const
{
	if (nullptr == m_pTarget_Manager)
		return nullptr;

	return m_pTarget_Manager->Get_SRV(pTargetTag);
}

#ifdef _DEBUG
HRESULT CGameInstance::Init_Console_Debug(_bool bInit)
{
	if (nullptr == m_pConsole_Debug)
	{
		MSG_BOX(L"Failed To CGameInstance : Init_Console_Debug : nullptr == m_pConsole_Debug");
		return E_FAIL;
	}
	HRESULT hr = 0;
	if (true == bInit)
	{
		hr = m_pConsole_Debug->NativeConstruct();
	}
	else
	{
		hr = S_OK;
	}
	return hr;
}

_bool CGameInstance::Get_WireFrame()
{
	return g_bWireFrame;
}
#endif // _DEBUG
void CGameInstance::LoadDataFile_Effect()
{
	_ulong			dwByte = 0;
	//Monster
	HANDLE			hFile = CreateFile(L"../../Client/Bin/data/EffectData/Monster/effect.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vInstanceEffectDesc_Monster.push_back(vTemp);
	}
	CloseHandle(hFile);

				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Monster/effectMesh.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC_MESH vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC_MESH), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vMeshEffectDesc_Monster.push_back(vTemp);
	}
	CloseHandle(hFile);

	//Boss
				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Boss/effect.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vInstanceEffectDesc_Boss.push_back(vTemp);
	}
	CloseHandle(hFile);

				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Boss/effectMesh.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC_MESH vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC_MESH), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vMeshEffectDesc_Boss.push_back(vTemp);
	}
	CloseHandle(hFile);

	//Alphen
				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Alphen/effect.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vInstanceEffectDesc_Alphen.push_back(vTemp);
	}
	CloseHandle(hFile);

				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Alphen/effectMesh.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC_MESH vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC_MESH), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vMeshEffectDesc_Alphen.push_back(vTemp);
	}
	CloseHandle(hFile);
	//Shionne
				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Shionne/effect.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vInstanceEffectDesc_Shionne.push_back(vTemp);
	}
	CloseHandle(hFile);

				hFile = CreateFile(L"../../Client/Bin/data/EffectData/Shionne/effectMesh.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	while (true)
	{
		EFFECTDESC_MESH vTemp;
		ReadFile(hFile, &vTemp, sizeof(EFFECTDESC_MESH), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vMeshEffectDesc_Shionne.push_back(vTemp);
	}
	CloseHandle(hFile);

	
}
EFFECTDESC * CGameInstance::Get_InstanceEffect_Data(UNIT_TYPE type, _int index)
{
	switch (type)
	{
	case Engine::UNIT_MONSTER:
	{
		if (index < vInstanceEffectDesc_Monster.size())
		{
			return &vInstanceEffectDesc_Monster[index];
			break;
		}
	}
		
	case Engine::UNIT_BOSS:
	{
		if (index < vInstanceEffectDesc_Boss.size())
		{
			return &vInstanceEffectDesc_Boss[index];
			break;
		}
	}
		
	case Engine::UNIT_ALPHEN:
	{
		if (index < vInstanceEffectDesc_Alphen.size())
		{
			return &vInstanceEffectDesc_Alphen[index];
			break;
		}
	}
		
	case Engine::UNIT_SHIONNE:
	{
		if (index < vInstanceEffectDesc_Shionne.size())
		{
			return &vInstanceEffectDesc_Shionne[index];
			break;
		}
	}
		
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
EFFECTDESC_MESH * CGameInstance::Get_MeshEffect_Data(UNIT_TYPE type, _int index)
{
	switch (type)
	{
	case Engine::UNIT_MONSTER:
		if (index < vMeshEffectDesc_Monster.size())
		{
			return &vMeshEffectDesc_Monster[index];
			break;
		}
	case Engine::UNIT_BOSS:
		if (index < vMeshEffectDesc_Boss.size())
		{
			return &vMeshEffectDesc_Boss[index];
			break;
		}
	case Engine::UNIT_ALPHEN:
		if (index < vMeshEffectDesc_Alphen.size())
		{
			return &vMeshEffectDesc_Alphen[index];
			break;
		}
	case Engine::UNIT_SHIONNE:
		if (index < vMeshEffectDesc_Shionne.size())
		{
			return &vMeshEffectDesc_Shionne[index];
			break;
		}
	default:
		return nullptr;
		break;
	}
	return nullptr;
}


void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();

#ifdef _DEBUG
	CConsole_Debug::GetInstance()->DestroyInstance();
#endif // _DEBUG

	CPicking_Manager::GetInstance()->DestroyInstance();

	CPhysX::GetInstance()->DestroyInstance();

	CPostProcessing::GetInstance()->DestroyInstance();

	CLevel_Manager::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CTarget_Manager::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CPipeLine::GetInstance()->DestroyInstance();

	CLight_Manager::GetInstance()->DestroyInstance();

	CFont_Manager::GetInstance()->DestroyInstance();

	CFrustum::GetInstance()->DestroyInstance();

	CGraphic_Device::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pConsole_Debug);
#endif // _DEBUG

	Safe_Release(m_pPhysX);
	Safe_Release(m_pPostProcessing);
	Safe_Release(m_pPicking_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
