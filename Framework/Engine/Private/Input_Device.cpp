#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	ZeroMemory(m_bButtonState, sizeof(m_bButtonState));
}

_bool CInput_Device::Key_Pressing(_uchar byKeyID)
{
	if (Get_DIKeyState(byKeyID) & 0x8000)
		return true;

	return false;
}

_bool CInput_Device::Key_Down(_uchar byKeyID)
{
	// 이전에 눌림이 없고, 현재 눌림이 있는 경우
	if (!m_bKeyState[byKeyID] && (Get_DIKeyState(byKeyID) & 0x8000))
	{
		m_bKeyState[byKeyID] = !m_bKeyState[byKeyID];
		return true;
	}

	if (m_bKeyState[byKeyID] && !(Get_DIKeyState(byKeyID) & 0x8000))
		m_bKeyState[byKeyID] = !m_bKeyState[byKeyID];

	return false;
}

_bool CInput_Device::Key_Up(_uchar byKeyID)
{
	// 이전에 눌림이 있고, 현재 눌리지 않았을 경우
	if (m_bKeyState[byKeyID] && !(Get_DIKeyState(byKeyID) & 0x8000))
	{
		m_bKeyState[byKeyID] = !m_bKeyState[byKeyID];
		return true;
	}

	if (!m_bKeyState[byKeyID] && (Get_DIKeyState(byKeyID) & 0x8000))
		m_bKeyState[byKeyID] = !m_bKeyState[byKeyID];

	return false;
}

_bool CInput_Device::Button_Pressing(MOUSEBUTTONSTATE eDIMBState)
{
	// 예외처리
	if (eDIMBState < DIMB_LBUTTON || eDIMBState >= DIMB_END)
		return false;

	if (Get_DIMButtonState(eDIMBState))
		return true;

	return false;
}

_bool CInput_Device::Button_Down(MOUSEBUTTONSTATE eDIMBState)
{
	// 예외처리
	if (eDIMBState < DIMB_LBUTTON || eDIMBState >= DIMB_END)
		return false;

	// 이전에 눌림이 없고, 현재 눌림이 있는 경우
	if (!m_bButtonState[eDIMBState] && (Get_DIMButtonState(eDIMBState) & 0x8000))
	{
		m_bButtonState[eDIMBState] = !m_bButtonState[eDIMBState];
		return true;
	}

	if (m_bButtonState[eDIMBState] && !(Get_DIMButtonState(eDIMBState) & 0x8000))
		m_bButtonState[eDIMBState] = !m_bButtonState[eDIMBState];

	return false;
}

_bool CInput_Device::Button_Up(MOUSEBUTTONSTATE eDIMBState)
{
	// 예외처리
	if (eDIMBState < DIMB_LBUTTON || eDIMBState >= DIMB_END)
		return false;

	// 이전에 눌림이 있고, 현재 눌리지 않았을 경우
	if (m_bButtonState[eDIMBState] && !(Get_DIMButtonState(eDIMBState) & 0x8000))
	{
		m_bButtonState[eDIMBState] = !m_bButtonState[eDIMBState];
		return true;
	}

	if (!m_bButtonState[eDIMBState] && (Get_DIMButtonState(eDIMBState) & 0x8000))
		m_bButtonState[eDIMBState] = !m_bButtonState[eDIMBState];

	return false;
}

HRESULT CInput_Device::InitDevice(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();


	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return;

	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	
	m_pKeyBoard->GetDeviceState(256, m_KeyBoardState);

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);	
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);

	Safe_Release(m_pInputSDK);
}
