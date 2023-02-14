#include "Core.h"

bool Core::PreInit()
{
	if (FAILED(InitDevice(m_hWnd, this->m_iWindowWidth, this->m_iWindowHeight, TRUE)))
	{
		MessageBox(0, _T("CreateDevice  ����"), _T("Fatal error"), MB_OK);
		return false;
	}

	CreateDXResource();

	// ����(�����)ī�޶� ����
	SAFE_NEW(m_pBackViewCamera, CameraBackView);
	SAFE_NEW(m_pDebugCamera, Camera);

	m_pBackViewCamera->Init();
	m_pDebugCamera->Init();

	m_pMainCamera = m_pDebugCamera;
	m_pMainCamera->SetProjTMatrix((float)m_SwapChainDesc.BufferDesc.Width, (float)m_SwapChainDesc.BufferDesc.Height, PI * 0.25f, NEAR_DISTANCE, FAR_DISTANCE);

	return true;
}

bool Core::Init()
{
	return true;
}

bool Core::PostInit()
{
	// ��ī�̹ڽ� ����(���)
	m_SkyObj.Init();
	m_SkyObj.Create(_T(""), L"../data/shader/SkyBoxVertexShader.vsh", L"../data/shader/SkyBoxPixelShader.psh", _T(""));
	m_SkyObj.SetTextureArray();

	// ��ǲ ����
	if (I_Input.InitDirectInput(m_hInstance, m_hWnd, true, true) == false)
	{
		return false;
	}

	// ��ǲ Init
	if (I_Input.Init() == false)
	{
		return false;
	}

	// Ÿ�̸� ����
	m_Timer.Init();

	// Writer ����
	IDXGISurface1* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (VOID**)&pBackBuffer);
	m_Writer.Set(pBackBuffer); 
	if (pBackBuffer != NULL)
		SAFE_RELEASE(pBackBuffer);

	// ������� ����
	m_DrLine.Init();
	m_DrLine.Create(_T(""), L"../data/shader/LineVertexShader.vsh", L"../data/shader/LinePixelShader.psh", _T(""));

	return true;
}

bool Core::GameInit()
{
	if (PreInit() == false)
	{
		return false;
	}
	if (Init() == false)
	{
		return false;
	}
	if (PostInit() == false)
	{
		return false;
	}
	return true;
}

bool Core::PreFrame()
{
	m_Timer.Frame();

	if (!I_Input.Frame())
		return false;

	return true;
}

bool Core::Frame()
{
	return true;
}

bool Core::PostFrame()
{
	// ���̾� ������ ��ȯ
	if (I_Input.KeyCheck(DIK_F4) == KEY_PUSH)
		m_bWireFrame = !m_bWireFrame;

	// ����� ��� ��ȯ
	if (I_Input.KeyCheck(DIK_V) == KEY_PUSH)
	{
		m_bDebug = !m_bDebug;
	}

	// ��� ī�޶� ��ȯ
	if (I_Input.KeyCheck(DIK_LCONTROL) == KEY_HOLD && I_Input.KeyCheck( DIK_1))
	{
		m_pMainCamera = m_pBackViewCamera;
		m_bCameraType = false;
	}

	// ����� ī�޶� ��ȯ
	if (I_Input.KeyCheck(DIK_LCONTROL) == KEY_HOLD && I_Input.KeyCheck(DIK_2))
	{
		m_pMainCamera = m_pDebugCamera;
		m_bCameraType = true;
	}

	// ��ī�̹ڽ� ON / OFF
	if (I_Input.KeyCheck(DIK_LCONTROL) == KEY_HOLD && I_Input.KeyCheck(DIK_3) == KEY_PUSH)
	{
		m_bSkyBoxSwitch = !m_bSkyBoxSwitch;
	}

	m_pMainCamera->Frame();

	return true;
}

bool Core::GameFrame()
{
	PreFrame();
	Frame();
	PostFrame();
	return true;
}

