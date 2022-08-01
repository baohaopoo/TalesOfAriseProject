#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Client)

class CImGUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGUI_Manager)
private:
	CImGUI_Manager();
	virtual ~CImGUI_Manager() = default;
public:
	HRESULT Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT StartFrame(void);
	HRESULT Set_Contents(void);
	HRESULT Render(void);
public:
	void InitWin32(HWND hWnd);
	LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Shutdown(void);
public:
	void Set_Camera(CCamera* pCamera);
	void Setting_Camera();
private:
	bool m_bImguiEnable = true;
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
private:
	CCamera* m_pCamera = nullptr;
public:
	void OnOffImgui(void) { m_bImguiEnable = !m_bImguiEnable; }
	bool isImguiEnable() { return m_bImguiEnable; }
public:
	virtual void Free() override;
};

END