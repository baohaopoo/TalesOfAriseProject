#include "stdafx.h"
#include "..\Public\Level_Tutorial.h"


#include "Sword.h"
#include "GameInstance.h"
#include "Camera_Default.h"
#include"..\Public\UIBackground.h"
#include"..\Public\UIEmblem.h"
#include"..\Public\UIExplain.h"
#include"..\Public\UI_Smash.h"
#include"..\Public\UI_SmashEffect.h"
#include"..\Public\UILine.h"
#include"..\Public\UINumber.h"
#include"..\Public\SmashNum.h"
#include"..\Public\UIName.h"
#include"..\Public\UIBar.h"


#include"UI_Manager.h"
#include"LineMsgUI.h"
#include"UI_Manager.h"
#include"TutorialMsg.h"
#include"MiniMap.h"
#include"TutoLineMsg.h"
#include"LineSprite.h"
#include "Layer.h"	//이거 겜인스턴스에 넣어주자.
#include"MiniMapView.h"
#include"UIGlow.h"
#include"UIBillBoard.h"
#include "UIStatus.h"
#include "UIFlag.h"


#include "MapObject.h"
#include "WayPoint.h"
#include "Player.h"
#include "Navigation.h"
#include "Level_Loading.h"


#include "ImGui_Manager.h"



CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
{
	Safe_AddRef(m_pBattle_Manager);
}

