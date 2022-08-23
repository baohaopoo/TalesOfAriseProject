#include "stdafx.h"
#include "..\Public\Loader_etc.h"

#include "GameInstance.h"
#include "Camera_Default.h"
#include "LightDepth.h"

#include "Navigation.h"
#include "Sky.h"

#include "Rect_Effect.h"
#include "Point_Effect.h"
#include "MeshEffect.h"
#include "Effect.h"

CLoader_etc::CLoader_etc(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: m_pDevice(pDeviceOut), m_pDeviceContext(pDeviceContextOut)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CRITICAL_SECTION CLoader_etc::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader_etc::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader_etc::Get_Finished() const
{
	return m_isFinished;
}

const _tchar* CLoader_etc::Get_LoadingText() const
{
	return m_szLoading;
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader_etc*		pLoader = (CLoader_etc*)pArg;

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

HRESULT CLoader_etc::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_etc::Loading_ForLobbyLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Lobby_Model()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Object()))
		return E_FAIL;

	if (FAILED(Loading_Lobby_Components()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_etc::Loading_Lobby_Model()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Lobby_Texture()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Lobby_Object()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Lobby_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region Collider
	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Collider_AABB");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Collider_OBB");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Collider_SPHERE");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;
#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_etc::Loading_ForTutorialLevel()
{
	m_isFinished = false;

	if (FAILED(Loading_Tutorial_Model()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Object()))
		return E_FAIL;

	if (FAILED(Loading_Tutorial_Components()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_etc::Loading_Tutorial_Model()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Tutorial_Texture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Texture_SkyBox");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/space.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Texture_Effect");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Effect(%d).png"), 29))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Texture_Effect_Mesh");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Mesh/Mesh(%d).png"), 421))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader_etc::Loading_Tutorial_Object()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Camera_Default");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_LightDepth");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LightDepth"),
		CLightDepth::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Rect_Effect");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rect_Effect"),
		CRect_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Point_Effect");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Point_Effect"),
		CPoint_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Mesh_Effect");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mesh_Effect"),
		CMeshEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Sky");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_GameObject_Effect");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader_etc::Loading_Tutorial_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region Shader
	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxNorTex");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxAnim");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxInstance");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxEffect");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffect"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxEffect.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxCube");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE_DECLARATION::Elements, VTXCUBE_DECLARATION::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Shader_VtxNonAnim");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl"), VTXNONANIMMODEL_DECLARATION::Elements, VTXNONANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region VIBuffer
	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_VIBuffer_Terrain");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_VIBuffer_RectInstance");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 10))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_VIBuffer_PointInstance");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 50))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_VIBuffer_Cube");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

#pragma region Navigation
	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Navigation_Map_Tutorial");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_Tutorial"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage01/CellList.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Navigation_Map_GrandYork_Battle_02");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_02"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage08/CellList.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Navigation_Map_GrandYork_Battle_04");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_04"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage09/CellList.dat")))))
		return E_FAIL;
#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_etc::Loading_ForBoss1Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss1_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Object()))
		return E_FAIL;

	if (FAILED(Loading_Boss1_Components()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss1_Model()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss1_Texture()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss1_Object()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss1_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Navigation_Map_Balseph");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_Balseph"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage03/CellList.dat")))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader_etc::Loading_ForBoss2Level()
{
	m_isFinished = false;

	if (FAILED(Loading_Boss2_Model()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Texture()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Object()))
		return E_FAIL;

	if (FAILED(Loading_Boss2_Components()))
		return E_FAIL;

	lstrcpy(m_szLoading, L"Loader_etc : Finish");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss2_Model()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss2_Texture()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss2_Object()
{
	return S_OK;
}

HRESULT CLoader_etc::Loading_Boss2_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, L"Loader_etc : Prototype_Component_Navigation_Map_FireAvatar");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_FireAvatar"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Dat/Stage04/CellList.dat")))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader_etc* CLoader_etc::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel)
{
	CLoader_etc* pInstance = new CLoader_etc(pDeviceOut, pDeviceContextOut);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader_etc"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader_etc::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}