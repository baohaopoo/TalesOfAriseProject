#include "stdafx.h"
#include "..\Public\Level_Lobby.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Lobby::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
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
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로비입니다."));
	return S_OK;
}

//HRESULT CLevel_Lobby::Ready_Layer_BackGround(const _tchar * pLayerTag)
//{
//	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
//
//	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_BackGround")))
//		return E_FAIL;
//
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}


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