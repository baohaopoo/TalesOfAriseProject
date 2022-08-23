#include "stdafx.h"
#include "..\Public\FadeInOut.h"


CFadeInOut::CFadeInOut(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CFadeInOut::CFadeInOut(const CFadeInOut & rhs)
	: CUI(rhs)
{
}

HRESULT CFadeInOut::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
			return S_OK;
}

HRESULT CFadeInOut::NativeConstruct(void * pArg)
{

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_tUIInfo.fCX = 1280;
	m_tUIInfo.fCY = 720;
	m_tUIInfo.fX = 640;
	m_tUIInfo.fY = 360;
	m_iShaderPass = 10;

	return S_OK;
}

void CFadeInOut::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_isEnterRgb)
	{
		//UI_Inven버튼 누를때 특별주문.
		rgb += 0.008f;

		if (rgb >= 0.8) rgb = 0.8;
	}
	else
	{

		//디폴트
		rgb += 0.008f;

		if (rgb >= 0.6) rgb = 0.6;
	}

	//
	//else
	//{
	//	m_isEnterRgb = false;
	//	rgb -= 0.008f;
	//	if (rgb <= 0.6) rgb = 0.6;
	//}
}

void CFadeInOut::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CFadeInOut::Render()
{

	if (FAILED(m_pShader->Set_RawValue("alphargb", &rgb, sizeof(float))))
		return E_FAIL;



	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CFadeInOut::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Fade", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To CUI_Logo : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CFadeInOut * CFadeInOut::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CFadeInOut* pInstance = new CFadeInOut(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CFadeInOut : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFadeInOut::Clone(void * pArg)
{
	CFadeInOut* pInstance = new CFadeInOut(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CFadeInOut : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFadeInOut::Free()
{
}
