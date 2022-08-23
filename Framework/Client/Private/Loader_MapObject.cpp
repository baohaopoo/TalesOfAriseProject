#include "stdafx.h"
#include "..\Public\Loader_MapObject.h"
#include "GameInstance.h"

CLoader_MapObject::CLoader_MapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_MapObject::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_MapObject::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_MapObject::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_MapObject::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc_MapObject(void* pArg)
{
	CLoader_MapObject* pLoader = (CLoader_MapObject*)pArg;
	EnterCriticalSection(&pLoader->Get_CriticalSection());
	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_LOBBY:
		pLoader->Loading_ForLobbyLevel();
		break;

	case LEVEL_TUTORIAL:
		pLoader->Loading_ForTutorialLevel();
		break;

	case LEVEL_BOSS1:
		pLoader->Loading_ForBoss1Level();
		break;

	case LEVEL_BOSS2:
		pLoader->Loading_ForBoss2Level();
		break;
	}
	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	return 0;
}

HRESULT CLoader_MapObject::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_MapObject, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	lstrcpy(m_szLoading, L"Loader_MapObject : Model");
	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Texture");
	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Object");
	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Tutorial_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, L"Loader_MapObject : Prototype_Component_Model_Map_GrandYork_Battle_02");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_Battle02.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Prototype_Component_Model_Map_GrandYork_Battle_02_NaviMap");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_Battle02_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Prototype_Component_Model_Map_GrandYork_Battle_04");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_Battle04.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Prototype_Component_Model_Map_GrandYork_Battle_04_NaviMap");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_Battle04_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Tutorial_Texture()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Tutorial_Object()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_ForBoss1Level()
{
	m_isFinished = false;

	lstrcpy(m_szLoading, L"Loader_MapObject : Model");
	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Texture");
	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Object");
	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss1_Model()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_ForBoss2Level()
{
	m_isFinished = false;

	lstrcpy(m_szLoading, L"Loader_MapObject : Model");
	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Texture");
	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Object");
	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_MapObject : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss2_Model()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_MapObject::Loading_Boss2_Object()
{
	return S_OK;
}

CLoader_MapObject* CLoader_MapObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader_MapObject*	pInstance = new CLoader_MapObject(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader_MapObject : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_MapObject::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}