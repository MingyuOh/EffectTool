#pragma once
#include "CoreSTD.h"

enum KeyState {
	KEY_FREE = 0,
	KEY_UP	 = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3
};

class Input :public Singleton<Input>
{
private:
	friend class Singleton<Input>;
public:
	//--------------------------------------------------------------------------------------
	// �Է� ��ġ
	//--------------------------------------------------------------------------------------
	LPDIRECTINPUT8				m_pDi;
	LPDIRECTINPUTDEVICE8		m_pDidevKey;
	LPDIRECTINPUTDEVICE8		m_pDidevMouse;
	DIMOUSESTATE				m_DIMouseState;

	//--------------------------------------------------------------------------------------
	// �Է���ġ ����
	//--------------------------------------------------------------------------------------
	BYTE						m_KeyState[KEYSTATECOUNT];
	BYTE						m_KeyBeforeState[KEYSTATECOUNT];
	BYTE						m_MouseState[3];
	BYTE						m_MouseBeforeState[3];
	DWORD						m_dwElements;
	POINT						m_MousePos;
	//--------------------------------------------------------------------------------------
	// ������ �ڵ�
	//--------------------------------------------------------------------------------------
	HWND						m_hWnd;
public:
	//--------------------------------------------------------------------------------------
	// ������ �Լ�
	//--------------------------------------------------------------------------------------
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		ResetDevice();
	bool		ResetResource() { return true; };

	bool		InitDirectInput(HINSTANCE, HWND, bool, bool);
	BYTE		KeyCheck(BYTE dwKey);
	bool		KeyProcess();
	bool		MouseProcess();
	void		DeviceAcquire();
	void		DeviceUnacquire();
	bool		IsKeyDown(DWORD dwKey);
	bool		IsKeyUP(DWORD dwKey);
	void		PostProcess();
	void		SetAcquire(bool bActive);
public:
	Input();
	virtual ~Input();
};
#define I_Input Input::GetInstance()

