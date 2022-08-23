#include "stdafx.h"
#include "UIExplain.h"
#include"UIBattleTex.h"
//ÀüÅõ
UIExplain::UIExplain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIExplain::UIExplain(const UIExplain& rhs)
	:CUI(rhs)
{
}

HRESULT UIExplain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIExplain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	m_iSprite = ((UIEXDESC*)pArg)->kind;
	//m_iShaderPass = 6;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	return S_OK;
}

void UIExplain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	 if(m_iSprite == 1){

		//µÞ¹è°æ
		m_iShaderPass = 6;
		time += (_float)TimeDelta;

		if (time > 4)
		{
			if (m_tUIInfo.fCX > 434)
				m_tUIInfo.fCX = 434;
			m_tUIInfo.fCX += 10;
			rgb = 0.5;
		}

	 }
	 else if (m_iSprite == 0)
	 {
		 //battletex
		 if (time > 4)
		 {
			 if (m_tUIInfo.fCX > 434)
				 m_tUIInfo.fCX = 434;
				m_tUIInfo.fCX += 10;
				 rgb = 0.5;
		 }
		 m_iShaderPass = 6;

	 }



	if (pGameInstnace->Key_Down(DIK_TAB))
	{
		m_bDead = true;
	}
	Safe_Release(pGameInstnace);
}

void UIExplain::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIExplain::Render()
{
	/*if (UIKind == 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("blrgb", &rgb, sizeof(float))))
			return E_FAIL;
		
	}
	else {
		if (FAILED(m_pShaderCom->Set_RawValue("exuirgb", &rgb, sizeof(float))))
			return E_FAIL;
	}
*/


	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIExplain::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_ExUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UIExplain : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIExplain * UIExplain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIExplain* pInstance = new UIExplain(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIExplain : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIExplain::Clone(void * pArg)
{
	UIExplain* pInstance = new UIExplain(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIExplain : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIExplain::Free()
{
	__super::Free();

}
