#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel * pLevel)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if(nullptr != m_pCurrentLevel)
	{
		if (FAILED(pGameInstance->Clear_LevelResource(m_iLevelIndex)))
			return E_FAIL;
	}

	Safe_Release(m_pCurrentLevel);
	
	m_pCurrentLevel = pLevel;

	m_iLevelIndex = iLevelIndex;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(TimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
