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
	ID3D11Device*				m_pd3dDevice;			// ����̽� ��ü
	ID3D11DeviceContext*		m_pImmediateContext;	// ����̽� ���ؽ�Ʈ ��ü
	IDXGISwapChain*				m_pSwapChain;			// ����ü�� ��ü
	IDXGIFactory*				m_pGIFactory;			// DXGI ��ü

	D3D_DRIVER_TYPE				m_DriverType;			// ����̽� Ÿ�� �Ӽ���
	D3D_FEATURE_LEVEL			m_FeatureLevel;			// Direct 3D Ư������ �Ӽ���
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;		// ����ü�� �Ӽ���
	D3D11_VIEWPORT				m_ViewPort;				// �� ��Ʈ �Ӽ���
	DxRT						m_DefaultRT;			// ���� ����Ÿ�� �� ��ü
	DxDS						m_DefaultDS;			// ���� ����-���ٽ� ��ü
public:
	//--------------------------------------------------------------------------------------
	// ����̽� �� ����ü�� ���� : 
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
	// DX ���ҽ� ����/ ���� 
	//--------------------------------------------------------------------------------------
	virtual HRESULT		CreateDXResource();
	virtual HRESULT		DeleteDxResource();

	//--------------------------------------------------------------------------------------
	// �ػ󵵰� ����Ǿ��� �� ������� ũ�⸦ Ŭ���̾�Ʈ ũ��� ���߱����� ȣ��Ǵ� �Լ�
	//--------------------------------------------------------------------------------------
	bool				ResizeDevice(UINT width, UINT height);
public:
	Device();
	virtual ~Device();
};

