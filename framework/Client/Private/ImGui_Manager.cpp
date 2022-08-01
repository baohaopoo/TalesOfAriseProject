#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Public\ImGUI_Manager.h"

IMPLEMENT_SINGLETON(CImGUI_Manager)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CImGUI_Manager::CImGUI_Manager()
{
}

HRESULT CImGUI_Manager::Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	ImGui_ImplDX11_Init(*ppDeviceOut, *ppDeviceContextOut);
	//m_pDevice = *ppDeviceOut;
	//m_pDeviceContext = *ppDeviceContextOut;

	//Safe_AddRef(m_pDevice);
	//Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CImGUI_Manager::StartFrame(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return S_OK;
}

HRESULT CImGUI_Manager::Set_Contents(void)
{
	static bool show_demo_window = true;
	static bool show_another_window = false;

	ImGui::SetNextWindowSize(ImVec2(600.f, 300.f));

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (ImGui::Begin("Tool"))
	{
	
	}

	if (ImGui::Button("Save"))
	{

	}


	ImGui::Text("Playerpos : {%f, %f, %f}",0.f, 0.f, 0.f);
	ImGui::End();

	return S_OK;
}

HRESULT CImGUI_Manager::Render(void)
{
	// Imgui render옵션이 켜져 있다면
	if (m_bImguiEnable)
	{
		StartFrame();
		Set_Contents();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return S_OK;
}

void CImGUI_Manager::InitWin32(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
}

LRESULT CImGUI_Manager::WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}

void CImGUI_Manager::Shutdown(void)
{
	Safe_Release(m_pCamera);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

void CImGUI_Manager::Set_Camera(CCamera* pCamera)
{
	m_pCamera = pCamera;
	Safe_AddRef(m_pCamera);
}

void CImGUI_Manager::Setting_Camera()
{
	if (nullptr == m_pCamera)
	{
		return;
	}
	CTransform* pTransform = (CTransform*)m_pCamera->Get_Component(L"Com_Transform");
	_float3 vCameraPosition;
	XMStoreFloat3(&vCameraPosition, pTransform->Get_State(CTransform::STATE_POSITION));
	ImGui::Text("Camera Pos : %.2f, %.2f, %.2f", vCameraPosition.x, vCameraPosition.y, vCameraPosition.z);
}

void CImGUI_Manager::Free()
{
	//Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}