#include "stdafx.h"
#include"UI_Inven.h"
#include"FadeInOut.h"
#include"ItemList.h"
#include"InvenBtn.h"
#include"UICharacter.h"
#include"LobbyBtn.h"
#include"UI_BtnHightLight.h"
CUI_Inven::CUI_Inven(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Inven::CUI_Inven(const CUI_Inven& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Inven::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Inven::NativeConstruct(void * pArg)
{
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();



	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_tUIInfo.fCX = 1400;
	m_tUIInfo.fCY = 780;
	m_tUIInfo.fX = 640;
	m_tUIInfo.fY = 360;

	m_iShaderPass = 27;


	//인벤토리 버튼 효과임,들 먼저 만들어놓기   인벤토리 버튼 누르면 반짝거리는 거. 
	LobbyBtn::UIINFO uidesc;
	uidesc.fX = 450;
	uidesc.fY = 500;
	uidesc.fCX = 80;
	uidesc.fCY = 80;

	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_InvenBtn1", L"Prototype_GameObject_StartBtn_UI", &uidesc))
	{
		MSG_BOX(L"인벤버튼오브젝트가 조졋다구~");
		return  S_OK;
	}

	ZeroMemory(&uidesc, sizeof(LobbyBtn::UIINFO));
	uidesc.fX = 550;
	uidesc.fY = 500;
	uidesc.fCX = 80;
	uidesc.fCY = 80;


	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_InvenBtn2", L"Prototype_GameObject_StartBtn_UI", &uidesc))
	{
		MSG_BOX(L"인벤버튼오브젝트가 조졋다구~");
		return  S_OK;
	}

	ZeroMemory(&uidesc, sizeof(LobbyBtn::UIINFO));
	uidesc.fX = 650;
	uidesc.fY = 500;
	uidesc.fCX = 80;
	uidesc.fCY = 80;

	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_InvenBtn3", L"Prototype_GameObject_StartBtn_UI", &uidesc))
	{
		MSG_BOX(L"인벤버튼오브젝트가 조졋다구~");
		return  S_OK;
	}

	ZeroMemory(&uidesc, sizeof(LobbyBtn::UIINFO));
	uidesc.fX = 750;
	uidesc.fY = 500;
	uidesc.fCX = 80;
	uidesc.fCY = 80;


	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_InvenBtn4", L"Prototype_GameObject_StartBtn_UI", &uidesc))
	{
		MSG_BOX(L"인벤버튼오브젝트가 조졋다구~");
		return  S_OK;
	}

	ZeroMemory(&uidesc, sizeof(LobbyBtn::UIINFO));
	uidesc.fX = 850;
	uidesc.fY = 500;
	uidesc.fCX = 80;
	uidesc.fCY = 80;


	if (nullptr == pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_InvenBtn5", L"Prototype_GameObject_StartBtn_UI", &uidesc))
	{
		MSG_BOX(L"인벤버튼오브젝트가 조졋다구~");
		return  S_OK;
	}


	return S_OK;
}

void CUI_Inven::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();



	//인벤토리 입장 눌렸다.
	if (m_isInvenBtn)
	{
		if (!m_isFirstload) 
		{

			//////Inven Btn 크기 조절.		
			//CLayer* invenBtnlayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_InvenBtn1"));
			//InvenBtn* invenBtn = dynamic_cast<InvenBtn*>(*invenBtnlayer->Get_ObjectList().begin());
			//invenBtn->smaller();


			//ItemList 창도 켜자
			ItemList::UIINFO uidesc;
			//ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));

			uidesc.fX = g_iWinCX / 2 - 300;
			uidesc.fY = g_iWinCY / 2 - 200;
			uidesc.fCX = 400;
			uidesc.fCY = 60;

			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_ItemList", TEXT("Prototype_GameObject_ItemList"), &uidesc)))
				return;



			ZeroMemory(&uidesc, sizeof(ItemList::UIINFO));

			uidesc.fX = g_iWinCX / 2 - 300;
			uidesc.fY = g_iWinCY / 2 - 120;
			uidesc.fCX = 400;
			uidesc.fCY = 60;

			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_ItemList", TEXT("Prototype_GameObject_ItemList"), &uidesc)))
				return;


			ZeroMemory(&uidesc, sizeof(ItemList::UIINFO));

			uidesc.fX = g_iWinCX / 2 - 300;
			uidesc.fY = g_iWinCY / 2 - 40;
			uidesc.fCX = 400;
			uidesc.fCY = 60;

			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_ItemList", TEXT("Prototype_GameObject_ItemList"), &uidesc)))
				return;


			ZeroMemory(&uidesc, sizeof(ItemList::UIINFO));

			uidesc.fX = g_iWinCX / 2 - 300;
			uidesc.fY = g_iWinCY / 2 + 50;
			uidesc.fCX = 400;
			uidesc.fCY = 100;						

			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_ItemList", TEXT("Prototype_GameObject_ItemList"), &uidesc)))
				return;

			//옆에 괴물 이미지도 켜자. 

			CUICharacter::UICHDESC uichdesc;
			uichdesc.tUIInfo.fCX = 700;
			uichdesc.tUIInfo.fCY = 700;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 + 240;
			uichdesc.tUIInfo.fY = g_iWinCY / 2;
			uichdesc.kind = 0;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uichar", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//위에 괴물 이미지도 켜자. 

			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 50;
			uichdesc.tUIInfo.fX = g_iWinCX / 2-390 ;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 290;
			uichdesc.kind = 4;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uicharAlphen", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//위에 괴물 이미지도 켜자. 

			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 50;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 335;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 290;
			uichdesc.kind = 5;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uicharRInwel", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 50;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 280;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 290;
			uichdesc.kind = 6;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uicharsion", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 50;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 225;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 290;
			uichdesc.kind = 7;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uicharSakira", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//HP관련 인포창..
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 70;
			uichdesc.tUIInfo.fX =218;
			uichdesc.tUIInfo.fY =410;
			uichdesc.kind = 8;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfo", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//무기 이름 : 각죄의검
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 85;
			uichdesc.tUIInfo.fCY = 25;
			uichdesc.tUIInfo.fX = 270;
			uichdesc.tUIInfo.fY = 160;
			uichdesc.kind = 9;					
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//무기 제목
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 28;
			uichdesc.tUIInfo.fCY = 15;
			uichdesc.tUIInfo.fX = 197;
			uichdesc.tUIInfo.fY = 124;
			uichdesc.kind = 10;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//방어구
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 40;
			uichdesc.tUIInfo.fCY = 15;
			uichdesc.tUIInfo.fX = 203;
			uichdesc.tUIInfo.fY = 205;
			uichdesc.kind = 11;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			// 액세서리 
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 20;
			uichdesc.tUIInfo.fX = 210;
			uichdesc.tUIInfo.fY = 280;
			uichdesc.kind = 16;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//각좌의 방패
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 100;
			uichdesc.tUIInfo.fCY = 25;
			uichdesc.tUIInfo.fX = 275;
			uichdesc.tUIInfo.fY = 238;
			uichdesc.kind = 12;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//없음
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 40;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = 244;
			uichdesc.tUIInfo.fY = 316;
			uichdesc.kind = 22;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoARmy", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//공격력 그림
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 15;
			uichdesc.tUIInfo.fCY = 15;
			uichdesc.tUIInfo.fX = 187;
			uichdesc.tUIInfo.fY = 399;
			uichdesc.kind = 17;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoArmyIcon", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//관통력 그림
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 15;
			uichdesc.tUIInfo.fCY = 15;
			uichdesc.tUIInfo.fX = 187;
			uichdesc.tUIInfo.fY = 416;
			uichdesc.kind = 18;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoArmyIcon", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//방어력 그림
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 15;
			uichdesc.tUIInfo.fCY = 15;
			uichdesc.tUIInfo.fX = 187;
			uichdesc.tUIInfo.fY = 433;
			uichdesc.kind = 19;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoArmyIcon", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//원전  그림
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 30;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = 206;
			uichdesc.tUIInfo.fY = 157;
			uichdesc.kind = 21;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoArmyIcon", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//마도복 그림
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 30;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = 206;
			uichdesc.tUIInfo.fY = 240;
			uichdesc.kind = 20;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_uiInfoArmyIcon", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;





			//Q
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 30;				
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 460;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 285;
			uichdesc.kind = 14;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_BtnQ", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//E
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 30;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 150;
			uichdesc.tUIInfo.fY = g_iWinCY / 2 - 285;
			uichdesc.kind = 15;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_BtnE", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//결정
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 50;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 - 150;
			uichdesc.tUIInfo.fY = 618;
			uichdesc.kind = 33;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_BtnE", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//돌아간다
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 70;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 + 150;
			uichdesc.tUIInfo.fY = 618;
			uichdesc.kind = 32;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_BtnE", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;

			//캐릭터를본다
			ZeroMemory(&uichdesc, sizeof(CUICharacter::UICHDESC));
			uichdesc.tUIInfo.fCX = 90;
			uichdesc.tUIInfo.fCY = 30;
			uichdesc.tUIInfo.fX = g_iWinCX / 2 ;
			uichdesc.tUIInfo.fY = 618;
			uichdesc.kind = 31;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_BtnE", TEXT("Prototype_GameObject_UICharacter"), &uichdesc)))
				return;


			//HightLight 사람 모형 UI 하이라이트


			//알렌
			CUI_BtnHightLight::UIHIGHTDESC uidescda;

			uidescda.tUIInfo.fCX = 50;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = g_iWinCX / 2 - 390;
			uidescda.tUIInfo.fY = g_iWinCY / 2 - 290;
			uidescda.kind = 0;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

			//키사라
			uidescda.tUIInfo.fCX = 50;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = g_iWinCX / 2 - 335;
			uidescda.tUIInfo.fY = g_iWinCY / 2 - 290;
			uidescda.kind = 8;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

			//시온
			uidescda.tUIInfo.fCX = 50;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = g_iWinCX / 2 - 280;
			uidescda.tUIInfo.fY = g_iWinCY / 2 - 290;
			uidescda.kind = 9;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

			//멜링?
			uidescda.tUIInfo.fCX = 50;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = g_iWinCX / 2 - 225;
			uidescda.tUIInfo.fY = g_iWinCY / 2 - 290;
			uidescda.kind = 10;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;



			//HightLight2  아이템 박스 1 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 338;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = 340;
			uidescda.tUIInfo.fY = 160;
			uidescda.kind = 1;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;
			//HightLigh3  아이템 박스 2 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 338;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = 340;
			uidescda.tUIInfo.fY = 239;
			uidescda.kind = 2;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

			//HightLigh3 아이템 박스 3 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 338;
			uidescda.tUIInfo.fCY = 50;
			uidescda.tUIInfo.fX = 340;
			uidescda.tUIInfo.fY = 317;
			uidescda.kind = 3;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;


			//HightLigh3  아이템 박스 4 하이라이트 
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 338;
			uidescda.tUIInfo.fCY = 80;
			uidescda.tUIInfo.fX = 340;
			uidescda.tUIInfo.fY = 410;
			uidescda.kind = 4;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;	

			//결정박스 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 80;
			uidescda.tUIInfo.fCY = 30;
			uidescda.tUIInfo.fX = g_iWinCX / 2 - 150;
			uidescda.tUIInfo.fY = 618;
			uidescda.kind = 5;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight_Pick", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

			//캐릭터를 본다 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 90;
			uidescda.tUIInfo.fCY = 30;
			uidescda.tUIInfo.fX = g_iWinCX / 2;
			uidescda.tUIInfo.fY = 618;
			uidescda.kind = 6;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight_LookCh", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;


			//esc 하이라이트
			ZeroMemory(&uidescda, sizeof(CUI_BtnHightLight::UIHIGHTDESC));

			uidescda.tUIInfo.fCX = 80;
			uidescda.tUIInfo.fCY = 30;
			uidescda.tUIInfo.fX = g_iWinCX / 2 + 150;
			uidescda.tUIInfo.fY = 618;
			uidescda.kind = 7;
			if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_HightLight_esc", TEXT("Prototype_GameObject_BtnHightLight"), &uidescda)))
				return;

		
			////fade도 좀 더 찐하게.
			CLayer* invenlayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_RealFade"));
			CFadeInOut* invenUI = dynamic_cast<CFadeInOut*>(*invenlayer->Get_ObjectList().begin());
			invenUI->ChangeEnterrgb(true);

			//Prototype_GameObject_GlowUI


			//한번만 생성하라구~
			m_isFirstload = true;
		}
		
		//크기 잠깐 줄어주는 효과
		if (m_tUIInfo.fCX >= 1280) {
			m_tUIInfo.fCX = 1280;
			m_tUIInfo.fCY = 720;
		}
		else
		{
			m_tUIInfo.fCX -= 4;
			m_tUIInfo.fCY -= 4;
		}

	}
	else
	{	



		CLayer* invenlayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_RealFade"));
		CFadeInOut* invenUI = dynamic_cast<CFadeInOut*>(*invenlayer->Get_ObjectList().begin());
		invenUI->Changebyergb(true);



		////입벤토리 퇴장키 눌렸다.R키눌렸다.
		m_tUIInfo.fCX = 1400;
		m_tUIInfo.fCY = 780;

		//r			
		CLayer* ItemListlayer = pGameInstnace->Find_Layer(LEVEL_GAME1, TEXT("Layer_ItemList"));
		
		if (ItemListlayer == nullptr) return;
		ItemList* ItemListUI = dynamic_cast<ItemList*>(*ItemListlayer->Get_ObjectList().begin());
		ItemListUI->RenderOn(false);

		
		////입벤토리 퇴장키 눌렸다.

		//if (m_tUIInfo.fCX <= 1400) {


		//	m_tUIInfo.fCX = 1400;
		//	m_tUIInfo.fCY = 780;
		//}
		//else
		//{
		//	m_tUIInfo.fCX += 4;
		//	m_tUIInfo.fCY += 4;
		//}




	}

	//그림바꿔주기
	if (m_isChangeCut)
	{
		m_iSprite = 1;
	}
	//알파값 조종
	if (!m_isFirst)
		rgb += 0.008f;

	if (rgb > 1) {
		rgb = 1;
		m_isFirst = true;

	}


}

void CUI_Inven::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUI_Inven::Render()
{


	
	if (FAILED(m_pShader->Set_RawValue("invenuirgb", &rgb, sizeof(float))))
		return E_FAIL;




	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Inven::Ready_layer_InvenBtnGlow(const _tchar * pLayerTag)
{



	




	return S_OK;
}

HRESULT CUI_Inven::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_GAME1, L"Prototype_Component_Texture_Inven", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To CUI_Inven : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUI_Inven * CUI_Inven::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Inven* pInstance = new CUI_Inven(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUI_Inven : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Inven::Clone(void * pArg)
{
	CUI_Inven* pInstance = new CUI_Inven(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI_Inven : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Inven::Free()
{
}
