#include "stdafx.h"
#include"InvenBtn.h"
#include"LobbyBtn.h"
//invenbtnrgb
InvenBtn::InvenBtn(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CButton_Super(pDevice, pDeviceContext)
{
}

InvenBtn::InvenBtn(const InvenBtn& rhs)
	: CButton_Super(rhs)
{
}

HRESULT InvenBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT InvenBtn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	tex = ((UIINVENBTNDESC*)pArg)->texnum;
	m_iShaderPass = 16; //16
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	//Start 버튼 - 바로 사라져버림..
	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_StartBtn3", L"Prototype_GameObject_StartBtn_UI"))
	{
		MSG_BOX(L"버튼오브젝트가 조졋다구~");
		return E_FAIL;
	}

	m_fDistanceToCamera = 3;
	return S_OK;
}

void InvenBtn::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	RECT rc1 = { 430,465,470,535 };
	RECT rc2 = { 530,465,570,535 };
	RECT rc3 = { 630,465,670,535 };
	RECT rc4 = { 730,465,770,535 };
	RECT rc5 = { 830,465,870,535 };

	POINT pt = Get_Mouse(g_hWnd);

	if (m_isBtnSmaller)
	{
		smaller();
	}

	if (PtInRect(&rc1, pt) && GetKeyState(VK_LBUTTON)&0x8000)	//rc1눌렀을떄,
	{

		CLayer* LogoLayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
		LobbyBtn* logoUI = dynamic_cast<LobbyBtn*>(*LogoLayer->Get_ObjectList().begin());
		logoUI->RenderFunc(false);


		//여기가 실질적으로 smaller하는 곳.
		m_isBtnSmaller = true;

		//smaller();
	}
	if (PtInRect(&rc2, pt) && GetKeyState(VK_LBUTTON) & 0x8000)	//rc1눌렀을떄,
	{

		CLayer* LogoLayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
		LobbyBtn* logoUI = dynamic_cast<LobbyBtn*>(*LogoLayer->Get_ObjectList().begin());
		logoUI->RenderFunc(false);


		//여기가 실질적으로 smaller하는 곳.
		m_isBtnSmaller = true;

		//smaller();
	}

	if (PtInRect(&rc3, pt) && GetKeyState(VK_LBUTTON) & 0x8000)	//rc1눌렀을떄,
	{

		CLayer* LogoLayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
		LobbyBtn* logoUI = dynamic_cast<LobbyBtn*>(*LogoLayer->Get_ObjectList().begin());
		logoUI->RenderFunc(false);


		//여기가 실질적으로 smaller하는 곳.
		m_isBtnSmaller = true;

		//smaller();
	}

	if (PtInRect(&rc4, pt) && GetKeyState(VK_LBUTTON) & 0x8000)	//rc1눌렀을떄,
	{

		CLayer* LogoLayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
		LobbyBtn* logoUI = dynamic_cast<LobbyBtn*>(*LogoLayer->Get_ObjectList().begin());
		logoUI->RenderFunc(false);


		//여기가 실질적으로 smaller하는 곳.
		m_isBtnSmaller = true;

		//smaller();
	}

	if (PtInRect(&rc5, pt) && GetKeyState(VK_LBUTTON) & 0x8000)	//rc1눌렀을떄,
	{

		CLayer* LogoLayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
		LobbyBtn* logoUI = dynamic_cast<LobbyBtn*>(*LogoLayer->Get_ObjectList().begin());
		logoUI->RenderFunc(false);


		//여기가 실질적으로 smaller하는 곳.
		m_isBtnSmaller = true;

		//smaller();
	}
	switch (tex)
	{
	case 0:
		m_iSprite = 0;
		break;
	case 1:
		m_iSprite = 1;
		break;
	case 2:
		m_iSprite = 2;
		break;
	case 3:
		m_iSprite =3;
		break;
	case 4:
		m_iSprite = 4;
		break;
	default:
		break;
	}

}

void InvenBtn::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);


}

HRESULT InvenBtn::Render()
{
	

	if (FAILED(m_pShader->Set_RawValue("invenbtnrgb", &rgb, sizeof(float))))
		return E_FAIL;



	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

void InvenBtn::smaller()
{
	m_tUIInfo.fX += 10;

	if (m_tUIInfo.fCX <= 1) {
		m_tUIInfo.fCX = 1;
	}
	else
	{
		m_tUIInfo.fCX -= 5;
		m_tUIInfo.fCY -= 5;
	}

}

void InvenBtn::bigger()
{
	m_tUIInfo.fX -= 10;


	if (m_tUIInfo.fCX >= 70) {
		m_tUIInfo.fCX = 70;
	}
	else
	{
		m_tUIInfo.fCX += 5;
		m_tUIInfo.fCY += 5;
	}

}

HRESULT InvenBtn::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_GAME1, L"Prototype_Component_Texture_InvenBtn", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"texture오류다임마.");
		return E_FAIL;
	}
}



InvenBtn * InvenBtn::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	InvenBtn* pInstance = new InvenBtn(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To InvenBtn : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * InvenBtn::Clone(void * pArg)
{
	InvenBtn* pInstance = new InvenBtn(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To InvenBtn : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void InvenBtn::Free()
{
}
