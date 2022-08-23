#include "stdafx.h"
#include "..\Public\UIName.h"
#include"UIGlow.h"
CUIName::CUIName(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUIName::CUIName(const CUIName& rhs)
	: CUI(rhs)
{
}

HRESULT CUIName::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIName::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	numKind = ((UINAMEDESC*)pArg)->kind;


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_iShaderPass = 3;
	m_iSprite = numKind;
	m_fDistanceToCamera = 89.3f;
	return S_OK;
}

void CUIName::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


}

void CUIName::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUIName::Render()
{

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIName::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_NameUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UINumber : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUIName * CUIName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIName* pInstance = new CUIName(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINumber : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIName::Clone(void * pArg)
{
	CUIName* pInstance = new CUIName(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UINumber : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIName::Free()
{
	__super::Free();
}
