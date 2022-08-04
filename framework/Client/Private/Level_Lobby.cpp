#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include"BackGround.h"
CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Lobby::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	if (FAILED(Ready_Fonts()))
		return E_FAIL;
	return S_OK;

}

void CLevel_Lobby::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_TUTORIAL))))
			return;
	}

	Safe_Release(pGameInstance);
}

HRESULT CLevel_Lobby::Render()
{


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로비입니다."));


	wsprintf(UIMsg, TEXT("START"));
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), UIMsg, _float2(233, 800), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	wsprintf(ExitMsg, TEXT("EXIT"));
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), ExitMsg, _float2(240, 860), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	CBackGround::BACKDESC backdesc;
	backdesc.kind = 1;

	// 스프라이트 넣자.
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_BackGround"),&backdesc))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Fonts()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance)
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan20"), TEXT("../Bin/Resources/Fonts/Javanese20.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}

	pGameInstance->Release();
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