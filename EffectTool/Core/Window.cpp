#include "Window.h"

Window *	g_pWindow = nullptr;
HWND		g_hWnd;
HINSTANCE	g_hInstance;
UINT		g_iWindowWidth = 0;
UINT		g_iWindowHeight = 0;
bool		g_bActiveApp = true;

Window::Window()
{
	g_pWindow = this;
}

Window::~Window()
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			g_iWindowWidth = width;
			g_iWindowHeight = height;
			g_pWindow->ResizeDevice(width, height);
		}break;
		case WM_ACTIVATEAPP:
		{
			g_bActiveApp = (BOOL)wParam; //wParam에 TRUE가 들어오면 활성화.
		}break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}break;
	}

	LRESULT hRet = g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool Window::SetWindow(HINSTANCE hInstance, T_STR gameName, int iX, int iY, int iWidth, int iHeight)
{
	WNDCLASSEX wcex;
	m_hInstance = hInstance;
	g_hInstance = hInstance;

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.hCursor = NULL;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = m_hInstance;
	wcex.lpszClassName = L"GAME";
	wcex.lpszMenuName = NULL;

	if (RegisterClassEx(&wcex) == false)
	{
		cout << "윈도우 생성 실패!" << endl;
		return false;
	}

	// 내가 원하는 크기만큼 윈도우를 생성하게 해달라 라는 함수가 있다.
	RECT rt = { 0, 0, iWidth, iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"Game",
		gameName.c_str(),
		WS_OVERLAPPEDWINDOW,
		iX, iY,
		rt.right - rt.left, rt.bottom - rt.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);
	g_hWnd = m_hWnd;
	ShowWindow(m_hWnd, SW_SHOW);

	// 윈도우 크기 반환 윈도우 만들어지고 해야함
	GetClientRect(m_hWnd, &m_rtClient);
	GetWindowRect(m_hWnd, &m_rtWindow);

	// 현재 클라이언트 크기를 전역 윈도우 변수로 사용중
	/*m_iWindowWidth = m_rtWindow.right - m_rtWindow.left;
	m_iWindowHeight = m_rtWindow.bottom - m_rtWindow.top;*/
	m_iWindowWidth = m_rtClient.right - m_rtClient.left;
	m_iWindowHeight = m_rtClient.bottom - m_rtClient.top;
	g_iWindowWidth = m_iWindowWidth;
	g_iWindowHeight = m_iWindowHeight;
	return true;
}


bool Window::Init()
{
	return true;
}

bool Window::Frame()
{
	return true;
}

bool Window::Render()
{
	return true;
}

bool Window::Release()
{
	return true;
}