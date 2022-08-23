#include "stdafx.h"
#include"UIFlag.h"

UIFlag::UIFlag(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIFlag::UIFlag(const UIFlag& rhs)
	:CUI(rhs)
{
}

HRESULT UIFlag::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIFlag::NativeConstruct(void * pArg)
{

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	kind = ((UIFLAGDESC*)pArg)->kind;
	m_fDistanceToCamera = 400;
	m_iShaderPass = 3;


	if (kind == 0)
	{
		m_tUIInfo.fX = 230;
		m_tUIInfo.fY = 700;
		m_tUIInfo.fCX = 70;
		m_tUIInfo.fCY = 70;
	}
	else if (kind == 1)
	{
		m_tUIInfo.fX = 170;
		m_tUIInfo.fY = 800;
		m_tUIInfo.fCX = 70;
		m_tUIInfo.fCY = 70;
	}
	return S_OK;
}

void UIFlag::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	
	if (kind == 0)
	{
		m_tUIInfo.fX = 230;
		m_tUIInfo.fY = 700;
		m_tUIInfo.fCX = 70;
		m_tUIInfo.fCY = 70;
	}
	else if (kind == 1)
	{
		m_tUIInfo.fX = 170;
		m_tUIInfo.fY = 760;
		m_tUIInfo.fCX = 70;
		m_tUIInfo.fCY = 70;
	}
	Safe_Release(pGameInstnace);

}

void UIFlag::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIFlag::Render()
{

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIFlag::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Flag", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UINumber : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIFlag * UIFlag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIFlag* pInstance = new UIFlag(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINumber : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIFlag::Clone(void * pArg)
{
	UIFlag* pInstance = new UIFlag(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UINumber : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIFlag::Free()
{
	__super::Free();

}
