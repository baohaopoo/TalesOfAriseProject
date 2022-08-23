#include "stdafx.h"
#include "..\Public\Level_Loading.h"


#include "GameInstance.h"

#include "Level_Lobby.h"
#include "Level_Tutorial.h"

#include "Loader_etc.h"
#include "Loader_Map.h"
#include "Loader_Effect.h"
#include "Loader_Player.h"
#include "Loader_Monster.h"
#include "Loader_MapObject.h"

#include"..\Public\UIBackground.h"
#include"..\Public\LineSprite.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader_etc = CLoader_etc::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	m_pLoader_Effect = CLoader_Effect::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	m_pLoader_Map = CLoader_Map::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	m_pLoader_Player = CLoader_Player::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	m_pLoader_Monster = CLoader_Monster::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	m_pLoader_MapObject = CLoader_MapObject::Create(m_pDevice, m_pDeviceContext, eNextLevel);

	if (nullptr == m_pLoader_etc || nullptr == m_pLoader_Map ||
		nullptr == m_pLoader_Effect || nullptr == m_pLoader_Player ||
		nullptr == m_pLoader_Monster || nullptr == m_pLoader_MapObject)
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_LoadingBackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	numtime += TimeDelta;
	if (num > 3)
	{
		num = 0;
	}
	if (numtime > 0.4)
	{
		num++;
		numtime = 0;
		//num = 0;
	}


	if (true == m_pLoader_etc->Get_Finished() && true == m_pLoader_Map->Get_Finished() &&
		true == m_pLoader_Effect->Get_Finished() && true == m_pLoader_Player->Get_Finished() &&
		true == m_pLoader_Monster->Get_Finished() && true == m_pLoader_MapObject->Get_Finished())
	{

		CLevel*		pLevel = nullptr;


		//if (GetKeyState(VK_SPACE) & 0x8000)
		{
			switch (m_eNextLevel)
			{
			case LEVEL_LOBBY:
				pLevel = CLevel_Lobby::Create(m_pDevice, m_pDeviceContext);
				break;

			case LEVEL_TUTORIAL:
				pLevel = CLevel_Tutorial::Create(m_pDevice, m_pDeviceContext);
				break;
			}
			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				goto Finish;
		}
	}
Finish:
	Safe_Release(pGameInstance);
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, L"Level_Loading");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (num == 0) {
		wsprintf(LoadingMsg, TEXT("Now Loading"));
	}if (num == 1)
	{
		wsprintf(LoadingMsg, TEXT("Now Loading."));
	}
	if (num == 2)
	{
		wsprintf(LoadingMsg, TEXT("Now Loading.."));
	}
	if (num == 3)
	{
		wsprintf(LoadingMsg, TEXT("Now Loading..."));
	}
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), LoadingMsg, _float2(1576, 993), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	_tchar szLoader[MAX_PATH] = L"";

	lstrcpy(szLoader, m_pLoader_etc->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 230.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	lstrcpy(szLoader, m_pLoader_Map->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 200.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	lstrcpy(szLoader, m_pLoader_Effect->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 170.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	lstrcpy(szLoader, m_pLoader_Player->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 140.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	lstrcpy(szLoader, m_pLoader_Monster->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 110.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	lstrcpy(szLoader, m_pLoader_MapObject->Get_LoadingText());
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan20"), szLoader, _float2(0.f, _float(g_iWinCY - 80.f)), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUIBackground::UIBACKDESC uidesc;
	uidesc.kind = 2;
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_BackGroundUI"), &uidesc))
		return E_FAIL;

	////LineSprite
	LineSprite::MINIMAPDESC LineSpritedesc;
	LineSpritedesc.uidesc.fX = 1555;
	LineSpritedesc.uidesc.fY = 1006;
	LineSpritedesc.uidesc.fCX = 30;
	LineSpritedesc.uidesc.fCY = 30;
	LineSpritedesc.kind = 26;
	LineSpritedesc.level = 2;//

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, L"Layer_Loading", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
		return E_FAIL;

	//ZeroMemory(&LineSpritedesc, sizeof(LineSprite::MINIMAPDESC));
	//LineSpritedesc.uidesc.fX = 1157;
	//LineSpritedesc.uidesc.fY = 622;
	//LineSpritedesc.uidesc.fCX = 10;
	//LineSpritedesc.uidesc.fCY = 10;
	//LineSpritedesc.kind = 27;
	//LineSpritedesc.level = 3;//

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, L"Layer_Loading", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDeviceOut, pDeviceContextOut);
	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Loading"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader_etc);
	Safe_Release(m_pLoader_Map);
	Safe_Release(m_pLoader_MapObject);
	Safe_Release(m_pLoader_Player);
	Safe_Release(m_pLoader_Monster);
	Safe_Release(m_pLoader_Effect);
}