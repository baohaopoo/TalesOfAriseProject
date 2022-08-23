#include "stdafx.h"
#include"UI_BtnHightLight.h"
#include"UICharacter.h"
CUI_BtnHightLight::CUI_BtnHightLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_BtnHightLight::CUI_BtnHightLight(const CUI_BtnHightLight& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_BtnHightLight::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_BtnHightLight::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	UIkind = ((UIHIGHTDESC*)pArg)->kind;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_fDistanceToCamera = 99.98;
	m_iShaderPass = 28;
	return S_OK;				
}

void CUI_BtnHightLight::Tick(_double TimeDelta)
{

	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	//사람 얼굴 나오는 UI
	if (UIkind == 0)
	{
		RECT rc1 = { g_iWinCX / 2 - 410, g_iWinCY / 2 - 310, g_iWinCX / 2 - 365, g_iWinCY / 2 - 265 };
		


		POINT pt = Get_Mouse(g_hWnd);
////////////////////////////////////////////////////////////////////////////
		if ( PtInRect(&rc1, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}


		if (pGameInstnace->Key_Down(DIK_Q))
		{
			m_tUIInfo.fX += 55;
		}

		if (pGameInstnace->Key_Down(DIK_E))
		{
				m_tUIInfo.fX -= 55;
		}
	}
	//무기란.
	if (UIkind == 1)
	{
		RECT rc = { 172,131, 500, 172 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			CUICharacter::UICHDESC uichdesc;
			if (!m_isFirst) {


				//각죄의검.
		
				uichdesc.tUIInfo.fCX = 500;
				uichdesc.tUIInfo.fCY = 100;
				uichdesc.tUIInfo.fX = 420;
				uichdesc.tUIInfo.fY = 522;
				uichdesc.kind = 23;
				if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_AlpenArmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
					return;


			

				m_isFirst = true;

			}
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
				//여기서 화살표 빤짝반짝.

				ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
				uichdesc.tUIInfo.fCX = 50;
				uichdesc.tUIInfo.fCY = 50;
				uichdesc.tUIInfo.fX = 171;
				uichdesc.tUIInfo.fY = 158;
				uichdesc.kind = 34;
				if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_pickcursor", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
					return;


			}
			
		}
		else
		{ 
			rgb = 0.4;
			RenderFunc(false);



		}
	}

	if (UIkind == 2)
	{

		RECT rc = { 180, 210, 495, 235};
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{


			RenderFunc(true);
			if (!m_isFirst) {


				//방어구
				CUICharacter::UICHDESC uichdesc;

				uichdesc.tUIInfo.fCX = 560; 
				uichdesc.tUIInfo.fCY =	130;
				uichdesc.tUIInfo.fX = 450;		
				uichdesc.tUIInfo.fY = 528;				
				uichdesc.kind = 24;						
				if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_AlpenPassing", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
					return;

				m_isFirst = true;

			}

			

			m_isFirst = true;
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;


			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);


		}
	}



	if (UIkind == 3)
	{

		RECT rc = { 176, 293, 501, 337 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 4)
	{

		RECT rc = { 177, 367, 504, 439 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 5)
	{

		RECT rc = { 458, 599, 507, 635 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 6)
	{

		RECT rc = { 593, 598, 683, 619 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 7)
	{
		//ESC키
		RECT rc = { 752, 599, 828, 623 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
				//이전 레벨로 돌아가야함. 


				m_isRender = false;

			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}


	if (UIkind == 8)
	{
		//키사라 
		RECT rc = { 267,48,321,83 };
		POINT pt = Get_Mouse(g_hWnd);



		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
				//이전 레벨로 돌아가야함. 


				m_isRender = false;

			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 9)
	{
		//시온 
		RECT rc = { 339, 48, 375, 81 };
		POINT pt = Get_Mouse(g_hWnd);

	
		

		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
				//이전 레벨로 돌아가야함. 


				m_isRender = false;

			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}

	if (UIkind == 10)
	{
		//멜ㄹ랑 
		RECT rc = { 397, 48, 436, 84 };
		POINT pt = Get_Mouse(g_hWnd);

		


		if (/*GetAsyncKeyState(VK_LBUTTON) &&*/ PtInRect(&rc, pt))
		{
			RenderFunc(true);
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				rgb = 0.7;
				//이전 레벨로 돌아가야함. 


				m_isRender = false;

			}

		}
		else
		{
			rgb = 0.4;
			RenderFunc(false);
		}
	}
}

void CUI_BtnHightLight::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	
}

HRESULT CUI_BtnHightLight::Render()
{
	if (m_isRender) {


		if (FAILED(m_pShader->Set_RawValue("btnhightrgb", &rgb, sizeof(float))))
			return E_FAIL;


	
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CUI_BtnHightLight::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_BtnHightLight", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To CUI_BtnHightLight : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUI_BtnHightLight * CUI_BtnHightLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_BtnHightLight* pInstance = new CUI_BtnHightLight(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUI_BtnHightLight : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_BtnHightLight::Clone(void * pArg)
{
	CUI_BtnHightLight* pInstance = new CUI_BtnHightLight(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI_BtnHightLight : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_BtnHightLight::Free()
{
}
