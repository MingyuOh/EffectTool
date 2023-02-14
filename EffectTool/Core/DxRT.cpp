#include "DxRT.h"

namespace DX
{
	void DxRT::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
		FLOAT MinDepth, FLOAT MaxDepth)
	{
		m_ViewPort.TopLeftX = TopLeftX;
		m_ViewPort.TopLeftY = TopLeftY;
		m_ViewPort.Width = Width;
		m_ViewPort.Height = Height;
		m_ViewPort.MinDepth = MinDepth;
		m_ViewPort.MaxDepth = MaxDepth;

		D3DXMatrixPerspectiveFovLH(&m_matProj, (float)(PI * 0.25f), Width / Height, NEAR_DISTANCE, FAR_DISTANCE);
	}

	HRESULT DxRT::CreateCubeMap(ID3D11Device* pd3dDevice, FLOAT fWidth, FLOAT fHeight)
	{
		HRESULT hr;
		Set(pd3dDevice, 0, 0, fWidth, fHeight, 0.0f, 0.1f);

		// 랜더타켓용 텍스쳐 생성
		D3D11_TEXTURE2D_DESC TexDesc;
		ZeroMemory(&TexDesc, sizeof(D3D11_TEXTURE2D_DESC));
		TexDesc.Width = fWidth;
		TexDesc.Height = fHeight;
		TexDesc.MipLevels = 1;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.ArraySize = 6;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		if (FAILED(hr = pd3dDevice->CreateTexture2D(&TexDesc, NULL, &m_pTexture)))
		{
			return S_FALSE;
		}

		// 랜더타켓용 뷰 생성
		D3D11_RENDER_TARGET_VIEW_DESC RtvDesc;
		ZeroMemory(&RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		RtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		RtvDesc.Texture2DArray.ArraySize = 6;
		if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture, &RtvDesc, &m_pRenderTargetView)))
		{
			return S_FALSE;
		}

		// 쉐이더 리소스 뷰 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMemory(&SrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SrvDesc.Format = TexDesc.Format;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SrvDesc.Texture2DArray.MipLevels = 1;
		if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture, &SrvDesc, &m_pTextureSRV)))
		{
			return S_FALSE;
		}

		// 깊이 스텐실 뷰 텍스쳐 생성
		ID3D11Texture2D* pDSTexture = nullptr;
		D3D11_TEXTURE2D_DESC DSDesc;
		ZeroMemory(&DSDesc, sizeof(D3D11_TEXTURE2D_DESC));
		DSDesc.Width = fWidth;
		DSDesc.Height = fHeight;
		DSDesc.MipLevels = 1;
		DSDesc.ArraySize = 6;
		DSDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DSDesc.SampleDesc.Count = 1;
		DSDesc.SampleDesc.Quality = 0;
		DSDesc.Usage = D3D11_USAGE_DEFAULT;
		DSDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DSDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		if (FAILED(hr = pd3dDevice->CreateTexture2D(&DSDesc, NULL, &pDSTexture)))
		{
			return S_FALSE;
		}

		// 깊이 스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC DsvDesc;
		ZeroMemory(&DsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		DsvDesc.Format = DSDesc.Format;
		DsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		DsvDesc.Texture2DArray.ArraySize = 6;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilView(pDSTexture, &DsvDesc, &m_dxDS.m_pDepthStencilView)))
		{
			return S_FALSE;
		}

		return S_OK;
	}

	HRESULT DxRT::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
	{
		HRESULT hr = S_OK;
		Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

		m_TexDesc.Width = (UINT)Width;
		m_TexDesc.Height = (UINT)Height;
		m_TexDesc.MipLevels = 1;
		m_TexDesc.ArraySize = 1;
		m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_TexDesc.SampleDesc.Count = 1;
		m_TexDesc.SampleDesc.Quality = 0;
		m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
		m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_TexDesc.CPUAccessFlags = 0;
		m_TexDesc.MiscFlags = 0;

		if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, &m_pTexture)))
		{
			return hr;
		}

		if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pTextureSRV)))
		{
			return hr;
		}
		if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture, NULL, &m_pRenderTargetView)))
		{
			return hr;
		}

		// 백버퍼의 크기가 변경되면 반드시 깊이스텐실 뷰어도 다시 작성되어야 한다.		
		if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
		{
			return hr;
		}
		return hr;
	}

	HRESULT DxRT::CreateDepthStencilTexture(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
	{
		HRESULT hr = S_OK;
		Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

		m_TexDesc.Width = Width;
		m_TexDesc.Height = Height;
		m_TexDesc.MipLevels = 1;
		m_TexDesc.ArraySize = 1;
		m_TexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		m_TexDesc.SampleDesc.Count = 1;
		m_TexDesc.SampleDesc.Quality = 0;
		m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
		m_TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		m_TexDesc.CPUAccessFlags = 0;
		m_TexDesc.MiscFlags = 0;

		if (FAILED(hr = g_pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, &m_pTexture)))
		{
			return hr;
		}

		return hr;
	}

	HRESULT DxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height)
	{
		HRESULT hr = S_OK;

		m_ViewPort.Width = Width;
		m_ViewPort.Height = Height;

		hr = m_dxDS.CreateDepthStencilView(pDevice, Width, Height);

		return hr;
	}

	void DxRT::Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDepthStencilView, bool bDepthClear, bool bStencilClear)
	{
		assert(pContext);
		assert(pRTV);
		ID3D11RenderTargetView* pNullRTV = NULL;
		pContext->OMSetRenderTargets(1, &pNullRTV, NULL);

		if (pRTV != NULL)
		{
			if (pDepthStencilView != NULL)
				pContext->OMSetRenderTargets(1, &pRTV, pDepthStencilView);
			else
				pContext->OMSetRenderTargets(1, &pRTV, m_dxDS.m_pDepthStencilView);
		}
		else
		{
			if (pDepthStencilView != NULL)
				pContext->OMSetRenderTargets(1, &m_pRenderTargetView, pDepthStencilView);
			else
				pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_dxDS.m_pDepthStencilView);
		}

		if (bDepthClear)
		{
			if (pDepthStencilView != NULL)
				pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
			else
				pContext->ClearDepthStencilView(m_dxDS.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

		}
		if (bStencilClear)
		{
			if (pDepthStencilView != NULL)
				pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0, 0);
			else
				pContext->ClearDepthStencilView(m_dxDS.m_pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0, 0);
		}
		pContext->RSSetViewports(1, &m_ViewPort);
	}

	bool DxRT::Begin(ID3D11DeviceContext* pContext, TVector4 vColor, bool bTargetClear, bool bDepthClear, bool bStencilClear)
	{
		m_iNumViewPort = 1;
		pContext->RSGetViewports(&m_iNumViewPort, m_OldViewPort);
		pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_dxDS.m_pOldDSV);

		ID3D11RenderTargetView* pNullRTV = nullptr;
		pContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
		/*ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
		pContext->PSSetShaderResources(0, 2, ppSRVNULL);*/

		Clear(pContext, vColor, bTargetClear, bDepthClear, bStencilClear);

		pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_dxDS.m_pDepthStencilView);
		pContext->RSSetViewports(1, &m_ViewPort);
		return true;
	}

	bool DxRT::Clear(ID3D11DeviceContext* pContext, TVector4 vColor, bool bTargetClear, bool bDepthClear, bool bStencilClear)
	{
		float color[4] = { vColor.x, vColor.y, vColor.z, vColor.w };
		if (bTargetClear)
		{
			pContext->ClearRenderTargetView(m_pRenderTargetView, color);
		}
		if (bDepthClear)
		{
			pContext->ClearDepthStencilView(m_dxDS.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
		if (bStencilClear)
		{
			pContext->ClearDepthStencilView(m_dxDS.m_pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		pContext->RSSetViewports(1, &m_ViewPort);
		return true;
	}

	bool DxRT::End(ID3D11DeviceContext* pContext)
	{
		/*ID3D11RenderTargetView* pRTV = nullptr;
		ID3D11DepthStencilView* pDSV = nullptr;
		pContext->OMSetRenderTargets(1, &pRTV, pDSV);
		ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
		pContext->PSSetShaderResources(0, 2, ppSRVNULL);*/

		pContext->RSSetViewports(m_iNumViewPort, m_OldViewPort);
		pContext->OMSetRenderTargets(1, &m_pOldRTV, m_dxDS.m_pOldDSV);

		SAFE_RELEASE(m_pOldRTV);
		SAFE_RELEASE(m_dxDS.m_pOldDSV);

		return true;
	}

	HRESULT DxRT::SaveFile(ID3D11DeviceContext* pContext, T_STR name)
	{
		HRESULT hr = S_OK;
		// 랜더타켓뷰에서 텍스처 파일로 저장	
		if (m_pTexture != nullptr)
		{
			// 랜더타켓 텍스처를 텍스처 파일로 저장
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatPng, (name + L".png").c_str());
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatPng, (name + L".png").c_str());
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatBmp, (name + L".bmp").c_str());
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatBmp, (name + L".bmp").c_str(), &GUID_WICPixelFormat16bppBGR565);
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatTiff, (name + L".tif").c_str());
			hr = SaveWICTextureToFile(pContext, m_pTexture, GUID_ContainerFormatTiff, (name + L".tif").c_str(), nullptr, [&](IPropertyBag2* props)
			{
				PROPBAG2 options[2] = { 0, 0 };
				options[0].pstrName = const_cast<WCHAR*>(OLESTR("CompressionQuality"));
				options[1].pstrName = const_cast<WCHAR*>(OLESTR("TiffCompressionMethod"));

				VARIANT varValues[2];
				varValues[0].vt = VT_R4;
				varValues[0].fltVal = 0.75f;

				varValues[1].vt = VT_UI1;
				varValues[1].bVal = WICTiffCompressionNone;

				(void)props->Write(2, options, varValues);
			});
		}
		else if (m_pTextureSRV != nullptr)
		{
			ComPtr<ID3D11Resource> pResource;
			m_pTextureSRV->GetResource(&pResource);

			ComPtr<ID3D11Texture2D> pTexture;
			if (FAILED(hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(pTexture.GetAddressOf()))))
			{
				return hr;
			}
			if (SUCCEEDED(hr))
			{
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatPng, (name + L".png").c_str());
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatPng, (name + L".png").c_str());
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str());
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str(), &GUID_WICPixelFormat16bppBGR565);
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str());
				hr = SaveWICTextureToFile(pContext, pTexture.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str(), nullptr, [&](IPropertyBag2* props)
				{
					PROPBAG2 options[2] = { 0, 0 };
					options[0].pstrName = const_cast<WCHAR*>(OLESTR("CompressionQuality"));
					options[1].pstrName = const_cast<WCHAR*>(OLESTR("TiffCompressionMethod"));

					VARIANT varValues[2];
					varValues[0].vt = VT_R4;
					varValues[0].fltVal = 0.75f;

					varValues[1].vt = VT_UI1;
					varValues[1].bVal = WICTiffCompressionNone;

					(void)props->Write(2, options, varValues);
				});
			}
		}
		return hr;
	}

	//--------------------------------------------------------------------------------------
	// 백버퍼 저장 
	//--------------------------------------------------------------------------------------
	/*HRESULT DxRT::SaveBackBuffer(ID3D11DeviceContext*    pContext, T_STR name)
	{
		HRESULT hr = S_OK;
		ID3D11Resource *backbufferRes;
		m_pRenderTargetView.Get()->GetResource(&backbufferRes);
		D3D11_TEXTURE2D_DESC desc;
		m_pTexture.Get()->GetDesc(&desc);

		ID3D11Texture2D *texture;
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = 0;
		texDesc.CPUAccessFlags = desc.CPUAccessFlags;
		texDesc.Format = desc.Format;
		texDesc.Height = desc.Height;
		texDesc.Width = desc.Width;
		texDesc.MipLevels = 1;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc = desc.SampleDesc;
		texDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(->CreateTexture2D(&texDesc, 0, &texture)))
		{

		}
		pContext->CopyResource(texture, backbufferRes);

		if (FAILED(D3DX11SaveTextureToFile(pContext, texture, D3DX11_IFF_BMP, name.c_str())))
		{
			return hr;
		}
		texture->Release();
		backbufferRes->Release();
		return hr;
	}*/

	bool DxRT::Release()
	{
		m_dxDS.Release();
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pTextureSRV);
		SAFE_RELEASE(m_pTexture);
		SAFE_RELEASE(m_pOldRTV);

		return true;
	}

	DxRT::DxRT()
	{
		SAFE_ZERO(m_pRenderTargetView);
		SAFE_ZERO(m_pTextureSRV);
		SAFE_ZERO(m_pTexture);
		SAFE_ZERO(m_pOldRTV);
	}

	DxRT::~DxRT()
	{
	}

}