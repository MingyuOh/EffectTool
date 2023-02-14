#include "DirectWrite.h"

//--------------------------------------------------------------------------------------
// 초기화
//--------------------------------------------------------------------------------------
bool DirectWrite::Set(IDXGISurface1* pSurface)
{
	if (!Init())
	{
		return false;
	}
	
	CreateDeviceIndependentResources();
	CreateDeviceResources(pSurface);
	SetText(D2D1::Point2F(g_iWindowWidth, g_iWindowHeight), L"", D2D1::ColorF(0, 0, 0, 0));
	return true;
}

bool DirectWrite::Init()
{
	const wchar_t defaultText[] = L"";
	m_wszFontFamily = defaultText;
	return true;
}

//--------------------------------------------------------------------------------------
// 렌더링
//--------------------------------------------------------------------------------------
bool DirectWrite::Begin()
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->BeginDraw();
		// m_pRenderTarget->SetTransform(D2D1::IdentityTMatrix());
	}
	return true;
}

HRESULT	DirectWrite::DrawText(RT_F rc, TCHAR* pText, D2D1::ColorF Color)
{
	HRESULT hr = S_OK;
	if (m_pRenderTarget && m_pBlackBrush)
	{/*
		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left) / m_fDPIScaleX,
			static_cast<FLOAT>(rc.top) / m_fDPIScaleY,
			static_cast<FLOAT>(rc.right) / m_fDPIScaleX,
			static_cast<FLOAT>(rc.bottom) / m_fDPIScaleY );*/
		m_pBlackBrush->SetColor(Color);
		m_pRenderTarget->DrawText(pText, wcslen(pText), m_pTextFormat, rc, m_pBlackBrush);
	}
	return hr;
}

HRESULT	DirectWrite::DrawText(D2D1_POINT_2F pos, D2D1::ColorF Color)
{
	HRESULT hr = S_OK;
	D2D1_POINT_2F origin = D2D1::Point2F(
		static_cast<FLOAT>(pos.x / m_fDPIScaleX),
		static_cast<FLOAT>(pos.y / m_fDPIScaleY) );
	m_pBlackBrush->SetColor(Color);
	m_pRenderTarget->DrawTextLayout(origin, m_pTextLayout, m_pBlackBrush);
	return hr;
}

bool DirectWrite::End()
{
	if (m_pRenderTarget && FAILED(m_pRenderTarget->EndDraw()))
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// 전체 소멸
//--------------------------------------------------------------------------------------
bool DirectWrite::Release()
{
	m_wszText.clear();
	m_wszFontFamily.clear();
	DiscardDeviceIndependentResources();
	DiscardDeviceResources();
	return true;
}

//--------------------------------------------------------------------------------------
// 독립 객체 생성 및 소멸
//--------------------------------------------------------------------------------------
HRESULT	DirectWrite::CreateDeviceIndependentResources()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	UINT dpi = GetDpiForWindow(g_hWnd);
	m_fdpiX = static_cast<FLOAT>(dpi);
	m_fdpiY = static_cast<FLOAT>(dpi);
	m_fDPIScaleX = m_fdpiX / 96.0f;
	m_fDPIScaleY = m_fdpiY / 96.0f;

	if (FAILED(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory))))
		return hr;

	if (FAILED(hr = m_pDWriteFactory->CreateTextFormat(L"돋음", NULL, m_fontWeight, m_fontStyle, DWRITE_FONT_STRETCH_NORMAL, 15, L"ko-kr", &m_pTextFormat)))
		return hr;
	return hr;
}

void DirectWrite::DiscardDeviceIndependentResources()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_pTextLayout);
}

//--------------------------------------------------------------------------------------
// 디바이스에 종속적인 객체 생성 및 소멸
//--------------------------------------------------------------------------------------
HRESULT	DirectWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
	HRESULT hr;
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		static_cast<float>(m_fdpiX),
		static_cast<float>(m_fdpiY));

	if (FAILED(hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, (const D2D1_RENDER_TARGET_PROPERTIES*)&props, &m_pRenderTarget)))
	{
		return hr;
	}
	
	if (FAILED(hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush)))
	{
		return hr;
	}

	return hr;
}

