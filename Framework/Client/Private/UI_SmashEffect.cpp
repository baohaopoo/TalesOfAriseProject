#include "stdafx.h"
#include"UI_SmashEffect.h"

CUI_SmashEffect::CUI_SmashEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_SmashEffect::CUI_SmashEffect(const CUI_SmashEffect& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_SmashEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_SmashEffect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_tUIInfo.fCX = 2;
	m_tUIInfo.fCY = 2;
	
	m_iShaderPass = 3;
	return S_OK;				
}

void CUI_SmashEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	

	if (m_tUIInfo.fCX < 300) {

		if (rgb < 0)
			m_bDead = true;

		rgb -= 0.05f;


		m_tUIInfo.fCX += 16.f;
		m_tUIInfo.fCY += 22.f;

		
	
	}
	else
	{
		
		m_bDead = true;
		
	}
	
	
}

void CUI_SmashEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (rgb <0)
	{
		m_bDead = 1;	
	}
}

HRESULT CUI_SmashEffect::Render()
{
	if (!m_isBattleOn) {
		if (FAILED(m_pShaderCom->Set_RawValue("smasheffectrgb", &rgb, sizeof(float))))
			return E_FAIL;

		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CUI_SmashEffect::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture",LEVEL_STATIC, L"Prototype_Component_Texture_SmashEffect", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To CUI_SmashEffect : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUI_SmashEffect * CUI_SmashEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_SmashEffect* pInstance = new CUI_SmashEffect(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUI_SmashEffect : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_SmashEffect::Clone(void * pArg)
{
	CUI_SmashEffect* pInstance = new CUI_SmashEffect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI_SmashEffect : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SmashEffect::Free()
{
	__super::Free();


}
