#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"

//UI용 헤더
#include "BackGround.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "..\Public\UIBackground.h"
#include"..\Public\LobbyBtn.h"
#include"..\Public\MouseUI.h"
#include"..\Public\UIEmblem.h"
#include"..\Public\UIExplain.h"
#include"..\Public\UI_Smash.h"
#include"..\Public\UI_SmashEffect.h"
#include"..\Public\UILine.h"
#include"..\Public\UINumber.h"
#include"..\Public\SmashNum.h"
#include"..\Public\UIName.h"
#include"..\Public\UIStatus.h"
#include"..\Public\UIStatusBall.h"
#include"..\Public\uicharacter.h"
#include"..\Public\UIBar.h"
#include"..\Public\InfoFadeUI.h"
#include"..\Public\LineMsgUI.h"
#include"..\Public\TutorialMsg.h"
#include"..\Public\MiniMap.h"
#include"..\Public\TutoLineMsg.h"
#include"..\Public\LineSprite.h"
#include"..\Public\MiniMapView.h"
#include"..\Public\uiglow.h"
#include"..\Public\UIBillBoard.h"
#include "AutoTargetUI.h"
#include"MovingHP.h"
#include"UIFlag.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	/*D3D11_SAMPLER_DESC*/

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;

	/* 장치초기화 등. 엔진의 기능을 활용하기위한 기초 초기화 작업을 마친다. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : m_pGameInstance->Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(Ready_Fonts()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Ready_Fonts()");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Component()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Ready_Prototype_Component()");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Ready_Prototype_GameObject()");
		return E_FAIL;
	}


	ShowCursor(true);
	////Mouse 생성
	if (nullptr == m_pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"Layer_Mouse", L"Prototype_GameObject_Mouse"))
	{
		MSG_BOX(L"MouseUI 생성 터진다");
		return E_FAIL;
	}
	if (FAILED(Open_Level(LEVEL_LOGO)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Open_Level");
		return E_FAIL;
	}



#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Init_Console_Debug(true)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : m_pGameInstance->Init_Console_Debug");
		return E_FAIL;
	}

	if (FAILED(Init_Imgui()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Init_Imgui");
		return E_FAIL;
	}
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Tick : nullptr == m_pGameInstance");
		return;
	}

	m_pGameInstance->Tick_Engine(TimeDelta);

#ifdef _DEBUG	
	m_TimeAcc += TimeDelta;
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Render : nullptr == m_pGameInstance");
		return E_FAIL;
	}

	m_pGameInstance->Render_Begin(_float4(0.0f, 0.f, 1.f, 1.f));

	if (FAILED(m_pRenderer->Render_GameObjects()))
	{
		MSG_BOX(L"Failed To CMainApp : Render : m_pRenderer->Render_GameObjects");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

#ifdef _DEBUG
	++m_iNumRender;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
		m_iNumRender = 0;
		m_TimeAcc = 0.0;
	}

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont

	if (FAILED(m_pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
#endif // _DEBUG

#ifdef _DEBUG
	//CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	//pImgui->Render();		// 여기 주석처리하면 IMGUI꺼진다
	//RELEASE_INSTANCE(CImGUI_Manager);
#endif //_DEBUG

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel*			pLevel = nullptr;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
		break;

	default:
		pLevel = CLevel_Loading::Create(m_pDevice, m_pDeviceContext, (LEVEL)eLevelIndex);
		break;
	}

	if (FAILED(m_pGameInstance->Open_Level(eLevelIndex, pLevel)))
	{
		MSG_BOX(L"Failed To CMainApp : Open_Level : m_pGameInstance->Open_Level(eLevelIndex, pLevel)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (FAILED(Ready_Prototype_GameObject_UI()))
		return E_FAIL;
	
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_UI()
{

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGroundUI"),
		CUIBackground::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LobbyBtnUI"),
		LobbyBtn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse"),
		MouseUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Emblem 오브젝트

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EmblemUI"),
		UIEmblem::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//전투 설명 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExplainUI"),
		UIExplain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//smash 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Smash_UI"),
		CUI_Smash::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//smash 이펙트 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SmashEffectUI"),
		CUI_SmashEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Line오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Line"),
		UILine::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//Number font 오브젝트

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NumberFont"),
		CUINumber::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//Number font 오브젝트

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Name"),
		CUIName::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StatusBall"),
		CUIStatusBall::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Status"),
		CUIStatus::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UICharacter"),
		CUICharacter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UISmashNum"),
		SmashNum::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;





	//2차추가

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InfoFadeUI"),
		InfoFadeUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LineMsgUI"),
		LineMsgUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BarUI"),
		UIBar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//TutoLineMsg와 헷갈리지말기
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TutorialMsg"),
		TutorialMsg::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiniMap"),
		MiniMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TutoLineMsg"),
		TutoLineMsg::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LineSprite"),
		LineSprite::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiniMapView"),
		MIniMapView::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIGlow"),
		UIGlow::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBillBoard"),
		UIBillBoard::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MovingHP"),
		MovingHP::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AutoTarget"),
		AutoTargetUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flag"),
		UIFlag::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}
HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_UI"), CVIBufferUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	if (FAILED(Ready_Prototype_Texture()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture()
{
	if (FAILED(Ready_Prototype_Texture_UI()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_UI()
{
	//백그라운드 UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Logo%d.dds"), 5))))
		return E_FAIL;

	//lobby btn 클릭 effect 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BtnGlow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/btnglow2.dds")))))
		return E_FAIL;

	//Mouse 클릭 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseClick"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/cursorclick.dds")))))
		return E_FAIL;
	//Mouse
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/cursor.dds")))))
		return E_FAIL;

	//엠블럼 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emblem"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/emblem.dds")))))
		return E_FAIL;


	//smash 이펙트 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmashEffect"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/SmashEffect.dds")))))
		return E_FAIL;


	//SmashUI 텍스쳐 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmashUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Smash%d.dds"), 8))))
		return E_FAIL;

	//Line 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Line%d.dds"), 2))))
		return E_FAIL;

	//설명 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ExUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/exui%d.dds"), 3))))
		return E_FAIL;
	//glow텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GlowUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/dd%d.dds"), 2))))
		return E_FAIL;


	//여기까지는 확실히 됨.



	//font
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Font"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/font%d.dds"), 11))))
		return E_FAIL;
	//Status Ball
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StatusBall"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Status.dds")))))
		return E_FAIL;
	//.dds

	//image

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemCharacter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/image%d.dds"), 35))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmashNumTexture"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/SmashNum%d.dds"), 15))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AutoTargetUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/AutoTargetUI.dds")))))
		return E_FAIL;



	//name UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NameUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/name%d.dds"), 4))))
		return E_FAIL;


	//2차 추가
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InfoFadeUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Acquire4.dds")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LineMsgUI"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/MsgLineUI%d.dds"), 5))))
		return E_FAIL;


	//HBBar텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPBar"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/HPBackground%d.dds"), 8))))
		//CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/HPBar2.dds")))))
		return E_FAIL;

	////hp 자체 텍스쳐 (초록이)
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenHP"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/ShowHP%d.dds"), 1))))
		/*	CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/hp%d.dds",2)))))*/
		return E_FAIL;

	////hp 자체 텍스쳐 (빨강이)
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedHP"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/HPGROUP1.dds")))))
		/*	CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/hp%d.dds",2)))))*/
		return E_FAIL;


	//추가


	//Tutorial Msg
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TutorialMsg"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/TutorialMsg%d.dds"), 4))))
		return E_FAIL;

	//Tutorial Msg
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniMap"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/MiniMap.dds")))))
		return E_FAIL;


	//Tutoline Msg
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TutoLineMsg"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/TutoLineMsg%d.dds"), 3))))
		return E_FAIL;

	//스프라이트 가즈아
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TutoLineSprite"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/S%d.dds"), 30)))) //사실 이건 15지 있어.
		return E_FAIL;

	//미니맵 가즈아
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniMapView"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/minimapview%d.dds"), 2))))
		return E_FAIL;

	//깃발 플래그
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flag"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Flag.dds")))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pDeviceContext, TEXT("Font_MS"), TEXT("../Bin/Resources/Fonts/125.spritefont"))))
		return E_FAIL;

	// MS.spritefont
	return S_OK;
}

#ifdef _DEBUG
HRESULT CMainApp::Init_Imgui()
{
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);

	pImgui->InitWin32(g_hWnd);
	pImgui->Init_ImGui(&m_pDevice, &m_pDeviceContext);

	RELEASE_INSTANCE(CImGUI_Manager);
	return S_OK;
}
#endif //Debug

CMainApp * Client::CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

#ifdef _DEBUG
	CImGUI_Manager::GetInstance()->DestroyInstance();
#endif // DEBUG
}