void DirectWrite::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pBlackBrush);
}

//--------------------------------------------------------------------------------------
// 각 속성값 저장
//--------------------------------------------------------------------------------------
HRESULT	DirectWrite::SetText(D2D1_POINT_2F size, wchar_t* text, D2D1::ColorF Color)
{
	HRESULT hr = S_OK;
	m_cTextLength = (UINT32)wcslen(text);
	m_wszText.clear();
	m_wszText = text;
	SAFE_RELEASE(m_pTextLayout);

	if (FAILED(hr = m_pDWriteFactory->CreateTextLayout(m_wszText.c_str(), m_cTextLength, m_pTextFormat, size.x, size.y, &m_pTextLayout)))
	{
		return hr;
	}

	IDWriteTypography* pTypography = NULL;

	if (FAILED(hr = m_pDWriteFactory->CreateTypography(&pTypography)))
	{
		return hr;
	}

	DWRITE_FONT_FEATURE fontFeature = { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7, 1 };

	if (FAILED(hr = pTypography->AddFontFeature(fontFeature)))
	{
		return hr;
	}

	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	if (FAILED(hr = m_pTextLayout->SetTypography(pTypography, textRange)))
	{
		return hr;
	}

	if (FAILED(hr = m_pTextLayout->SetUnderline(m_bFontUnderline, textRange)))
	{
		return hr;
	}
	SAFE_RELEASE(pTypography);
	return hr;
}

HRESULT	DirectWrite::SetFont(wchar_t* fontFamily)
{
	HRESULT hr;

	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	hr = m_pTextLayout->SetFontFamilyName(fontFamily, textRange);

	if (SUCCEEDED(hr))
	{
		m_wszFontFamily.clear();
		m_wszFontFamily = fontFamily;
	}
	return hr;
}

HRESULT	DirectWrite::SetFontSize(float size)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	hr = m_pTextLayout->SetFontSize(size, textRange);

	if (SUCCEEDED(hr))
	{
		m_fontSize = size;
	}
	return hr;
}

HRESULT	DirectWrite::SetBold(bool bold)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	if (bold)
	{
		m_fontWeight = DWRITE_FONT_WEIGHT_BOLD;
	}
	else
	{
		m_fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	}

	hr = m_pTextLayout->SetFontWeight(m_fontWeight, textRange);
	return hr;
}

HRESULT	DirectWrite::SetItalic(bool italic)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	if (italic)
	{
		m_fontStyle = DWRITE_FONT_STYLE_ITALIC;
	}
	else
	{
		m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
	}

	// Set the font style.
	hr = m_pTextLayout->SetFontStyle(m_fontStyle, textRange);
	return hr;
}

HRESULT	DirectWrite::SetUnderline(bool underline)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

	m_bFontUnderline = underline;

	// Set the underline.
	hr = m_pTextLayout->SetUnderline(m_bFontUnderline, textRange);
	return hr;
}

//--------------------------------------------------------------------------------------
// 화면 사이즈 변경
//--------------------------------------------------------------------------------------
void DirectWrite::OnSize(IDXGISurface1* pSurface)
{
	DiscardDeviceResources();
	CreateDeviceResources(pSurface);
	SetText(D2D1::Point2F(g_iWindowWidth, g_iWindowHeight), L"", D2D1::ColorF(1, 1, 1, 1));
}

DirectWrite::DirectWrite()
{
	m_pBlackBrush		= nullptr;
	m_pDWriteFactory	= nullptr;
	m_pTextFormat		= nullptr;
	m_pD2DFactory		= nullptr;
	m_pTextLayout		= nullptr;
	m_pRenderTarget		= nullptr;
	m_cTextLength		= 0;
	m_fontWeight		= DWRITE_FONT_WEIGHT_NORMAL;
	m_fontStyle			= DWRITE_FONT_STYLE_NORMAL;
	m_bFontUnderline	= FALSE;
	m_fontSize			= 72.0f;
}


DirectWrite::~DirectWrite()
{
}
