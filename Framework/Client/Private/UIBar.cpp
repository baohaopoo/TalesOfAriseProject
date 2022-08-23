#include "stdafx.h"
#include "UIBar.h"
#include"UI_Manager.h"
#include"Player_Manager.h"
#include"Player.h"
#include"Alphen.h"
//ÀüÅõ
UIBar::UIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

UIBar::UIBar(const UIBar& rhs)
	: CUI(rhs)
{
}

HRESULT UIBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_fDistanceToCamera = 40;
	m_iShaderPass = 22;




	return S_OK;
}

void UIBar::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	CUI_Manager* pUiManager = CUI_Manager::GetInstance();
	CPlayer_Manager* pPlayerManager = CPlayer_Manager::GetInstance();

	Safe_AddRef(pUiManager);
	Safe_AddRef(pPlayerManager);

	CPlayer* player = pUiManager->GetPlayer(pPlayerManager->Get_MainPlayerIndex());
	if (player == nullptr) return;

	if (pPlayerManager->Get_MainPlayerIndex() == 0)
	{
		hp = dynamic_cast<CAlphen*>(player)->Get_PlayerInfo().m_iCurrentHp;
		percent = hp / dynamic_cast<CAlphen*>(player)->Get_PlayerInfo().m_iMaxHp;
	}
	
	if (m_Redbar > percent)
	{
		m_Redbar -= TimeDelta;
	}
	if (m_Redbar < percent)
		m_Redbar = percent;

	Safe_Release(pUiManager);
	Safe_Release(pPlayerManager);

}

void UIBar::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT UIBar::Render()
{
	m_pShaderCom->Set_RawValue("lefthpPercent", &percent, sizeof(_float));
	m_pShaderCom->Set_RawValue("g_leftRedBar", &m_Redbar, sizeof(_float));

	//if (FAILED(m_pShaderCom->Set_RawValue("exuirgb", &rgb, sizeof(float))))
	//	return E_FAIL;

	if (!m_isBattleOn) {




		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT UIBar::SetUp_Texture()
{
	//Prototype_Component_Texture_HPBar

	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_HPBar", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UIBar : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIBar * UIBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIBar* pInstance = new UIBar(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIBar : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIBar::Clone(void * pArg)
{
	UIBar* pInstance = new UIBar(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIBar : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIBar::Free()
{
	__super::Free();

}
