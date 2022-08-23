#include "stdafx.h"
#include "UIEmblem.h"

//ÀüÅõ
UIEmblem::UIEmblem(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIEmblem::UIEmblem(const UIEmblem& rhs)
	:CUI(rhs)
{
}

HRESULT UIEmblem::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIEmblem::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_iShaderPass = 15;
	return S_OK;
}

void UIEmblem::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	time += TimeDelta;

	if (time > 2)
	{
		m_isRenderOff = true;
	}

	if (m_isRenderOff)
	{
		if (rgb <= 0) rgb = 0;
		rgb -= 0.008;
	}
	else
	{
		
		if (!m_isFirst)
			rgb += 0.008f;

		if (rgb > 0.8) {
			rgb = 0.8;
			m_isFirst = true;

		}
	}



}

void UIEmblem::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIEmblem::Render()
{

	if (FAILED(m_pShader->Set_RawValue("emblemrgb", &rgb, sizeof(float))))
		return E_FAIL;




	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIEmblem::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Emblem", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To UIEmblem : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIEmblem * UIEmblem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIEmblem* pInstance = new UIEmblem(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIEmblem : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIEmblem::Clone(void * pArg)
{
	UIEmblem* pInstance = new UIEmblem(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIEmblem : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIEmblem::Free()
{
}
