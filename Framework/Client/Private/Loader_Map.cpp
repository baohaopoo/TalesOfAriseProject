#include "stdafx.h"
#include "..\Public\Loader_Map.h"
#include "GameInstance.h"

#include "MapObject.h"
#include "ImGUI_Manager.h"
#include "WayPoint.h"

CLoader_Map::CLoader_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_Map::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_Map::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_Map::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_Map::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc_Map(void* pArg)
{
	CLoader_Map* pLoader = (CLoader_Map*)pArg;
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

HRESULT CLoader_Map::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_Map, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Map::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Map::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Finish");
	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader_Map::Loading_Tutorial_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix	PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_GrandYork_HeightMapMesh");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Map_GrandYork_HeightMapMesh"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_HeightMapMesh.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_GrandYork_MapMesh");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Map_GrandYork_MapMesh"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_MapMesh.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_GrandYork_NaviMap");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Map_GrandYork_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_GrandYork_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Map::Loading_Tutorial_Texture()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_Tutorial_Object()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_GameObject_MapObject");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapObject"), CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Map::Loading_ForBoss1Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss1_Model()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix	PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_Boss_Balseph");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS1, TEXT("Prototype_Component_Model_Map_Boss_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_Boss_Balseph.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_Boss_Balseph_NaviMap");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS1, TEXT("Prototype_Component_Model_Map_Boss_Balseph_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_Boss_Balseph_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_ForBoss2Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss2_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix	PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_GrandYork_HeightMapMesh");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS2, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_Boss_FireAvatar.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Map : Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS2, TEXT("Prototype_Component_Model_Map_Boss_FireAvatar_NaviMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/UMap/Map_GrandYork/", "Map_Boss_FireAvatar_NaviMap.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_Map::Loading_Boss2_Object()
{
	return S_OK;
}

CLoader_Map* CLoader_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader_Map*	pInstance = new CLoader_Map(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader_Map : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_Map::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}