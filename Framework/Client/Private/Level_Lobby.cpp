#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include"..\Public\UIBackground.h"
#include"..\Public\LobbyBtn.h"
CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Lobby::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_LobbyBackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_StartBtnEffect(TEXT("Layer_StartBtnEffect"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_ExitBtnEffect(TEXT("Layer_ExitBtnEffect"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_Lobby::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance)
		return;

	RECT rc = { 317,633,409,666 };
	POINT pt = Get_Mouse(g_hWnd);

	if (PtInRect(&rc, pt) && GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		//pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_MapTool));	// -> 맵툴 이동용

		pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_TUTORIAL));	// -> 인게임 이동
	}


	Safe_Release(pGameInstance);
}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	wsprintf(UIMsg, TEXT("START"));
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), UIMsg, _float2(322, g_iWinCY / 2 + 100), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	wsprintf(ExitMsg, TEXT("EXIT"));
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), ExitMsg, _float2(332, g_iWinCY / 2 + 150), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	Safe_Release(pGameInstance);
	SetWindowText(g_hWnd, TEXT("로비입니다."));
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUIBackground::UIBACKDESC uidesc;
	uidesc.kind = 1;
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_BackGroundUI"), &uidesc))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_StartBtnEffect(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance)
		return E_FAIL;

	LobbyBtn::UIINFO uidesc;



	uidesc.fX = 370;
	uidesc.fY = 650;
	uidesc.fCX = 100;
	uidesc.fCY = 60;


	StartBtnEffect = pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, L"Prototype_GameObject_LobbyBtnUI", &uidesc);
	if (nullptr == StartBtnEffect)
	{
		MSG_BOX(L"시작오브젝트 effect 생성 오류");
		return  S_OK;
	}


	dynamic_cast<LobbyBtn*>(StartBtnEffect)->Set_Kind(0);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_ExitBtnEffect(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance)
		return E_FAIL;

	LobbyBtn::UIINFO uidesc;
	uidesc.fX = 370;
	uidesc.fY = 701;
	uidesc.fCX = 100;
	uidesc.fCY = 60;


	ExitBtnEffect = pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, L"Prototype_GameObject_LobbyBtnUI", &uidesc);
	if (nullptr == ExitBtnEffect)
	{
		MSG_BOX(L"시작오브젝트 effect 생성 오류");
		return  S_OK;
	}


	dynamic_cast<LobbyBtn*>(ExitBtnEffect)->Set_Kind(1);

	Safe_Release(pGameInstance);
	return S_OK;


}



CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Lobby*	pInstance = new CLevel_Lobby(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Lobby"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();
}