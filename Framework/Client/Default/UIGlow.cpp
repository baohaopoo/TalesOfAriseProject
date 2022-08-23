#include "stdafx.h"
#include "UIGlow.h"

//ÀüÅõ
UIGlow::UIGlow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIGlow::UIGlow(const UIGlow& rhs)
	: CUI(rhs)
{
}

HRESULT UIGlow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIGlow::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	glowkind = ((UIGLOWDESC*)pArg)->kind;


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_iShaderPass = 18;

	m_fDistanceToCamera = 89;
	return S_OK;
}

void UIGlow::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	if (glowkind == 0) {


		rgb = 0.8f;


		if (m_tUIInfo.fCX > 100) {

			m_tUIInfo.fCX = 90;
			m_tUIInfo.fCY = 90;
		}
		m_tUIInfo.fCX += 1;
		m_tUIInfo.fCY += 1;

	}
	if (glowkind == 1)
	{
		rgb = 0.8f;


		if (m_tUIInfo.fCX > 40) {

			m_tUIInfo.fCX = 30;
			m_tUIInfo.fCY = 30;
		}
		m_tUIInfo.fCX += 1;
		m_tUIInfo.fCY += 1;
	}

	Safe_Release(pGameInstnace);

}

void UIGlow::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIGlow::Render()
{


	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Set_RawValue("glowrgb", &rgb, sizeof(float))))
		return E_FAIL;



	return S_OK;
}

HRESULT UIGlow::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_GlowUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UIGlow : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIGlow * UIGlow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIGlow* pInstance = new UIGlow(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIGlow : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIGlow::Clone(void * pArg)
{
	UIGlow* pInstance = new UIGlow(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIGlow : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIGlow::Free()
{
	__super::Free();

}
