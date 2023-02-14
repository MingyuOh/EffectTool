#pragma once
#include "CoreSTD.h"
#include "DxRT.h"
using namespace DX;

class Device
{
public:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------
	ID3D11Device*				m_pd3dDevice;			// 디바이스 객체
	ID3D11DeviceContext*		m_pImmediateContext;	// 디바이스 컨텍스트 객체
	IDXGISwapChain*				m_pSwapChain;			// 스왑체인 객체
	IDXGIFactory*				m_pGIFactory;			// DXGI 객체

	D3D_DRIVER_TYPE				m_DriverType;			// 디바이스 타입 속성값
	D3D_FEATURE_LEVEL			m_FeatureLevel;			// Direct 3D 특성레벨 속성값
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;		// 스왑체인 속성값
	D3D11_VIEWPORT				m_ViewPort;				// 뷰 포트 속성값
	DxRT						m_DefaultRT;			// 메인 렌더타켓 뷰 객체
	DxDS						m_DefaultDS;			// 메인 뎁스-스텐실 객체
public:
	//--------------------------------------------------------------------------------------
	// 디바이스 및 스왑체인 생성 : 
	// 1. CreateDevice()
	// 2. CreateGIFactory()
	// 3. CreateSwapChain()
	// 4. SetRenderTargetView()
	// 5. SetViewPort()
	//--------------------------------------------------------------------------------------
	HRESULT				CreateDevice();
	HRESULT				CreateGlFactory();
	HRESULT				CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen);
	HRESULT				SetRenderTargetView();
	HRESULT				SetDepthStencilView();
	HRESULT				SetViewPort();
	bool				CleanupDevice();
	HRESULT				InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen = FALSE);

	//--------------------------------------------------------------------------------------
	// DX 리소스 생성/ 해제 
	//--------------------------------------------------------------------------------------
	virtual HRESULT		CreateDXResource();
	virtual HRESULT		DeleteDxResource();

	//--------------------------------------------------------------------------------------
	// 해상도가 변경되었을 때 백버퍼의 크기를 클라이언트 크기와 맞추기위해 호출되는 함수
	//--------------------------------------------------------------------------------------
	bool				ResizeDevice(UINT width, UINT height);
public:
	Device();
	virtual ~Device();
};