HRESULT CLevel_Tutorial::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;	

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Balseph(TEXT("Layer_Balseph"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_FireAvatar(TEXT("Layer_FireAvatar"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Mantis(TEXT("Layer_Mantis"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Punisher(TEXT("Layer_Punisher"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Boar(TEXT("Layer_Boar"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Practice(TEXT("Layer_Practice"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	if (FAILED(Ready_Map("../Bin/Resources/Dat/Stage01/", "MeshList.dat", XMVectorSet(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(Ready_Map_Battle02(XMVectorSet(1000.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(Ready_Map_Battle04(XMVectorSet(2000.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Tutorial::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (nullptr == pGameInstance)
		return;

	m_pBattle_Manager->Tick(TimeDelta);
	int cp = m_pBattle_Manager->GetCP();
	cp -= 5;
	m_pBattle_Manager->SetCP(cp);

	if (m_pBattle_Manager->Get_Battle())
	{
		if (!m_bFirstBattleUI) {
			//GetBattle이면 바로 그냥~
			if (FAILED(Ready_Layer_Battle_UI()))
				return;

			m_bFirstBattleUI = true;

			//m_isComboHit = true;
			CUI_Manager* pUIManager = CUI_Manager::GetInstance();
			CPlayer_Manager* pPlayerManager = CPlayer_Manager::GetInstance();

			Safe_AddRef(pUIManager);
			Safe_AddRef(pPlayerManager);



			CPlayer* player = pUIManager->GetPlayer((pPlayerManager->Get_MainPlayerIndex()));
			combohitNum = player->Get_PlayerInfo().m_iComboHits;



			Safe_Release(pUIManager);
			Safe_Release(pPlayerManager);
		}
	}


	//LineMsg관련된 코드.
	if (LineMsgCreate)
	{
		LineMsgUI::MSGLINEUIDESC LineMsgDESC;
		LineMsgDESC.uidesc.fX = g_iWinCX / 2;
		LineMsgDESC.uidesc.fY = g_iWinCX / 2 - 700;
		LineMsgDESC.uidesc.fCX = 700;
		LineMsgDESC.uidesc.fCY = 250;
		LineMsgDESC.sprite = LineMsgSprite;  //1부터 쓰는 이유가 있다.

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineMsgUI"), &LineMsgDESC)))
			return;

		LineMsgCreate = false;
	}
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD7))
	{

		//마신검. 붕쇠 이런거 다 바꿔놓아야함. 해당 몬스터랑 부딫혔을때 각각 다른거 생성시키면 될듯.
		LineMsgCreate = true;


	}

	//맨티스와의 전투를 준비하세요 코드.
	if (pGameInstance->Get_DIKeyState(DIK_F10))
	{

		//Line
		UILine::LINEDESC linedesc;
		linedesc.uidesc.fX = 0;
		linedesc.uidesc.fY = g_iWinCX / 2 - 700;
		linedesc.uidesc.fCX = 1920;
		linedesc.uidesc.fCY = 80;
		linedesc.kind = 1;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
			return;


		//위에 있는 TutoLine위에 써질 전투 메시지 이를테면 멘티스와의 전투를 준비하세요.
		//5 - world탐험 6- 발셉과의 7 - 멘티스와의 전투 
		//그냥 새클래스 만드세요
		//TutoLineMsg
		TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
		TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
		TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
		TutoLineMsgDesc.uidesc.fCX = 500;
		TutoLineMsgDesc.uidesc.fCY = 70;
		TutoLineMsgDesc.sprite = 1;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
			return;


		////LineSprite
		LineSprite::MINIMAPDESC LineSpritedesc;
		LineSpritedesc.uidesc.fX = g_iWinCX / 2;
		LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
		LineSpritedesc.uidesc.fCX = 400;
		LineSpritedesc.uidesc.fCY = 400;
		LineSpritedesc.kind = 0;

		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
			return;

	}

	//Tutorial 레벨 다 꺼버리는 함수.
	if (pGameInstance->Get_DIKeyState(DIK_F11))
	{
		m_isBattle = false;
		//Tutorial 레벨 다 꺼버리기
		CLayer* TutorialLayer = pGameInstance->Find_Layer(LEVEL_TUTORIAL, TEXT("Layer_TutorialUI"));
		list<CGameObject*>& TutorialList = TutorialLayer->Get_ObjectList();

		for (auto& iter : TutorialList)
		{
			//후후
			dynamic_cast<CUI*>(iter)->RenderChange(false);
		}
	}




	//F12누르면, TutorialLayer 다 켜버리는 함수
	if (pGameInstance->Get_DIKeyState(DIK_F12))
	{

		//tutolevel 다 켜버리기
		CLayer* TutorialLayer = pGameInstance->Find_Layer(LEVEL_TUTORIAL, TEXT("Layer_TutorialUI"));
		list<CGameObject*>& TutorialList = TutorialLayer->Get_ObjectList();
		m_isBattle = false;
		for (auto& iter : TutorialList)
		{
			//후후
			dynamic_cast<CUI*>(iter)->RenderChange(true);
		}

	}


	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD3))
	{

		//SmashUI alphen
		CUI_SmashEffect::UIINFO uidesc;

		uidesc.fX = 190;
		uidesc.fY = 712;
		uidesc.fCX = 10;
		uidesc.fCY = 10;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return;

	}
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD4))
	{

		CUI_SmashEffect::UIINFO uidesc;

		ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
		uidesc.fX = 130;
		uidesc.fY = 777;
		uidesc.fCX = 140;
		uidesc.fCY = 150;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return;


	}


	
	Safe_Release(pGameInstance);

}

HRESULT CLevel_Tutorial::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("튜토리얼 레벨입니다."));

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (m_isBattle) {

		wsprintf(HitMsg, TEXT("HITS"));
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan40"), HitMsg, _float2(1787, 217/*g_iWinCX / 2 + 700, g_iWinCY / 2 - 200*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;



		wsprintf(DamageMsg, TEXT("DAMAGES"));
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan40"), DamageMsg, _float2(1747, 267/*g_iWinCX / 2 + 650, g_iWinCY / 2 - 150*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;

		wsprintf(ComboHit, TEXT("%d"), combohitNum);
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan50"), ComboHit, _float2(1680, 150), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;


		wsprintf(DamageHit, TEXT("%d"), damageNum);
		if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan50"), DamageHit, _float2(1680, 210), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
			return E_FAIL;
	
	}
	
	return S_OK;

}


HRESULT CLevel_Tutorial::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float4(10.f, 4.f, 10.f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.0f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);	
	//LightDesc.vSpecular = _float4(1.f, 0.0f, 0.0f, 1.f);
	//if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;	

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float4(15.f, 4.f, 10.f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.0f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//LightDesc.vSpecular = _float4(0.f, 1.f, 0.0f, 1.f);
	//if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.eType = CCamera::CAMERA;
	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc))
		return E_FAIL;

	//for.lightdepth
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.eType = CCamera::LIGHT;
	CameraDesc.vEye = _float3(0.f, 20.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 500.f;
	CameraDesc.fFovy = XMConvertToRadians(120.f);
	CameraDesc.fAspect = (_float)g_iLightCX / g_iLightCY;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_LightDepth"), &CameraDesc))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// Alphen 생성
	m_pPlayerAlphen = dynamic_cast<CPlayer*>(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Alphen")));
	if (nullptr == m_pPlayerAlphen)
		return E_FAIL;

	CNavigation* pNavigation = nullptr;
	CTransform* pAlphen_TransformCom = nullptr;


	// 맵의 Navigation Mesh를 Alphen이 타게 함.
	pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_Tutorial"));

	pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
	pAlphen_TransformCom->Move(-133.25f, -6.7f, -27.5f);
	pNavigation->Find_My_Cell(XMVectorSet(-133.25f, -6.7f, -27.5f, 1.f));
	pAlphen_TransformCom->Move(-133.25f, pAlphen_TransformCom->Get_Height(pNavigation), -27.5f);


	// 맵의 Navigation Mesh를 Alphen이 타게 함.
	/*pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_Tutorial"));

	pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
	pAlphen_TransformCom->Move(35.91f, 3.84f, 0.37f);
	pAlphen_TransformCom->Move(35.91f, pAlphen_TransformCom->Get_Height(pNavigation), 0.37f);
	pNavigation->Find_My_Cell(XMVectorSet(35.91f, 0.f, 0.37f, 1.f));*/
	//	break;
	//case Client::MAP_BALSELPH:

	//	// 맵의 Navigation Mesh를 Alphen이 타게 함.
	//	pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_Balseph"));

	//	pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
	//	pAlphen_TransformCom->Move(-75.31, 19.25, 0.02);
	//	pAlphen_TransformCom->Move(-75.31, pAlphen_TransformCom->Get_Height(pNavigation), 0.02);
	//	pNavigation->Find_My_Cell(XMVectorSet(-75.31, 19.25, 0.02, 1.f));
	//	break;
	//case Client::MAP_FIREAVATAR:

	//	// 맵의 Navigation Mesh를 Alphen이 타게 함.
	//	pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_FireAvatar"));

	//	pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
	//	pAlphen_TransformCom->Move(-91.22, 2.42, 0.32);
	//	pAlphen_TransformCom->Move(-91.22, pAlphen_TransformCom->Get_Height(pNavigation), 0.32);
	//	pNavigation->Find_My_Cell(XMVectorSet(-91.22, 2.42, 0.32, 1.f));
	//	break;
	//case Client::MAP_BATTLE02:

	//	// 맵의 Navigation Mesh를 Alphen이 타게 함.
		//pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_02"));

		//pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
		//pAlphen_TransformCom->Move(0.f, 0.f, 0.f);
		//pAlphen_TransformCom->Move(0.f, pAlphen_TransformCom->Get_Height(pNavigation), 0.f);
		//pNavigation->Find_My_Cell(XMVectorSet(0.f, 0.f, 0.f, 1.f));
	//	break;
	//case Client::MAP_BATTLE04:

	//	// 맵의 Navigation Mesh를 Alphen이 타게 함.
	//	pNavigation = m_pPlayerAlphen->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_04"));

	//	pAlphen_TransformCom = dynamic_cast<CTransform*>(m_pPlayerAlphen->Get_Component(TEXT("Com_Transform")));
	//	pAlphen_TransformCom->Move(0.f, 0.f, 0.f);
	//	pAlphen_TransformCom->Move(0.f, pAlphen_TransformCom->Get_Height(pNavigation), 0.f);
	//	pNavigation->Find_My_Cell(XMVectorSet(0.f, 0.f, 0.f, 1.f));
	//	break;





	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Shionne")))
		return E_FAIL;



	//if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sword"), &SocketDesc))
	//	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Balseph(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Balseph")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	CUIBackground::UIBACKDESC uidesc;
	uidesc.kind = 3;
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BackGroundUI"), &uidesc))
		return E_FAIL;



	//if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Terrain")))
	//	return E_FAIL;

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Sky")))
		return E_FAIL;




	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Rect_Effect")))
		return E_FAIL;


	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Point_Effect")))
		return E_FAIL;

	for (_uint i = 0; i < 20; ++i)
	{
		if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Effect")))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;


	
}

HRESULT CLevel_Tutorial::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	m_isBattle = false;
	if (FAILED(Ready_Layer_Tutorial_UI())) //Tutorial Layer
		return E_FAIL;


	//if (FAILED(Ready_Layer_BarUI(L"Layer_BarUI"))) //하양이
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HP(L"Layer_HpUI"))) //초록이
	//	return E_FAIL;
	////빨강이 SEt_dead 이슈 고쳐지면 풀어서 쓰면 될듯





	/*if (FAILED(Ready_Layer_Balseph(TEXT("Layer_Balseph"))))
	return E_FAIL;*/

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	///////////falied to texture


	////////////그냥 이미지만 있는거
	//if (FAILED(Ready_Layer_SmashNum(L"Layer_SmashNum")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_AlphaTexture(L"Layer_chTalk")))
	//	return E_FAIL;




	Safe_Release(pGameInstance);


	return S_OK;
}



HRESULT CLevel_Tutorial::Ready_Layer_EmblemUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	UIINFO uidesc;
	uidesc.fX = g_iWinCX / 2;
	uidesc.fY = 400;
	uidesc.fCX = 400;
	uidesc.fCY = 400;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_EmblemUI"), &uidesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_ExUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//뒤에 배경 검정 UI
	UIExplain::UIEXDESC uidesc;
	uidesc.kind = 1;
	uidesc.tUIInfo.fX = 150;
	uidesc.tUIInfo.fY = 319;
	uidesc.tUIInfo.fCX = 0.7f;	//428
	uidesc.tUIInfo.fCY = 200.f;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
		return E_FAIL;

	//BATTLE TEX
	ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	uidesc.kind = 0;
	uidesc.tUIInfo.fX = 150;
	uidesc.tUIInfo.fY = 310;
	uidesc.tUIInfo.fCX = 300;
	uidesc.tUIInfo.fCY = 200;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
		return E_FAIL;

	//RESULT
	/*UIExplain::UIEXDESC uidesc;
	ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	uidesc.kind = 2;
	uidesc.tUIInfo.fX =100;
	uidesc.tUIInfo.fY = 600;
	uidesc.tUIInfo.fCX =500;
	uidesc.tUIInfo.fCY = 600;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
	return E_FAIL;*/

	////전투 설명 UI
	//ZeroMemory(&uidesc, sizeof(UIExplain::UIEXDESC));
	//

	//uidesc.kind = 0;
	//uidesc.tUIInfo.fX = 175;
	//uidesc.tUIInfo.fY = 200;
	//uidesc.tUIInfo.fCX = 350;
	//uidesc.tUIInfo.fCY = 200;



	//if (nullptr == (m_pGameInstance->Add_GameObjectToLayer(LEVEL_GAME1, pLayerTag, TEXT("Prototype_GameObject_ExplainUI"), &uidesc)))
	//	return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_SmashUI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Smash::UISMDESC uidesc;
	uidesc.tUIInfo.fX = 190;
	uidesc.tUIInfo.fY = 712;//470
	uidesc.tUIInfo.fCX = 110;
	uidesc.tUIInfo.fCY = 110;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
		return E_FAIL;

	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 190;
	//uidesc.tUIInfo.fY = 762;
	//uidesc.tUIInfo.fCX = 80;
	//uidesc.tUIInfo.fCY = 80;
	//uidesc.kind = 1;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	uidesc.tUIInfo.fX = 130;
	uidesc.tUIInfo.fY = 777;
	uidesc.tUIInfo.fCX = 110;
	uidesc.tUIInfo.fCY = 110;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
		return E_FAIL;


	//맨마지막
	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 150;
	//uidesc.tUIInfo.fY = 812;
	//uidesc.tUIInfo.fCX = 80;
	//uidesc.tUIInfo.fCY = 80;
	//uidesc.kind = 3;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;


	//옆에 추가 잔챙이
	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 70;
	//uidesc.tUIInfo.fY = 802;
	//uidesc.tUIInfo.fCX = 40;
	//uidesc.tUIInfo.fCY = 40;
	//uidesc.kind = 2;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;

	//ZeroMemory(&uidesc, sizeof(CUI_Smash::UISMDESC));

	//uidesc.tUIInfo.fX = 55;
	//uidesc.tUIInfo.fY = 824;
	//uidesc.tUIInfo.fCX = 40;
	//uidesc.tUIInfo.fCY = 40;
	//uidesc.kind = 3;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Smash_UI"), &uidesc)))
	//	return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLevel_Tutorial::Ready_Layer_SmashEffect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUI_SmashEffect::UIINFO uidesc;

	uidesc.fX = 190;
	uidesc.fY = 520;
	uidesc.fCX = 10;
	uidesc.fCY = 10;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 150;
	uidesc.fY = 470;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 110;
	uidesc.fY = 520;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
	uidesc.fX = 150;
	uidesc.fY = 570;
	uidesc.fCX = 140;
	uidesc.fCY = 150;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Line(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	UILine::LINEDESC uidesc;
	uidesc.uidesc.fX = 1788;//1748g_iWinCX / 2 + 700;
	uidesc.uidesc.fY = 247;//g_iWinCY / 2 - 250;
	uidesc.uidesc.fCX = 300;
	uidesc.uidesc.fCY = 100;
	uidesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
		return E_FAIL;


	/*ZeroMemory(&uidesc, sizeof(UILine::UIINFO));


	uidesc.fX = 1598;
	uidesc.fY = 807;
	uidesc.fCX = 100;
	uidesc.fCY = 40;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(UILine::UIINFO));
	uidesc.fX = 1098;
	uidesc.fY = 827;
	uidesc.fCX = 100;
	uidesc.fCY = 40;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;



	ZeroMemory(&uidesc, sizeof(UILine::UIINFO));
	uidesc.fX = 1598;
	uidesc.fY = 847;
	uidesc.fCX = 100;
	uidesc.fCY = 40;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Line"), &uidesc)))
	return E_FAIL;
	*/



	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Tutorial::Ready_Layer_NumberFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	////Hit
	//CUINumber::UINUMDESC uidesc;
	//uidesc.tUIInfo.fX = 1757; // g_iWinCX / 2 + 470;
	//uidesc.tUIInfo.fY = 214;//g_iWinCY / 2 - 285; //235 50씩 더해주자.
	//uidesc.tUIInfo.fCX = 300;
	//uidesc.tUIInfo.fCY = 100;
	//uidesc.kind = 0;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
	//	return E_FAIL;




	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_DamageFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	uidesc.tUIInfo.fX = 1730;//g_iWinCX / 2 + 440;
	uidesc.tUIInfo.fY = 278;//g_iWinCY / 2 - 220;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 50;
	uidesc.kind = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;





	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_HPFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	uidesc.tUIInfo.fX = 1870;
	uidesc.tUIInfo.fY = 600;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	uidesc.tUIInfo.fX = 1870;
	uidesc.tUIInfo.fY = 800;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	//ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	//uidesc.tUIInfo.fX = g_iWinCX / 2 + 590;
	//uidesc.tUIInfo.fY = g_iWinCY / 2 + 80;
	//uidesc.tUIInfo.fCX = 20;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 3;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
	//	return E_FAIL;



	//ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));


	//uidesc.tUIInfo.fX = g_iWinCX / 2 + 590;
	//uidesc.tUIInfo.fY = g_iWinCY / 2 + 140;
	//uidesc.tUIInfo.fCX = 20;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 3;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_StatusFont(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;
	//밑에 써있는 거.. 끝에서 부터 썼다구..
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 840;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;

	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 830;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 820;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 20;
	uidesc.tUIInfo.fCY = 20;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_StatusFont2(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//Hit
	CUINumber::UINUMDESC uidesc;


	//앞에 있는 좀 더 큰 글씨 를 끝에서 부터 썼다구 
	//밑에 써있는 거.. 끝에서 부터 썼다구..
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 795;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	//밑에 써있는 거..

	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 780;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 765;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 35;
	uidesc.tUIInfo.fCY = 35;
	uidesc.kind = 3;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;




	// 그 작대기
	ZeroMemory(&uidesc, sizeof(CUINumber::UINUMDESC));
	uidesc.tUIInfo.fX = g_iWinCX / 2 + 814;
	uidesc.tUIInfo.fY = g_iWinCY / 2 - 190;
	uidesc.tUIInfo.fCX = 30;
	uidesc.tUIInfo.fCY = 30;
	uidesc.kind = 4;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_NumberFont"), &uidesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_NameUI(const _tchar * pLayerTag)
{
	//
	//Safe_AddRef(pGameInstance);
	//CUIName::UINAMEDESC uidesc;

	//CUI_Manager* pUIInstance = CUI_Manager::GetInstance();
	//CGameObject* test = pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_ALPHEN);

	////어쩔수 없어 지금은 그냥 이렇게 하자.

	//uidesc.tUIInfo.fX = 1670;
	//uidesc.tUIInfo.fY = 541;
	//uidesc.tUIInfo.fCX = 50;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 0;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//	return E_FAIL;



	//ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	//uidesc.tUIInfo.fX = 1670;
	//uidesc.tUIInfo.fY = 613;
	//uidesc.tUIInfo.fCX = 50;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 1;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//	return E_FAIL;

	//if (nullptr != test)
	//{
	//	uidesc.tUIInfo.fX = 1670;
	//	uidesc.tUIInfo.fY = 541;
	//	uidesc.tUIInfo.fCX = 50;
	//	uidesc.tUIInfo.fCY = 20;
	//	uidesc.kind = 0;
	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//		return E_FAIL;
	//}
	//if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_SION))
	//{
	//	ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	//	uidesc.tUIInfo.fX = 1610;
	//	uidesc.tUIInfo.fY = 613;
	//	uidesc.tUIInfo.fCX = 50;
	//	uidesc.tUIInfo.fCY = 20;
	//	uidesc.kind = 1;

	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//		return E_FAIL;
	//}
	///*if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_RINWELL))
	//{

	//ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	//uidesc.tUIInfo.fX = 1752;
	//uidesc.tUIInfo.fY = 674;
	//uidesc.tUIInfo.fCX = 50;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 2;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//return E_FAIL;

	//}	if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYRE_KAKURA))
	//{

	//ZeroMemory(&uidesc, sizeof(CUIName::UINAMEDESC));
	//uidesc.tUIInfo.fX = 1752;
	//uidesc.tUIInfo.fY = 737;
	//uidesc.tUIInfo.fCX = 50;
	//uidesc.tUIInfo.fCY = 20;
	//uidesc.kind = 3;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Name"), &uidesc)))
	//return E_FAIL;

	//}*/

	////나머지 애들.



	
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	//Safe_Release(pGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD3))
	{

		//SmashUI alphen
		CUI_SmashEffect::UIINFO uidesc;

		uidesc.fX = 190;
		uidesc.fY = 712;
		uidesc.fCX = 10;
		uidesc.fCY = 10;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return E_FAIL;

	}
	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD4))
	{
		CUI_SmashEffect::UIINFO uidesc;
		ZeroMemory(&uidesc, sizeof(CUI_SmashEffect::UIINFO));
		uidesc.fX = 130;
		uidesc.fY = 777;
		uidesc.fCX = 140;
		uidesc.fCY = 150;
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_SmashEffectUI"), &uidesc)))
			return E_FAIL;
	}

	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_StatusBallUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_StatusBall"))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_StatusUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	
	CUIStatus::STATUSUIDESC uidesc;


	//앞에 있는 좀 더 큰 글씨 를 끝에서 부터 썼다구 
	//밑에 써있는 거.. 끝에서 부터 썼다구..
	uidesc.uidesc.fX = 1855;
	uidesc.uidesc.fY = g_iWinCY / 2 - 88;
	uidesc.uidesc.fCX = 104;
	uidesc.uidesc.fCY = 102;
	uidesc.kind = 0;


	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Status"), &uidesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_SmashNum(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	SmashNum::UISMNUMDESC uidesc;

	uidesc.tUIInfo.fX = 183;
	uidesc.tUIInfo.fY = 760;//502;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//2
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 170;
	uidesc.tUIInfo.fY = 772;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;

	//3
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 206;
	uidesc.tUIInfo.fY = 772;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;
	uidesc.kind = 2;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;
	//4  
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 191;
	uidesc.tUIInfo.fY = 786;
	uidesc.tUIInfo.fCX = 40;
	uidesc.tUIInfo.fCY = 40;

	uidesc.kind = 3;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;

	//CP
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1806;
	uidesc.tUIInfo.fY = 446;
	uidesc.tUIInfo.fCX = 50;
	uidesc.tUIInfo.fCY = 50;

	uidesc.kind = 4;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;
	//R
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1756;
	uidesc.tUIInfo.fY = 866;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 5;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//E
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1724;
	uidesc.tUIInfo.fY = 893;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 6;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//F
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1756;
	uidesc.tUIInfo.fY = 921;
	uidesc.tUIInfo.fCX = 70;
	uidesc.tUIInfo.fCY = 70;

	uidesc.kind = 7;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;


	//그 위 화살표
	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));
	uidesc.tUIInfo.fX = 1111;
	uidesc.tUIInfo.fY = 578;
	uidesc.tUIInfo.fCX = 150;
	uidesc.tUIInfo.fCY = 150;

	uidesc.kind = 8;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"), &uidesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_AlphaTexture(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	SmashNum::UISMNUMDESC uidesc;
	uidesc.tUIInfo.fX = 0;
	uidesc.tUIInfo.fY = 488;//338;
	uidesc.tUIInfo.fCX = 200;
	uidesc.tUIInfo.fCY = 70;
	uidesc.kind = 9;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"),&uidesc)))
		return E_FAIL;


	ZeroMemory(&uidesc, sizeof(SmashNum::UISMNUMDESC));

	uidesc.tUIInfo.fX = 0;
	uidesc.tUIInfo.fY = 540;//390;
	uidesc.tUIInfo.fCX = 200;
	uidesc.tUIInfo.fCY = 60;
	uidesc.kind = 10;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UISmashNum"),&uidesc)))
		return E_FAIL;
	Safe_Release(pGameInstance);




	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_AutoTargetUI(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Tutorial::Ready_Layer_AllAboutHp(const _tchar * pLayerTag)
{
	if (FAILED(Ready_Layer_BarUI(L"Layer_BattleUI")))
		return E_FAIL;
	


	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_MonsterHp(const _tchar * pLayerTag)
{

	//나중엔 몬스터한테 줘야함. 

	//if (FAILED(Ready_Layer_BarUI(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HPRed(L"Layer_BattleUI")))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_HP(L"Layer_BattleUI")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_BarUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIInstance = CUI_Manager::GetInstance();
	//시온거만 만들어놓으면 됨.
	UIINFO uidesc;
	//uidesc.fX = g_iWinCX / 2 + 800;
	//uidesc.fY = 570;
	//uidesc.fCX = 280;
	//uidesc.fCY = 18;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
	//	return E_FAIL;
	ZeroMemory(&uidesc, sizeof(UIINFO));
	uidesc.fX = g_iWinCX / 2 + 800;
	uidesc.fY = 630;
	uidesc.fCX = 280;
	uidesc.fCY = 18;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
		return E_FAIL;


	//if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_ALPHEN))
	//{

	//	uidesc.fX = g_iWinCX / 2 + 800;
	//	uidesc.fY = 570;
	//	uidesc.fCX = 280;
	//	uidesc.fCY = 18;

	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
	//		return E_FAIL;

	//}
	//if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_SION)) {
	//	ZeroMemory(&uidesc, sizeof(UIINFO));

	//	uidesc.fX = g_iWinCX / 2 + 800;
	//	uidesc.fY = 630;
	//	uidesc.fCX = 280;
	//	uidesc.fCY = 18;


	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
	//		return E_FAIL;

	//}
	//if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYER_RINWELL)) {
	//	ZeroMemory(&uidesc, sizeof(UIINFO));
	//	uidesc.fX = g_iWinCX / 2 + 800;
	//	uidesc.fY = 690;
	//	uidesc.fCX = 280;
	//	uidesc.fCY = 18;

	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
	//		return E_FAIL;

	//}
	//if (nullptr != pUIInstance->GetPlayer(CUI_Manager::UIPLAYER::PLAYRE_KAKURA))
	//{
	//	ZeroMemory(&uidesc, sizeof(UIINFO));
	//	uidesc.fX = g_iWinCX / 2 + 800;
	//	uidesc.fY = 750;
	//	uidesc.fCX = 280;
	//	uidesc.fCY = 18;


	//	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_BarUI"), &uidesc)))
	//		return E_FAIL;


	//}


	return S_OK;

	Safe_Release(pGameInstance);

	Safe_Release(pUIInstance);

}

HRESULT CLevel_Tutorial::Ready_Layer_InfoFadeUI(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	UIINFO uidesc;
	uidesc.fX = g_iWinCX / 2;
	uidesc.fY = g_iWinCX / 2 - 400;
	uidesc.fCX = 400;
	uidesc.fCY = 300;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_InfoFadeUI"), &uidesc)))
		return E_FAIL;


	return S_OK;

	Safe_Release(pGameInstance);

}

HRESULT CLevel_Tutorial::Ready_Layer_LineMsgUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LineMsgUI::MSGLINEUIDESC uidesc;
	uidesc.uidesc.fX = g_iWinCX / 2;
	uidesc.uidesc.fY = g_iWinCX / 2 - 700;
	uidesc.uidesc.fCX = 700;
	uidesc.uidesc.fCY = 250;
	uidesc.sprite = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_LineMsgUI"), &uidesc)))
		return E_FAIL;


	return S_OK;

	Safe_Release(pGameInstance);
}

CLevel_Tutorial * CLevel_Tutorial::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Tutorial*	pInstance = new CLevel_Tutorial(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Tutorial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLevel_Tutorial::Ready_Layer_Boar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Boar")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Practice(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vPos = XMVectorSet(5.f, 0.f, 5.f,1.f);
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Practice"),&vPos))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLevel_Tutorial::Ready_Layer_Punisher(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vPos = XMVectorSet(10.f, 0.f, 0.f, 1.f);
	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Punisher"), &vPos))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CLevel_Tutorial::Ready_Layer_FireAvatar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_FireAvatar")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Mantis(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	CEnemy::EnemyDesc Desc;

	Desc.vPos = XMVectorSet(0.f, 0.f, 5.f, 1.f);
	Desc.eMapType = MAP_BATTLE02;

	CEnemy* pEnemy = dynamic_cast<CEnemy*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Mantis"), &Desc));

	if (nullptr == pEnemy)
		return E_FAIL;

	// 튜토리얼 맵에 네비를 타게 함.
	CNavigation* pNavigation = pEnemy->SetUp_Navigation(TEXT("Prototype_Component_Navigation_Map_Tutorial"));

	// 특정 위치로 움직이게 설정
	CTransform* pEnemyTransform = dynamic_cast<CTransform*>(pEnemy->Get_Component(TEXT("Com_Transform")));
	pEnemyTransform->Move(-130.25f, -6.7f, -22.5f);

	// 현재 있는 셀 정보 수정
	pNavigation->Find_My_Cell(DirectX::XMVectorSet(-130.25f, -6.7f, -22.5f, 1.f));

	// y값 수정
	pEnemyTransform->Move(-130.25f, pEnemyTransform->Get_Height(pNavigation), -22.5f);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Tutorial_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//엠블럼 쉐이더 패스 조금 이상함.
	//if (FAILED(Ready_Layer_EmblemUI(L"Layer_TutorialUI")))
	//	return E_FAIL;

	//이를테면 채집.. 뭐이런거
	if (FAILED(Ready_Layer_InfoFadeUI(L"Layer_TutorialUI")))
		return E_FAIL;

	//Tutorial Msg 추가.
	TutorialMsg::TMSGDESC uidesc;
	uidesc.uidesc.fX = g_iWinCX / 2;
	uidesc.uidesc.fY = g_iWinCX / 2;
	uidesc.uidesc.fCX = 200;
	uidesc.uidesc.fCY = 50;
	uidesc.kind = 0;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutorialMsg"), &uidesc)))
		return E_FAIL;

	//Line
	UILine::LINEDESC linedesc;
	linedesc.uidesc.fX = 0;
	linedesc.uidesc.fY = g_iWinCX / 2 - 700;
	linedesc.uidesc.fCX = 1920;
	linedesc.uidesc.fCY = 80;
	linedesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
		return E_FAIL;


	//위에 있는 TutoLine위에 써질 전투 메시지 이를테면 멘티스와의 전투를 준비하세요.
	//5 - world탐험 6- 발셉과의 7 - 멘티스와의 전투 
	//그냥 새클래스 만드세요
	//TutoLineMsg
	TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
	TutoLineMsgDesc.uidesc.fCX = 500;
	TutoLineMsgDesc.uidesc.fCY = 70;
	TutoLineMsgDesc.sprite = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
		return E_FAIL;


	////LineSprite
	LineSprite::MINIMAPDESC LineSpritedesc;
	LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
	LineSpritedesc.uidesc.fCX = 400;
	LineSpritedesc.uidesc.fCY = 400;
	LineSpritedesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
		return E_FAIL;

	//

	//미니맵
	MiniMap::MINIMAPDESC minidesc;
	minidesc.uidesc.fX = g_iWinCX - 300;
	minidesc.uidesc.fY = 300;
	minidesc.uidesc.fCX = 400;
	minidesc.uidesc.fCY = 400;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_MiniMap"), &minidesc)))
		return E_FAIL;


	//미니맵 veow
	MIniMapView::MINIMAPDESC MIniMapViewDesc;
	MIniMapViewDesc.uidesc.fX = g_iWinCX - 300;
	MIniMapViewDesc.uidesc.fY = 300;
	MIniMapViewDesc.uidesc.fCX = 50;
	MIniMapViewDesc.uidesc.fCY = 50;
	MIniMapViewDesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_TutorialUI", TEXT("Prototype_GameObject_MiniMapView"), &MIniMapViewDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Tutorial::Ready_Layer_Battle_UI()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_isBattle = true;
	////Flag를 여기다가 만들예정이긴 한데.. 
	UIFlag::UIFLAGDESC flagdesc;
	flagdesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_Flag", TEXT("Prototype_GameObject_Flag"), &flagdesc)))
		return E_FAIL;


	if (FAILED(Ready_Layer_SmashUI()))
		return E_FAIL;

	//전투 설명 
	if (FAILED(Ready_Layer_ExUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_AllAboutHp(L"Layer_BattleUI")))
		return E_FAIL;

	//Line
	if (FAILED(Ready_Layer_Line(L"Layer_BattleUI")))
		return E_FAIL;


	if (FAILED(Ready_Layer_NumberFont(L"Layer_BattleUI")))
		return E_FAIL;

	//if (FAILED(Ready_Layer_DamageFont(L"Layer_BattleUI")))
	//	return E_FAIL;


	//if (FAILED(Ready_Layer_HPFont(L"Layer_BattleUI")))
	//	return E_FAIL;


	if (FAILED(Ready_Layer_NameUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_StatusBallUI(L"Layer_BattleUI")))
		return E_FAIL;

	if (FAILED(Ready_Layer_StatusUI(L"Layer_BattleUI")))
		return E_FAIL;

	//Line
	UILine::LINEDESC linedesc;
	linedesc.uidesc.fX = 0;
	linedesc.uidesc.fY = g_iWinCX / 2 - 700;
	linedesc.uidesc.fCX = 1920;
	linedesc.uidesc.fCY = 80;
	linedesc.kind = 1;
	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
		return E_FAIL;


	//위에 있는 TutoLine위에 써질 전투 메시지 이를테면 멘티스와의 전투를 준비하세요.
	//5 - world탐험 6- 발셉과의 7 - 멘티스와의 전투 
	//그냥 새클래스 만드세요

	//TutoLineMsg
	TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 750;
	TutoLineMsgDesc.uidesc.fCX = 600;
	TutoLineMsgDesc.uidesc.fCY = 70;
	TutoLineMsgDesc.sprite = 1;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
		return E_FAIL;


	////LineSprite
	LineSprite::MINIMAPDESC LineSpritedesc;
	LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 750;
	LineSpritedesc.uidesc.fCX = 500;
	LineSpritedesc.uidesc.fCY = 400;
	LineSpritedesc.kind = 0;

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
		return E_FAIL;

	//AutoTarget

	if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_AutoTarget"))))
		return E_FAIL;
	////Line
	//UILine::LINEDESC linedesc;
	//linedesc.uidesc.fX = 0;
	//linedesc.uidesc.fY = g_iWinCX / 2 - 500;
	//linedesc.uidesc.fCX = 1920;
	//linedesc.uidesc.fCY = 80;
	//linedesc.kind = 1;
	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_Line"), &linedesc)))
	//	return E_FAIL;

	////이건 그거임.. 전투시작전 멘티스 조심하라는 메시지
	////위에 있는 TutoLine위에 써질 전투 메시지 이를테면 멘티스와의 전투를 준비하세요.
	////5 - world탐험 6- 발셉과의 7 - 멘티스와의 전투 
	////그냥 새클래스 만드세요
	////TutoLineMsg
	//TutoLineMsg::MSGLINEUIDESC TutoLineMsgDesc;
	//TutoLineMsgDesc.uidesc.fX = g_iWinCX / 2;
	//TutoLineMsgDesc.uidesc.fY = g_iWinCX / 2 - 550;
	//TutoLineMsgDesc.uidesc.fCX = 400;
	//TutoLineMsgDesc.uidesc.fCY = 70;
	//TutoLineMsgDesc.sprite = 1;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_TutoLineMsg"), &TutoLineMsgDesc)))
	//	return E_FAIL;


	//////LineSprite
	//LineSprite::MINIMAPDESC LineSpritedesc;
	//LineSpritedesc.uidesc.fX = g_iWinCX / 2;
	//LineSpritedesc.uidesc.fY = g_iWinCX / 2 - 550;
	//LineSpritedesc.uidesc.fCX = 400;
	//LineSpritedesc.uidesc.fCY = 400;
	//LineSpritedesc.kind = 0;

	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineSprite"), &LineSpritedesc)))
	//	return E_FAIL;


	//이건 진짜 num들,..
	if (FAILED(Ready_Layer_SmashNum(L"Layer_BattleUI")))
		return E_FAIL;

	////이건아마 마신검 텍스쳐 일단은 Tick에서 생성.
	//if (FAILED(Ready_Layer_AlphaTexture(L"Layer_BattleUI")))
	//	return E_FAIL;


	//name..
	if (FAILED(Ready_Layer_NameUI(L"Layer_BattleUI")))
		return E_FAIL;



	////hp생성 가즈아
	if (FAILED(Ready_Layer_AllAboutHp(L"Layer_BattleUI")))
		return E_FAIL;



	////glowUI 생성
	//UIGlow::UIINFO uidesc;
	//uidesc.fX = g_iWinCX / 2;
	//uidesc.fY = g_iWinCX / 2 - 750;
	//uidesc.fCX = 500;
	//uidesc.fCY = 400;


	//if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_UIGlow"), &LineSpritedesc)))
	//	return E_FAIL;


	////Line

	//


	//

	//BillBoardUI
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_UIBillBoard"))))
	//	return E_FAIL;





	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Inventory_UI()
{
	return E_NOTIMPL;
}

HRESULT CLevel_Tutorial::Ready_Layer_Inventory_Itme_UI()
{
	return E_NOTIMPL;
}


void CLevel_Tutorial::Free()
{
	__super::Free();

	Safe_Release(m_pBattle_Manager);
}

HRESULT CLevel_Tutorial::Ready_Fonts()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance)
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan40"), TEXT("../Bin/Resources/Fonts/Javanese20.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan50"), TEXT("../Bin/Resources/Fonts/Javanese50.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_Javan10"), TEXT("../Bin/Resources/Fonts/Javanese10.spritefont"))))
	{


		MSG_BOX(TEXT("Failed to Load Font"));
		return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Map(const char * pModelFilePath, const char * pModelFileName, _vector vCenterPos)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	TCHAR szCurPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFullPath, (int)strlen(szFullPath), szCurPath, MAX_PATH);

	// 파일을 연다.
	HANDLE		hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	CMapObject::MAPOBJDesc Desc;

	while (true)
	{
		// 문자열 메모리의 크기를 읽음
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		// 읽어온 메모리의 크기만큼 문자열을 할당하고, 해당 문자열을 읽어옴
		TCHAR*	pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

		// 잘못됬을 경우 해당 메모리 삭제 및 멈추기
		if (0 == dwByte)
		{
			delete[]pName;
			break;
		}

		// Desc에 정보 넣기
		Desc.pPrototype_ObjectName = pName;

		// Collider 정보 설정
		Desc.fColliderSize = _float3(10.f, 10.f, 10.f);

		// 행렬 정보를 설정
		ReadFile(hFile, &Desc.TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);

		_float4x4 matWorld4x4;
		XMStoreFloat4x4(&matWorld4x4, XMMatrixIdentity());

		_float3 fPos;
		XMStoreFloat3(&fPos, vCenterPos);
		memcpy(&matWorld4x4.m[CTransform::STATE_POSITION], &fPos, sizeof(_float3));

		// 파일에서 읽은 값과 인자로 받은 월드 위치를 곱하여 오브젝트들을 해당 위치로 이동시킨다.
		XMStoreFloat4x4(&Desc.TransformMatrix, XMMatrixMultiply(XMLoadFloat4x4(&Desc.TransformMatrix), XMLoadFloat4x4(&matWorld4x4)));



		// 맵 여부 확인
		ReadFile(hFile, &Desc.m_bMap, sizeof(_bool), &dwByte, nullptr);

		Desc.eLevel = LEVEL_TUTORIAL;

		if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02")) ||
			!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_02_NaviMap")) ||
			!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04")) ||
			!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_Map_GrandYork_Battle_04_NaviMap"))
			) {
			Desc.eLevel = LEVEL_STATIC;
		}

		// 해당 오브젝트의 이름이 WarpGate 일 경우
		if (!_tcscmp(Desc.pPrototype_ObjectName, TEXT("Prototype_Component_Model_WarpGate"))) {
			static _uint iWayPointNumber = 0;

			// WayPoint Desc값을 채운다.
			CWayPoint::WayPoint_DESC WayPointDesc;
			WayPointDesc.iPointNumber = iWayPointNumber++;

			// 위치 설정
			memcpy(&WayPointDesc.fPos, &Desc.TransformMatrix.m[CTransform::STATE_POSITION][0], sizeof(_float3));

			// 방향 설정
			memcpy(&WayPointDesc.fDir, &Desc.TransformMatrix.m[CTransform::STATE_LOOK][0], sizeof(_float3));

			CWayPoint* pWayPoint = dynamic_cast<CWayPoint*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, LAYER_MAPTOOL_WayPoints, TEXT("Prototype_GameObject_WayPoint"), &WayPointDesc));
			m_vWayPoints.push_back(pWayPoint);
		}
		else {
			// 맵 오브젝트 생성
			CMapObject* pMapObject = dynamic_cast<CMapObject*>(pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, LAYER_MAPTOOL_PlacedObjects, TEXT("Prototype_GameObject_MapObject"), &Desc));
			m_vMapObject.push_back(pMapObject);
		}
	}

	// 파일을 닫는다.
	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Map_Battle02(_vector vCenterPos)
{
	XMStoreFloat4x4(&matBattle04World, XMMatrixIdentity());

	_float3 fPos;
	XMStoreFloat3(&fPos, vCenterPos);
	memcpy(&matBattle04World.m[CTransform::STATE_POSITION], &fPos, sizeof(_float3));

	return Ready_Map("../Bin/Resources/Dat/Stage08/", "MeshList.dat", vCenterPos);
}

HRESULT CLevel_Tutorial::Ready_Map_Battle04(_vector vCenterPos)
{
	XMStoreFloat4x4(&matBattle02World, XMMatrixIdentity());

	_float3 fPos;
	XMStoreFloat3(&fPos, vCenterPos);
	memcpy(&matBattle02World.m[CTransform::STATE_POSITION], &fPos, sizeof(_float3));

	return Ready_Map("../Bin/Resources/Dat/Stage09/", "MeshList.dat", vCenterPos);
}


