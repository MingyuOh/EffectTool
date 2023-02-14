#include "Device.h"
ID3D11Device* g_pd3dDevice = nullptr;

//--------------------------------------------------------------------------------------
// ID3D11Device 인터페이스를 생성한다.
//--------------------------------------------------------------------------------------
HRESULT	Device::CreateDevice()
{
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	hr = D3D11CreateDevice(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext);

	if (FAILED(hr))
		return hr;
	return hr;
}

//--------------------------------------------------------------------------------------
// DXGIFactory 인터페이스를 생성한다.
//--------------------------------------------------------------------------------------
HRESULT	Device::CreateGlFactory()
{
	HRESULT hr = S_OK;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory));

	if (FAILED(hr))
		return hr;
	return hr;
}

//--------------------------------------------------------------------------------------
// DXGIFactory 인터페이스로부터 IDXGISwapChain 인터페이스를 생성한다.
//--------------------------------------------------------------------------------------
HRESULT	Device::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == NULL)
		return S_FALSE;
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_SwapChainDesc.BufferCount = 1; // 백버퍼 개수
	m_SwapChainDesc.BufferDesc.Width = iWidth;
	m_SwapChainDesc.BufferDesc.Height = iHeight;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.OutputWindow = hWnd;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;
	m_SwapChainDesc.Windowed = IsFullScreen;
	//m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &m_SwapChainDesc, &m_pSwapChain)))
	{
		return hr;
	}

	/*if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}*/

	g_iWindowWidth = m_SwapChainDesc.BufferDesc.Width;
	g_iWindowHeight = m_SwapChainDesc.BufferDesc.Height;
	return hr;
}

HRESULT	Device::SetRenderTargetView()
{
	HRESULT hr;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		return hr;
	}

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_DefaultRT.m_pRenderTargetView);
	if (FAILED(hr))
	{
		pBackBuffer->Release();
		return hr;
	}

	m_pImmediateContext->OMSetRenderTargets(1, &m_DefaultRT.m_pRenderTargetView, NULL);
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	m_DefaultRT.m_pRenderTargetView->GetDesc(&rtvd);
	SAFE_RELEASE(pBackBuffer);
	return hr;
}

HRESULT	Device::SetDepthStencilView()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	hr = m_DefaultDS.CreateDepthStencilView(g_pd3dDevice, Desc.BufferDesc.Width, Desc.BufferDesc.Height);

	return hr;
}

HRESULT	Device::SetViewPort()
{
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return S_OK;
}

bool Device::CleanupDevice()
{
	m_DefaultDS.Release();
	m_DefaultRT.Release();

	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();
	
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pImmediateContext = nullptr;
	m_pGIFactory = nullptr;

	return true;
}

HRESULT	Device::InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{
	HRESULT hr;
	if (FAILED(hr = CreateGlFactory()))
	{
		return hr;
	}
	if (FAILED(hr = CreateDevice()))
	{
		return hr;
	}
	g_pd3dDevice = m_pd3dDevice;
	if (FAILED(hr = CreateSwapChain(hWnd, iWidth, iHeight, IsFullScreen)))
	{
		return hr;
	}
	if (FAILED(hr = SetRenderTargetView()))
	{
		return hr;
	}
	if (FAILED(hr = SetDepthStencilView()))
	{
		return hr;
	}
	if (FAILED(hr = SetViewPort()))
	{
		return hr;
	}
	return hr;
}

bool Device::ResizeDevice(UINT width, UINT height)
{
	HRESULT hr = S_OK;

	if (m_pImmediateContext == NULL)
		return false;

	//--------------------------------------------------------------------------------------
	// 랜더타켓과 깊이스텐실 버퍼를 해제한다.
	//--------------------------------------------------------------------------------------
	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	SAFE_RELEASE(m_DefaultRT.m_pRenderTargetView);
	SAFE_RELEASE(m_DefaultDS.m_pDepthStencilView);

	//--------------------------------------------------------------------------------------
	// 백버퍼의 크기를 조정한다.
	//--------------------------------------------------------------------------------------
	hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);
	if (SUCCEEDED(hr))
	{
		m_pSwapChain->GetDesc(&m_SwapChainDesc);
	}

	//--------------------------------------------------------------------------------------
	// 렌더 타겟 뷰 및 뎁스 스텐실 재생성
	//--------------------------------------------------------------------------------------
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}

	if (FAILED(m_DefaultDS.CreateDepthStencilView(g_pd3dDevice, width, height)))
	{
		return false;
	}

	//--------------------------------------------------------------------------------------
	// 뷰포트 재설정
	//--------------------------------------------------------------------------------------
	SetViewPort();

	//--------------------------------------------------------------------------------------
	// DX 리소스 생성
	//--------------------------------------------------------------------------------------
	//CreateDXResource();
	return true;
}

HRESULT	Device::CreateDXResource()
{
	return S_OK;
}

HRESULT	Device::DeleteDxResource()
{
	return S_OK;
}

Device::Device()
{
	m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	SAFE_ZERO(m_pd3dDevice);
	SAFE_ZERO(m_pImmediateContext);
	SAFE_ZERO(m_pSwapChain);
	SAFE_ZERO(m_pGIFactory);
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
}


Device::~Device()
{
}