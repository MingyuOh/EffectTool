#pragma once
#include "DxDS.h"

namespace DX
{
	class DxRT
	{
	public:
		DxDS												m_dxDS;
		ID3D11RenderTargetView*								m_pRenderTargetView;
		ID3D11ShaderResourceView*							m_pTextureSRV;
		ID3D11Texture2D*									m_pTexture;
		D3D11_TEXTURE2D_DESC								m_TexDesc;
		D3D11_VIEWPORT										m_ViewPort;
		TMatrix												m_matProj;

		D3D11_VIEWPORT										m_OldViewPort[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
		UINT												m_iNumViewPort;
		ID3D11RenderTargetView*								m_pOldRTV;
	public:
		HRESULT		Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
		HRESULT		CreateCubeMap(ID3D11Device* pd3dDevice, FLOAT fWidth = 1024, FLOAT fHeight = 1024);
		HRESULT		CreateDepthStencilTexture(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);

		void		Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
		HRESULT		UpdateDepthStencilView(ID3D11Device* pd3dDevice, UINT dwWidth, UINT dwHeight);
		void		Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV = NULL, ID3D11DepthStencilView* pDepthStencilView = NULL, 
							bool bDepthClear = true, bool bStencilClear = true);
		bool		Begin(ID3D11DeviceContext* pContext, TVector4 vColor, bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
		bool		Clear(ID3D11DeviceContext* pContext, TVector4 vColor, bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
		bool		End(ID3D11DeviceContext* pContext);
		HRESULT		SaveFile(ID3D11DeviceContext* pContext, T_STR name);
		//HRESULT SaveBackBuffer(ID3D11DeviceContext*    pContext, T_STR name);

		bool		Release();
	public:
		DxRT();
		~DxRT();
	};
}