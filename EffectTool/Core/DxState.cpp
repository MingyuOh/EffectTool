#include "DxState.h"

namespace DX
{
	ID3D11DepthStencilState* DxState::g_pDSSCurrent = nullptr;
	ID3D11DepthStencilState* DxState::g_pDSSLessEqual = nullptr;
	ID3D11DepthStencilState* DxState::g_pDSSDisable = nullptr;

	ID3D11SamplerState* DxState::g_pSSCurrent[3] = { nullptr, nullptr, nullptr };
	ID3D11SamplerState* DxState::g_pSS = nullptr;
	ID3D11SamplerState* DxState::g_pSSLinear = nullptr;
	ID3D11SamplerState* DxState::g_pSSPoint = nullptr;
	ID3D11SamplerState* DxState::g_pSSClamp = nullptr;

	ID3D11RasterizerState* DxState::g_pRSCurrent = nullptr;
	ID3D11RasterizerState* DxState::g_pRSCullBack = nullptr;
	ID3D11RasterizerState* DxState::g_pRSCullFront = nullptr;
	ID3D11RasterizerState* DxState::g_pRSCullBackWire = nullptr;
	ID3D11RasterizerState* DxState::g_pRSSolid = nullptr;

	HRESULT	DxState::CreateDepthStencilState()
	{
		HRESULT hr = S_OK;
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));

		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;//D3D11_COMPARISON_ALWAYS;
		dsd.StencilEnable = FALSE;
		hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSSLessEqual);

		ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsd.DepthEnable = FALSE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsd.StencilEnable = FALSE;
		hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSSDisable);

		return hr;
	}

	HRESULT DxState::CreateSamplerState()
	{
		HRESULT hr = S_OK;
		D3D11_SAMPLER_DESC sd;

		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		sd.MinLOD = FLT_MIN;
		sd.MaxLOD = FLT_MAX;
		hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pSS);
		
		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.MinLOD = FLT_MIN;
		sd.MaxLOD = FLT_MAX;
		hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pSSLinear);
		
		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sd.MinLOD = FLT_MIN;
		sd.MaxLOD = FLT_MAX;
		hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pSSPoint);

		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.MinLOD = FLT_MIN;
		sd.MaxLOD = FLT_MAX;
		hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pSSClamp);
		
		return hr;
	}

	HRESULT	DxState::CreateRasterizerState()
	{
		HRESULT hr = S_OK;

		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));

		rd.DepthClipEnable = TRUE;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSCullBack);

		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.DepthClipEnable = TRUE;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_FRONT; // 뒤에꺼 제거하고 앞에꺼 뿌려라    
		hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSCullFront);

		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.DepthClipEnable = TRUE;
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_BACK; // 뒤에꺼 제거하고 앞에꺼 뿌려라    
		hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSCullBackWire);

		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.DepthClipEnable = TRUE;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE; // 앞뒤면 가리지말고 다 뿌려라
		hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSSolid);

		return hr;
	}

	ID3D11DepthStencilState* DxState::ApplyDSS(ID3D11DeviceContext* pContext, ID3D11DepthStencilState* pState)
	{
		ID3D11DepthStencilState* pDSS = DxState::g_pDSSCurrent;
		pContext->OMSetDepthStencilState(pState, 0x01);
		DxState::g_pDSSCurrent = pState;
		return pDSS;
	}

	ID3D11SamplerState* DxState::ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pState, UINT StartSlot)
	{
		ID3D11SamplerState* pSS = DxState::g_pSSCurrent[StartSlot];
		pContext->PSSetSamplers(StartSlot, 1, &pState);
		DxState::g_pSSCurrent[StartSlot] = pState;
		return pSS;
	}

	ID3D11RasterizerState* DxState::ApplyRS(ID3D11DeviceContext* pContext, ID3D11RasterizerState* pState)
	{
		ID3D11RasterizerState* pRS = DxState::g_pRSCurrent;
		pContext->RSSetState(pState);
		DxState::g_pRSCurrent = pState;
		return pRS;
	}

	bool DxState::Init()
	{
		CreateDepthStencilState();
		CreateSamplerState();
		CreateRasterizerState();

		return true;
	}

	bool DxState::Release()
	{
		SAFE_RELEASE(g_pDSSLessEqual);
		SAFE_RELEASE(g_pDSSDisable);
		SAFE_RELEASE(g_pSS);
		SAFE_RELEASE(g_pSSLinear);
		SAFE_RELEASE(g_pSSPoint);
		SAFE_RELEASE(g_pSSClamp);
		SAFE_RELEASE(g_pRSSolid);
		SAFE_RELEASE(g_pRSCullBack);
		SAFE_RELEASE(g_pRSCullFront);
		SAFE_RELEASE(g_pRSCullBackWire);

		return true;
	}
}