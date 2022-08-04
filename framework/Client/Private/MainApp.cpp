#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
//#ifdef _DEBUG
//#include "ImGui_Manager.h"
//#endif // _DEBUG

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

	if (FAILED(Open_Level(LEVEL_LOGO)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Open_Level");
		return E_FAIL;
	}
#ifdef _DEBUG
	// 콘솔 디버그 창 출력
	//if (FAILED(m_pGameInstance->Init_Console_Debug(true)))
	//{
	//	MSG_BOX(L"Failed To CMainApp : NativeConstruct : m_pGameInstance->Init_Console_Debug");
	//	return E_FAIL;
	//}

	//if (FAILED(Init_Imgui()))
	//{
	//	MSG_BOX(L"Failed To CMainApp : NativeConstruct : Init_Imgui");
	//	return E_FAIL;
	//}
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
		wsprintf(m_szFPS, TEXT("FPS: %d"), m_iNumRender);
		m_iNumRender = 0;
		m_TimeAcc = 0.0;
	}

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont

	if (FAILED(m_pGameInstance->Render_Fonts(TEXT("Font_MS"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
#endif // _DEBUG

//#ifdef _DEBUG
//	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
//	//pImgui->Render();		// 여기 주석처리하면 IMGUI꺼진다
//	RELEASE_INSTANCE(CImGUI_Manager);
//#endif //_DEBUG

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
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pDeviceContext))))
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

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Default%d.dds"), 2))))
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

//#ifdef _DEBUG
//HRESULT CMainApp::Init_Imgui()
//{
//	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
//
//	pImgui->InitWin32(g_hWnd);
//	pImgui->Init_ImGui(&m_pDevice, &m_pDeviceContext);
//
//	RELEASE_INSTANCE(CImGUI_Manager);
//	return S_OK;
//}
//#endif //Debug

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
}

