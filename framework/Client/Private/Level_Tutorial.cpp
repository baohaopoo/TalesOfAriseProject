#include "stdafx.h"
#include "..\Public\Level_Tutorial.h"

#include "Sword.h"
#include "GameInstance.h"
#include "Camera_Default.h"


CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Tutorial::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;	

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))			// sword 주석처리로 안나오게 막아놓음
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	

	

	return S_OK;
}

void CLevel_Tutorial::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

HRESULT CLevel_Tutorial::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("튜토리얼 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 4.f, 10.f, 1.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);	
	LightDesc.vSpecular = _float4(1.f, 0.0f, 0.0f, 1.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;	

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(15.f, 4.f, 10.f, 1.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 1.f, 0.0f, 1.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Player")))
		return E_FAIL;

	CSword::SOCKETDESC			SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CSword::SOCKETDESC));
	
	SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_TUTORIAL, pLayerTag, TEXT("Com_Model"));
	SocketDesc.pBoneName = "SWORD";

	//if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sword"), &SocketDesc))
	//	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Terrain")))
		return E_FAIL;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sky")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Rect_Effect")))
		return E_FAIL;


	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Point_Effect")))
		return E_FAIL;

	for (_uint i = 0; i < 20; ++i)
	{
		if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Effect")))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;


	
}

HRESULT CLevel_Tutorial::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UI")))
		return E_FAIL;
	

	Safe_Release(pGameInstance);


	return S_OK;
}

CLevel_Tutorial * CLevel_Tutorial::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Tutorial*	pInstance = new CLevel_Tutorial(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Tutorial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tutorial::Free()
{
	__super::Free();


}
