#include "stdafx.h"
#include "..\Public\LightDepth.h"

CLightDepth::CLightDepth(ID3D11Device* pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice, pDeviceContext)
{
}

CLightDepth::CLightDepth(const CLightDepth & rhs)
	:CCamera(rhs)
{
}

HRESULT CLightDepth::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLightDepth::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 10.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CLightDepth::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLightDepth::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CLightDepth::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CLightDepth * CLightDepth::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLightDepth*	pInstance = new CLightDepth(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CLightDepth"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLightDepth::Clone(void * pArg)
{
	CLightDepth*	pInstance = new CLightDepth(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CLightDepth"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightDepth::Free()
{
	__super::Free();
}
