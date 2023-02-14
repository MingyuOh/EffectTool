#pragma once
#include "CoreSTD.h"

namespace DX
{
	class DxState
	{
	public:
		//--------------------------------------------------------------------------------------
		// 깊이 버퍼
		//--------------------------------------------------------------------------------------
		static ID3D11DepthStencilState* g_pDSSCurrent;
		static ID3D11DepthStencilState* g_pDSSLessEqual;
		static ID3D11DepthStencilState*	g_pDSSDisable;
		//--------------------------------------------------------------------------------------
		// 샘플러 스테이트
		//--------------------------------------------------------------------------------------
		static ID3D11SamplerState*		g_pSSCurrent[3];
		static ID3D11SamplerState*		g_pSS;
		static ID3D11SamplerState*		g_pSSLinear;
		static ID3D11SamplerState*		g_pSSPoint;
		static ID3D11SamplerState*		g_pSSClamp;
		//--------------------------------------------------------------------------------------
		// 레스터라이저
		//--------------------------------------------------------------------------------------
		static ID3D11RasterizerState*	g_pRSCurrent;
		static ID3D11RasterizerState*	g_pRSCullBack;
		static ID3D11RasterizerState*	g_pRSCullFront;
		static ID3D11RasterizerState*	g_pRSCullBackWire;
		static ID3D11RasterizerState*	g_pRSSolid;
	public:
		static HRESULT					CreateDepthStencilState();
		static HRESULT					CreateSamplerState();
		static HRESULT					CreateRasterizerState();

		static ID3D11SamplerState*		ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pState, UINT StartSlot = 0);
		static ID3D11RasterizerState*	ApplyRS(ID3D11DeviceContext* pContext, ID3D11RasterizerState* pState);
		static ID3D11DepthStencilState* ApplyDSS(ID3D11DeviceContext* pContext, ID3D11DepthStencilState* pState);

		static bool						Init();
		static bool						Release();
	};
}

