#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEBUTTONSTATE { DIMB_LBUTTON, DIMB_RBUTTON, DIMB_WHEEL, DIMB_END };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;


public:
	_char Get_DIKeyState(_uchar byKeyID) {
		return m_KeyBoardState[byKeyID];
	}

	_char Get_DIMButtonState(MOUSEBUTTONSTATE eDIMBState) {
		return m_MouseState.rgbButtons[eDIMBState];
	}


	_long Get_DIMMoveState(MOUSEMOVESTATE eDIMMState) {
		return ((_long*)&m_MouseState)[eDIMMState];
	}


public:
	_bool      Key_Pressing(_uchar byKeyID);
	_bool      Key_Down(_uchar byKeyID);
	_bool      Key_Up(_uchar byKeyID);

	/* For. Mouse Button_Input */
	_bool      Button_Pressing(MOUSEBUTTONSTATE eDIMBState);
	_bool      Button_Down(MOUSEBUTTONSTATE eDIMBState);
	_bool      Button_Up(MOUSEBUTTONSTATE eDIMBState);



public:
	HRESULT InitDevice(HINSTANCE hInst, HWND hWnd);
	void SetUp_InputDeviceState();


private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_char				m_KeyBoardState[256];	
	DIMOUSESTATE		m_MouseState;
	_bool               m_bKeyState[0xff];	//0xff
	_bool               m_bButtonState[4];

public:
	virtual void Free() override;
};

END