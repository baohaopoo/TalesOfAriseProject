#include "stdafx.h"
#include "..\Public\Loader_Effect.h"
#include "GameInstance.h"

CLoader_Effect::CLoader_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_Effect::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_Effect::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_Effect::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_Effect::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc_Effect(void* pArg)
{
	CLoader_Effect* pLoader = (CLoader_Effect*)pArg;
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

HRESULT CLoader_Effect::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_Effect, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Effect::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Effect : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Effect::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Effect : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Effect::Loading_Tutorial_Model()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix PivotMatrix, PivotAxeMatrix;

	lstrcpy(m_szLoading, L"Loader_Effect : Prototype_Component_Model_Effect1");
	PivotMatrix = XMMatrixScaling(10.f, 10.f, 10.f)*XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Effect1",
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_EFFECT, "../Bin/Resources/Model/Effect/", "0.fbx", PivotMatrix))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_Effect::Loading_Tutorial_Texture()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Tutorial_Object()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_ForBoss1Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss1_Model()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_ForBoss2Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_Effect : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss2_Model()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_Effect::Loading_Boss2_Object()
{
	return S_OK;
}

CLoader_Effect* CLoader_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader_Effect*	pInstance = new CLoader_Effect(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader_Effect : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_Effect::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}