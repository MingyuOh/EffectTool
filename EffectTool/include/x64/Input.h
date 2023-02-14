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
	// 입력 장치
	//--------------------------------------------------------------------------------------
	LPDIRECTINPUT8				m_pDi;
	LPDIRECTINPUTDEVICE8		m_pDidevKey;
	LPDIRECTINPUTDEVICE8		m_pDidevMouse;
	DIMOUSESTATE				m_DIMouseState;

	//--------------------------------------------------------------------------------------
	// 입력장치 상태
	//--------------------------------------------------------------------------------------
	BYTE						m_KeyState[KEYSTATECOUNT];
	BYTE						m_KeyBeforeState[KEYSTATECOUNT];
	BYTE						m_MouseState[3];
	BYTE						m_MouseBeforeState[3];
	DWORD						m_dwElements;
	POINT						m_MousePos;
	//--------------------------------------------------------------------------------------
	// 윈도우 핸들
	//--------------------------------------------------------------------------------------
	HWND						m_hWnd;
public:
	//--------------------------------------------------------------------------------------
	// 재정의 함수
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

