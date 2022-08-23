#include "stdafx.h"
#include"UIHpBarRed.h"
//전투
UIHpBarRed::UIHpBarRed(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CProgressUI(pDevice, pDeviceContext)
{
}

UIHpBarRed::UIHpBarRed(const UIHpBarRed& rhs)
	: CProgressUI(rhs)
{
}

HRESULT UIHpBarRed::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT UIHpBarRed::NativeConstruct(void * pArg)
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

	random_device(dre);
//	m_fDistanceToCamera = 43;
	return S_OK;
}

void UIHpBarRed::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	m_iShaderPass = 3;// 20;
	if (GetKeyState(VK_F8) & 0x8000)
	{
		Set_Dead(1);
	}

	//m_fDistanceToCamera = 60;
	//if (m_isRedgone)
	//{

	//	m_isRendTime += TimeDelta;
	//	if (m_BarSize == 1)
	//	{
	//		m_tUIInfo.fCX -= 1.84f;
	//	}
	//	if (m_isRendTime > 2)
	//	{
	//		m_isRedgone = false;
	//		m_isRendTime = 0;
	//	}
	//	
	//}
	//Kind나누는것임
	/*if (pGameInstnace->Key_Up(DIK_7))LLLLLKKKKKKLLLLLLKKKKKLLLLKKKKKKKKKKKKKKKKKLLLLLLLLLLLLLLLLLLLL
	{

	m_isRedgone = true;


	}*/


	m_tUIInfo.fCX -= 0.6f;

	if (m_tUIInfo.fCX <= 0)
		m_bDead = true;




	__super::Tick(TimeDelta);
	pGameInstnace->Release();


}

void UIHpBarRed::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	rgb = 0.3f;
}

HRESULT UIHpBarRed::Render()
{
	if (!m_isBattleOn) {

		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT UIHpBarRed::SetUp_Texture()
{
	//Prototype_Component_Texture_HPBar

	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_RedHP", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To UIHpBarRed : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

UIHpBarRed * UIHpBarRed::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	UIHpBarRed* pInstance = new UIHpBarRed(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UIHpBarRed : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UIHpBarRed::Clone(void * pArg)
{
	UIHpBarRed* pInstance = new UIHpBarRed(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIHpBarRed : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void UIHpBarRed::Free()
{
	__super::Free();
}
