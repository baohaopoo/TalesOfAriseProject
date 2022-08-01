#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"

CCamera_Default::CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CCamera(pDeviceOut, pDeviceContextOut)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 10.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_BackWard(TimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta);
	}

	_long		MouseMove;

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove* 0.1f);
	}	

	__super::Tick(TimeDelta);

	Safe_Release(pGameInstance);
}

void CCamera_Default::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Default * CCamera_Default::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CCamera_Default*	pInstance = new CCamera_Default(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
