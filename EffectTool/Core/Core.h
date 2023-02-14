#pragma once
#include "Window.h"
#include "CameraBackView.h"
#include "DirectWrite.h"
#include "Timer.h"
#include "Input.h"
#include "DirectionLine.h"
#include "SkyBox.h"
#include "DxState.h"

class Core :public Window
{
public:
	//--------------------------------------------------------------------------------------
	// 메인/디버그/백뷰 카메라
	//--------------------------------------------------------------------------------------
	Camera*				m_pMainCamera;
	Camera*				m_pDebugCamera;
	CameraBackView*		m_pBackViewCamera;
	//--------------------------------------------------------------------------------------
	// 디버그 디렉션 라인
	//--------------------------------------------------------------------------------------
	DirectionLine		m_DrLine;
	//--------------------------------------------------------------------------------------
	// Writer
	//--------------------------------------------------------------------------------------
	DirectWrite			m_Writer;
	//--------------------------------------------------------------------------------------
	// 타이머
	//--------------------------------------------------------------------------------------
	Timer				m_Timer;
	//--------------------------------------------------------------------------------------
	// 스카이 박스(배경)
	//--------------------------------------------------------------------------------------
	SkyBox				m_SkyObj;
	//--------------------------------------------------------------------------------------
	// 토폴로지 와이어 프레임 체크
	//--------------------------------------------------------------------------------------
	bool				m_bWireFrame;
	//--------------------------------------------------------------------------------------
	// 디버그 체크 변수
	//--------------------------------------------------------------------------------------
	bool				m_bDebug;
	bool				m_bCameraType;
	//--------------------------------------------------------------------------------------
	// 스카이박스 ON/OFF
	//--------------------------------------------------------------------------------------
	bool				m_bSkyBoxSwitch;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual int			WndProc(HWND, UINT, WPARAM, LPARAM);

	virtual bool		PreInit();
	virtual bool		PostInit();

	virtual bool		PreFrame();
	virtual bool		PostFrame();

	virtual	bool		PreRender();
	virtual	bool		PostRender();

	bool				GameInit();
	bool				GameFrame();
	bool				GameRender();
	bool				GameRelease();

	bool				Run();

	//--------------------------------------------------------------------------------------
	// DX 리소스 생성/ 해제 재정의 && 리소스 생성 해제
	//--------------------------------------------------------------------------------------
	HRESULT				CreateDXResource();
    HRESULT				DeleteDxResource();
	virtual HRESULT		CreateResource();
	virtual HRESULT		DeleteResource();
public:
	Core();
	virtual ~Core();
};

