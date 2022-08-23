#include "stdafx.h"
#include "..\Public\Loader_Player.h"
#include "GameInstance.h"

#include "Alphen.h"
#include "Shionne.h"


#include "Normal_Bullet.h"
#include "Lunar_Blast.h"
#include "Annihilation.h"
#include "Displode.h"
#include "Spear_Sweep.h"
#include "Gravitas_Field.h"
#include "Explosion.h"

#include "Sword.h"
#include "Gun.h"

CLoader_Player::CLoader_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_Player::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_Player::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_Player::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_Player::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc_Player(void* pArg)
{
	CLoader_Player* pLoader = (CLoader_Player*)pArg;
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

HRESULT CLoader_Player::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_Player, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Player::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Player::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Player::Loading_Tutorial_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix PivotMatrix, PivotAxeMatrix;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Body");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Body"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Body/", "Ari_Body.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Face");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Face"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Face/", "Ari_Face.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Hair");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Hair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/Hair/", "Ari_Hair.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_SKL");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_SKL"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ari/SKL/", "Ari_SKL.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(180.f));

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Sword_26");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_26"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_26/", "Ari_Sword_26.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Sword_28");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_28"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_28/", "Ari_Sword_28.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Ari_Sword_100");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Sword_100"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ari/Sword/Ari_Sword_100/", "Ari_Sword_100.fbx", PivotMatrix))))
		return E_FAIL;

	//////////////////////////////////////////////////////Sio/////////////////////////////////////////////////////////////////

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Sio_Body");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Body"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Body/", "Sio_Body.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Sio_Face");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Face"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Face/", "Sio_Face.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Sio_Hair");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Hair"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/Hair/", "Sio_Hair.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Sio_SKL");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_SKL"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sio/SKL/", "Sio_SKL.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(180.f));
	
	lstrcpy(m_szLoading, L"Loader_Player : Prototype_Component_Model_Sio_Gun_0");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sio_Gun_0"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Sio/Gun/Sio_Gun_0/", "Sio_Gun_0.fbx", PivotMatrix))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Player::Loading_Tutorial_Texture()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Tutorial_Object()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_GameObject_Alphen");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alphen"),
		CAlphen::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_GameObject_Shionne");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shionne"),
		CShionne::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_GameObject_Sword");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Prototype_GameObject_Gun");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun"),
		CGun::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	//////////////////////////////////////////////////////Bullet////////////////////////////////////////////////////////////////////

	/* For.Prototype_GameObject_Normal_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Normal_Bullet"),
		CNormal_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Lunar_Blast */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Lunar_Blast"),
		CLunar_Blast::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Annihilation */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Annihilation"),
		CAnnihilation::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Explosion */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Explosion"),
		CExplosion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Displode */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Displode"),
		CDisplode::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Gravitas_Field */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gravitas_Field"),
		CGravitas_Field::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Displode */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spear_Sweep"),
		CSpear_Sweep::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_Player::Loading_ForBoss1Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss1_Model()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_ForBoss2Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Player : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss2_Model()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_Player::Loading_Boss2_Object()
{
	return S_OK;
}

CLoader_Player* CLoader_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader_Player*	pInstance = new CLoader_Player(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader_Player : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_Player::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}