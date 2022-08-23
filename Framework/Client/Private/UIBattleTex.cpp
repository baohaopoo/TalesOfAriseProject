#include "stdafx.h"
#include "UIBattleTex.h"

//ÀüÅõ
UIBattleTex::UIBattleTex(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIBattleTex::UIBattleTex(const UIBattleTex& rhs)
	:CUI(rhs)
{
}

HRESULT UIBattleTex::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIBattleTex::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	UIKind = ((UIBATTLETEX*)pArg)->kind;

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_iShaderPass = 21;

	return S_OK;
}

void UIBattleTex::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();


	if (!m_isFirst) {
		time += TimeDelta;
		if (time > 4.5)
		{
			m_isRender = true;
			m_isFirst = true;
		}
	}
	

}

void UIBattleTex::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIBattleTex::Render()
{
	if (m_isRender) {


		if (FAILED(m_pShader->Set_RawValue("battletexrgb", &rgb, sizeof(float))))
			return E_FAIL;


	
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT UIBattleTex::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_BattleTex", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To UIBattleTex : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIBattleTex * UIBattleTex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIBattleTex* pInstance = new UIBattleTex(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIBattleTex : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIBattleTex::Clone(void * pArg)
{
	UIBattleTex* pInstance = new UIBattleTex(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIBattleTex : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIBattleTex::Free()
{
}
