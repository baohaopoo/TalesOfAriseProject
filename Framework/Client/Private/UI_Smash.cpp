#include "stdafx.h"
#include "..\Public\UI_Smash.h"
#include"UI_SmashEffect.h"
CUI_Smash::CUI_Smash(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Smash::CUI_Smash(const CUI_Smash& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Smash::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Smash::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	x = ((UISMDESC*)pArg)->tUIInfo.fX;
	y = ((UISMDESC*)pArg)->tUIInfo.fY;

	
	m_iSprite = ((UISMDESC*)pArg)->kind;

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_iShaderPass = 3;
	return S_OK;
}

void CUI_Smash::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);



}
void CUI_Smash::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUI_Smash::Render()
{


	if (!m_isBattleOn) {
		if (FAILED(__super::Render()))
		{
			MSG_BOX(L"여기다 임마");
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CUI_Smash::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_SmashUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To CUI_Smash : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUI_Smash * CUI_Smash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Smash* pInstance = new CUI_Smash(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUI_Smash : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Smash::Clone(void * pArg)
{
	CUI_Smash* pInstance = new CUI_Smash(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI_Smash : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Smash::Free()
{
	__super::Free();

}
