#include "stdafx.h"
#include "InfoFadeUI.h"

//ÀüÅõ
InfoFadeUI::InfoFadeUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

InfoFadeUI::InfoFadeUI(const InfoFadeUI& rhs)
	:CUI(rhs)
{
}

HRESULT InfoFadeUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InfoFadeUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_iShaderPass = 3;
	return S_OK;
}

void InfoFadeUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	if (GetKeyState(VK_F8) & 0x8000)
	{
		Set_Dead(1);
	}

	RECT rc = { 908,525,1004,544 };
	POINT pt = Get_Mouse(g_hWnd);
	if (PtInRect(&rc, pt) && GetAsyncKeyState(VK_RBUTTON)&0x8000)
	{
		Set_Dead(true);
	}


}

void InfoFadeUI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT InfoFadeUI::Render()
{





	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InfoFadeUI::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_InfoFadeUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To InfoFadeUI : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

InfoFadeUI * InfoFadeUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	InfoFadeUI* pInstance = new InfoFadeUI(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To InfoFadeUI : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * InfoFadeUI::Clone(void * pArg)
{
	InfoFadeUI* pInstance = new InfoFadeUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To InfoFadeUI : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void InfoFadeUI::Free()
{
}
