#include "stdafx.h"
#include"../Public/MouseUI.h"

MouseUI::MouseUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

MouseUI::MouseUI(const MouseUI& rhs)
	: CUI(rhs)
{
}

HRESULT MouseUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MouseUI::NativeConstruct(void * pArg)
{

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}




	m_tUIInfo.fCX = 80;
	m_tUIInfo.fCY = 80;
	m_tUIInfo.fX = 100;
	m_tUIInfo.fY = 450;

	m_iShaderPass = 13;

	m_fDistanceToCamera = 300;


	return S_OK;
}

void MouseUI::Tick(_double TimeDelta)
{

	//mouse 위치 좌표 맞추기..
	POINT pt = Get_Mouse(g_hWnd);

	m_tUIInfo.fX = pt.x + 15;
	m_tUIInfo.fY = pt.y + 25;

	__super::Tick(TimeDelta);


	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	if (m_isClick)
	{
		Delete_Component(TEXT("Com_Texture"));

		if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_MouseClick", (CComponent**)&m_pTextureCom)))
			return;
	}
	else {
		Delete_Component(TEXT("Com_Texture"));

		if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Mouse", (CComponent**)&m_pTextureCom)))
			return;

	}



	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_isClick = true;
	}
	else
	{
		m_isClick = false;
	}






	Safe_Release(pGameInstnace);

}

void MouseUI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT MouseUI::Render()
{

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MouseUI::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Mouse", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To MouseUI : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

MouseUI * MouseUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	MouseUI* pInstance = new MouseUI(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To MouseUI : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * MouseUI::Clone(void * pArg)
{
	MouseUI* pInstance = new MouseUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To MouseUI : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MouseUI::Free()
{
	__super::Free();

}
