#include "Input.h"

#define KEYDOWN(key)	(((m_KeyState[key]) & 0x80) ? true : false)
#define KEYUP(key)		(((m_KeyState[key]) & 0x80) ? false : true)

bool Input::Init()
{
	memset(&m_KeyBeforeState, 0, sizeof(BYTE) * KEYSTATECOUNT);
	memset(&m_MouseState, 0, sizeof(BYTE) * 3);
	memset(&m_MouseBeforeState, 0, sizeof(BYTE) * 3);
	m_dwElements = 0;
	return true;
}

bool Input::Frame()
{
	if (g_bActiveApp == true)
	{
		GetCursorPos(&m_MousePos);
		ScreenToClient(g_hWnd, &m_MousePos);
		if (KeyProcess() == false || !MouseProcess() == false)
		{
			return false;
		}
	}
	return true;
}

bool Input::Render()
{
	return true;
}

bool Input::Release()
{
	if (m_pDi)
	{
		if (m_pDidevKey)
		{
			m_pDidevKey->Unacquire();
			SAFE_RELEASE(m_pDidevKey);
		}
		if (m_pDidevMouse)
		{
			m_pDidevMouse->Unacquire();
			SAFE_RELEASE(m_pDidevMouse);
		}
		SAFE_RELEASE(m_pDi);
	}
	return true;
}

bool Input::ResetDevice()
{
	Release();
	Init();
	return true;
}

bool Input::InitDirectInput(HINSTANCE hInst, HWND hWnd, bool keyboard, bool mouse)
{
	HRESULT hr;
	m_hWnd = hWnd;

	if (m_pDidevKey || m_pDidevMouse)
		return true;

	if (FAILED(hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDi, NULL)))
	{
		return false;
	}

	if (keyboard)
	{
		if (FAILED(m_pDi->CreateDevice(GUID_SysKeyboard, &m_pDidevKey, NULL)))
		{
			return false;
		}
		if (FAILED(m_pDidevKey->SetDataFormat(&c_dfDIKeyboard)))
		{
			return false;
		}
		if (FAILED(hr = m_pDidevKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
		{
			while (m_pDidevKey->Acquire() == DIERR_INPUTLOST);
			if (FAILED(m_pDidevKey->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY)))
			{
				//return false;
			}
		}
		while (m_pDidevKey->Acquire() == DIERR_INPUTLOST);
	}

	if (mouse)
	{
		if (FAILED(m_pDi->CreateDevice(GUID_SysMouse, &m_pDidevMouse, NULL)))
		{
			return false;
		}
		if (FAILED(m_pDidevMouse->SetDataFormat(&c_dfDIMouse)))
		{
			return false;
		}
		if (FAILED(hr = m_pDidevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			while (m_pDidevMouse->Acquire() == DIERR_INPUTLOST);
			if (FAILED(m_pDidevMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			{
				//return false;
			}
		}
		while (m_pDidevMouse->Acquire() == DIERR_INPUTLOST);
	}
	return true;
}

BYTE Input::KeyCheck(BYTE dwKey)
{
	BYTE sKey = m_KeyState[dwKey];
	if (sKey & 0x80)
	{
		if (m_KeyBeforeState[dwKey] == KEY_FREE)
			m_KeyBeforeState[dwKey] = KEY_PUSH;
		else
			m_KeyBeforeState[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_KeyBeforeState[dwKey] == KEY_PUSH || m_KeyBeforeState[dwKey] == KEY_HOLD)
			m_KeyBeforeState[dwKey] = KEY_UP;
		else
			m_KeyBeforeState[dwKey] = KEY_FREE;
	}
	return m_KeyBeforeState[dwKey];
}

bool Input::KeyProcess()
{
	ZeroMemory(m_KeyState, sizeof(BYTE)*KEYSTATECOUNT);
	if (!m_pDidevKey)
		return false;
	if (FAILED(m_pDidevKey->GetDeviceState(KEYSTATECOUNT, m_KeyState)))
	{
		while (m_pDidevKey->Acquire() == DIERR_INPUTLOST)
			m_pDidevKey->Acquire();
		return true;
	}
	return true;
}

bool Input::MouseProcess()
{
	ZeroMemory(&m_DIMouseState, sizeof(DIMOUSESTATE));
	if (!m_pDidevMouse)
		return false;

	if (FAILED(m_pDidevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
	{
		while (m_pDidevMouse->Acquire() == DIERR_INPUTLOST)
			m_pDidevMouse->Acquire();
		return true;
	}

	for (int iButton = 0; iButton < 3; iButton++)
	{
		m_MouseBeforeState[iButton] = m_DIMouseState.rgbButtons[iButton];
	}

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_MouseBeforeState[iButton] & 0x80)
		{
			if (m_MouseState[iButton] == KEY_FREE)
				m_MouseState[iButton] = KEY_PUSH;
			else
				m_MouseState[iButton] = KEY_HOLD;
		}
		else
		{
			if (m_MouseState[iButton] == KEY_PUSH || m_MouseState[iButton] == KEY_HOLD)
				m_MouseState[iButton] = KEY_UP;
			else
				m_MouseState[iButton] = KEY_FREE;
		}
	}
	return true;
}

void Input::PostProcess()
{
	memcpy(&m_KeyBeforeState, &m_KeyState, sizeof(BYTE) * KEYSTATECOUNT);
	memcpy(&m_MouseBeforeState, &m_MouseState, sizeof(DIMOUSESTATE));
}

void Input::DeviceAcquire()
{
	if (m_pDidevKey)
		m_pDidevKey->Acquire();
	if (m_pDidevMouse)
		m_pDidevMouse->Acquire();
}

void Input::DeviceUnacquire()
{
	if (m_pDidevKey)
		m_pDidevKey->Unacquire();
	if (m_pDidevMouse)
		m_pDidevMouse->Unacquire();
}

bool Input::IsKeyDown(DWORD dwKey)
{
	if (KEYDOWN(dwKey)) 		
		return true;
	return false;
}

bool Input::IsKeyUP(DWORD dwKey)
{
	if (m_KeyBeforeState[dwKey] & 0x80)
	{
		if (KEYUP(dwKey))
			return true;
	}
}

void Input::SetAcquire(bool bActive)
{
	if (bActive) 
		DeviceAcquire();
	else		  
		DeviceUnacquire();
}

Input::Input()
{
	m_pDi = NULL;
	m_pDidevKey = NULL;
	m_pDidevMouse = NULL;
	m_hWnd = NULL;
	m_dwElements = 0;
}


Input::~Input()
{
}