bool Core::PreRender()
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.5f };
	m_pImmediateContext->ClearRenderTargetView(m_DefaultRT.m_pRenderTargetView, ClearColor);
    m_pImmediateContext->ClearDepthStencilView(m_DefaultDS.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	m_pImmediateContext->OMSetRenderTargets(1, &m_DefaultRT.m_pRenderTargetView, m_DefaultDS.m_pDepthStencilView);
	DxState::ApplySS(m_pImmediateContext, DxState::g_pSSLinear);

	TMatrix matSkyWorld;
	TMatrix matSkyView = m_pMainCamera->m_matView;

	matSkyView._41 = 0;
	matSkyView._42 = 0;
	matSkyView._43 = 0;

	D3DXMatrixScaling(&matSkyView, 1000.0f, 1000.0f, 1000.0f);

	ID3D11RasterizerState* pRSCurrent = DxState::g_pRSCurrent;
	DxState::ApplyRS(m_pImmediateContext, DxState::g_pRSSolid);
	m_SkyObj.SetMatrix(&matSkyWorld, &matSkyView, &m_pMainCamera->m_matProj);
	DxState::ApplyDSS(m_pImmediateContext, DxState::g_pDSSDisable);
	if (m_bSkyBoxSwitch)
	{
		m_SkyObj.Render(m_pImmediateContext);
	}
	DxState::ApplyDSS(m_pImmediateContext, DxState::g_pDSSLessEqual);
	DxState::ApplyRS(m_pImmediateContext, pRSCurrent);

	// ���̾� ������
	if (m_bWireFrame == true)
	{
		DxState::ApplyRS(m_pImmediateContext, DxState::g_pRSCullBackWire);
	}
	else
	{
		DxState::ApplyRS(m_pImmediateContext, DxState::g_pRSCullBack);
	}

	return true;
}

bool Core::Render()
{
	return true;
}

bool Core::PostRender()
{
	HRESULT hr;

	// ����� ���(������ / ��������)
	if (m_bDebug)
	{
		if (m_Writer.Begin())
		{
			m_Writer.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			m_Writer.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			TCHAR buffer[256] = { 0, };
			_stprintf_s(buffer, L"����ð�: %5.3f, FPS: %5.d, SPF: %10.4f", m_Timer.GetElapsedTime(), m_Timer.GetFPS(), m_Timer.GetSPF());

			RT_F rc1(0, 0, m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
			m_Writer.DrawText(rc1, buffer, D2D1::ColorF(1, 1, 1, 1));
			m_Writer.End();
		}

		// ���� ���� ������
		TMatrix matWorld;
		matWorld = TMatrix::Identity;
		m_DrLine.SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_DrLine.Render(m_pImmediateContext);
	}
	if (FAILED(hr = m_pSwapChain->Present(0, 0)))
	{
		return false;
	}

	return true;
}

bool Core::GameRender()
{
	PreRender();
	Render();
	PostRender();
	return true;
}

bool Core::Release()
{
	return true;
}

bool Core::GameRelease()
{
	Release();

	SAFE_RELEASE(m_pDebugCamera);
	SAFE_RELEASE(m_pBackViewCamera);
	m_pMainCamera->Release();

	DeleteDxResource();
	return CleanupDevice();
}

bool Core::Run()
{
	if (!GameInit() == false)
	{
		return false;
	}
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameFrame();
			GameRender();
		}
	}
	if (!GameRelease()) return false;

#ifdef _DEBUG
	DXCheckMemoryLeak();
#endif // _DEBUG


	return true;
}

int	Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return -1;
}

HRESULT	Core::CreateDXResource()
{
	DxState::Init();

	return CreateResource();
}

HRESULT	Core::DeleteDxResource()
{
	DxState::Release();
	return S_OK;
	//return DeleteResource();
}

HRESULT	Core::CreateResource()
{
	return S_OK; 
}

HRESULT	Core::DeleteResource()
{
	m_SkyObj.Release();
	I_Input.Release();
	m_Timer.Release();
	m_Writer.Release();
	m_DrLine.Release();
	return S_OK;
}

Core::Core()
{
	m_pMainCamera		= nullptr;
	m_pDebugCamera		= nullptr;
	m_pBackViewCamera	= nullptr;
	m_bWireFrame		= false;
	m_bDebug			= false;
	m_bCameraType		= true;
	m_bSkyBoxSwitch		= true;
}

Core::~Core() {}