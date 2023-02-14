#pragma once
#include "Device.h"

class Window :public Device
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	RECT m_rtClient;
	RECT m_rtWindow;
	HBRUSH m_hOldRedBrush;
	UINT m_iWindowWidth;		// 클라이언트 영역 가로크기
	UINT m_iWindowHeight;		// 클라이언트 영역 세로크기
public:
	Window();
	virtual ~Window();
	virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool SetWindow(HINSTANCE hInstance, T_STR gameName, int iX = 0, int iY = 0, int iWidth = 800, int iHeight = 600);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
};