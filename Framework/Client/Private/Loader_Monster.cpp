#include "stdafx.h"
#include "..\Public\Loader_Monster.h"
#include "GameInstance.h"

#include "Fire_Avatar.h"
#include "FireAvatar_Rock.h"
#include "Lord_Balseph.h"
#include "BalsephAxe.h"
#include "Boar.h"
#include "Mantis.h"
#include "Punisher.h"
#include "Practice.h"

CLoader_Monster::CLoader_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_Monster::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_Monster::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_Monster::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_Monster::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc_Monster(void* pArg)
{
	CLoader_Monster* pLoader = (CLoader_Monster*)pArg;
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

HRESULT CLoader_Monster::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_Monster, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Monster::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Monster::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Finish");
	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Tutorial_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix PivotMatrix, PivotAxeMatrix;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_Balseph");
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Balseph"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "wow.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_BalsephFace");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephFace"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "balsephFace.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_BalsephSKL");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephSKL"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Baselph/", "balsephSKL.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_BalsephAXE");
	PivotAxeMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BalsephAXE"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Baselph/", "balsephAxe.fbx", PivotAxeMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_FireAvatar");
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_FireAvatar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/FireAvatar/", "fire_Avatar.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_Boar");
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Boar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Boar/", "boar.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_Mantis");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Mantis"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Mantis/", "mantis1.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_Punisher");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Punisher"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/punisher/", "punisher.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_Component_Model_EFRRock");
	PivotMatrix = XMMatrixScaling(1.5f, 1.5f, 1.5f) * XMMatrixRotationX(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_EFRRock"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FireAvatar/", "efrrock.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Monster::Loading_Tutorial_Texture()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Tutorial_Object()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_FireAvatar");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireAvatar"),
		CFire_Avatar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_Balseph");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Balseph"),
		CLord_Balseph::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_BalsephAxe");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BalsephAxe"),
		CBalsephAxe::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_Boar");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boar"),
		CBoar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_Practice");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Practice"),
		CPractice::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_Mantis");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mantis"),
		CMantis::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_Punisher");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Punisher"),
		CPunisher::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Prototype_GameObject_FireAvatar_Rock");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireAvatar_Rock"),
		CFireAvatar_Rock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Monster::Loading_ForBoss1Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Finish");
	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss1_Model()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_ForBoss2Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Monster : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss2_Model()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_Monster::Loading_Boss2_Object()
{
	return S_OK;
}

CLoader_Monster* CLoader_Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader_Monster*	pInstance = new CLoader_Monster(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader_Monster : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_Monster::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}