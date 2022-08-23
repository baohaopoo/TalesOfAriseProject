#include "stdafx.h"
#include "UIHpBar.h"
#include"UI_Manager.h"
#include"UIHpBarRed.h"
//ÀüÅõ
UIHpBar::UIHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CProgressUI(pDevice, pDeviceContext)
{
}

UIHpBar::UIHpBar(const UIHpBar& rhs)
	: CProgressUI(rhs)
{
}

HRESULT UIHpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT UIHpBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{

		return E_FAIL;
	}
	m_BarSize = ((UIHPBARDESC*)pArg)->kind;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_iShaderPass = 10;//3; // 0; / 20;
//	m_fDistanceToCamera = 80;


	return S_OK;
}

void UIHpBar::Tick(_double TimeDelta)
{


	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstnace);
	if (GetKeyState(VK_F8) & 0x8000)
	{
		Set_Dead(1);
	}



	////UI¿ë..
	//CUI_Manager* pGameUIInstance = CUI_Manager::GetInstance();
	//Safe_AddRef(pGameUIInstance);

	//int chp = (pGameUIInstance->GetPlayerInfo(CUI_Manager::UIPLAYER::PLAYER_ALPHEN)).iCurrentCp;
	//chp = 10;
	//chp = (pGameUIInstance->GetPlayerInfo(CUI_Manager::UIPLAYER::PLAYER_ALPHEN)).iCurrentHp;
	//chp;
	//Safe_Release(pGameUIInstance);
	if (pGameInstnace->Key_Down(DIK_L))
	{

		m_tUIInfo.fCX += damage;

		/*if (m_BarSize == 0)
		{

		if (m_tUIInfo.fX <= 1600)
		m_tUIInfo.fX = 1600;



		if (m_tUIInfo.fCX < 360) {
		m_tUIInfo.fCX -= 10;
		redgone = true;
		}
		}
		else
		{
		if (m_tUIInfo.fCX < 240) {
		m_tUIInfo.fCX -= 10;
		redgone = true;
		}


		}*/



	}
	else if (pGameInstnace->Key_Down(DIK_K)) {


		//m_tUIInfo.fCX -= damage;
		//UIHpBarRed::UIHPBARDESC uidesc;
		//uidesc.tUIInfo.fX = m_tUIInfo.fCX / 2;
		//uidesc.tUIInfo.fY = m_tUIInfo.fY;
		//uidesc.tUIInfo.fCX = 220;
		//uidesc.tUIInfo.fCY = 14;
		//uidesc.kind = 1;
		//if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_redbar", TEXT("Prototype_GameObject_HpREDUI"), &uidesc)))
		//	return;
		/*if (m_BarSize == 0)
		{
		if (m_tUIInfo.fX <= 1600)
		m_tUIInfo.fX = 1600;

		if (m_tUIInfo.fCX <360)
		m_tUIInfo.fCX += 10;
		}
		else
		{

		if (m_tUIInfo.fCX <240)
		m_tUIInfo.fCX += 10;
		}*/
	}



	__super::Tick(TimeDelta);
	pGameInstnace->Release();

}

void UIHpBar::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	rgb = 0.3f;
}

HRESULT UIHpBar::Render()
{
	if (!m_isBattleOn) {

		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT UIHpBar::SetUp_Texture()
{
	//Prototype_Component_Texture_HPBar

	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_GreenHP", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To UIHpBar : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIHpBar * UIHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIHpBar* pInstance = new UIHpBar(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIHpBar : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIHpBar::Clone(void * pArg)
{
	UIHpBar* pInstance = new UIHpBar(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIHpBar : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIHpBar::Free()
{
	__super::Free();

}
