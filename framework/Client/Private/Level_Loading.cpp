#include "stdafx.h"
#include "..\Public\Level_Loading.h"


#include "GameInstance.h"
#include "Level_Lobby.h"
#include "Level_Tutorial.h"
#include "Loader.h"


CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (true == m_pLoader->Get_Finished())
	{
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			CLevel*		pLevel = nullptr;

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

	_tchar		szText[MAX_PATH] = TEXT("");

	lstrcpy(szText, TEXT("로딩 레벨입니다 : "));
	lstrcat(szText, m_pLoader->Get_LoadingText());

	SetWindowText(g_hWnd, szText);

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

	Safe_Release(m_pLoader);


}
