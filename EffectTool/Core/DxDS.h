#pragma once
#include "CoreSTD.h"

namespace DX
{
	class DxDS
	{
	public:
		ID3D11DepthStencilView*			m_pDepthStencilView;
		ID3D11ShaderResourceView*		m_pTextureSRV;
		ID3D11Texture2D*				m_pTexture;
		ID3D11DepthStencilView*			m_pOldDSV;
		D3D11_DEPTH_STENCIL_VIEW_DESC	m_DepthStencilDesc;
		DXGI_FORMAT						m_DSFormat;
	public:
		ID3D11Texture2D*				CreateTexture(ID3D11Device* pd3dDevice, UINT Width, UINT Height);
		HRESULT							CreateDepthStencilView(ID3D11Device* pd3dDevice, UINT Width, UINT Height);
		bool							Release();
	public:
		DxDS();
	};
}

