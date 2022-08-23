#include "stdafx.h"
#include "UILine.h"

//ÀüÅõ
UILine::UILine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UILine::UILine(const UILine& rhs)
	: CUI(rhs)
{
}

HRESULT UILine::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UILine::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	m_iSprite = ((LINEDESC*)pArg)->kind;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_fDistanceToCamera = 100;

	return S_OK;
}

void UILine::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	if (m_iSprite == 0)
	{
		//Battle
		m_iShaderPass = 8;
	}
	if (m_iSprite == 1)
	{	//TutoLine
		m_iShaderPass = 3;

		m_tUIInfo.fX += 70; //¿·À¸·Î ÂÓ¹Ð°í ... 


		if (m_tUIInfo.fX > 5000)
			m_bDead = true;
	}


}

void UILine::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UILine::Render()
{

	if (!m_isBattleOn)
	{


		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT UILine::SetUp_Texture()
{
	//Prototype_Component_Texture_HPBar

	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Line", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UIBar : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UILine * UILine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UILine* pInstance = new UILine(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UILine : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UILine::Clone(void * pArg)
{
	UILine* pInstance = new UILine(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UILine : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UILine::Free()
{
	__super::Free();

}
