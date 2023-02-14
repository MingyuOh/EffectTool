#pragma once
#include "CoreSTD.h"

class DirectWrite
{
public:
	//--------------------------------------------------------------------------------------
	// Write에 사용할 DX 생성
	//--------------------------------------------------------------------------------------
	ID2D1RenderTarget*			m_pRenderTarget;
	ID2D1Factory*				m_pD2DFactory;
	IDWriteFactory*				m_pDWriteFactory;
	ID2D1SolidColorBrush*		m_pBlackBrush;
	IDWriteTextFormat*			m_pTextFormat;
	IDWriteTextLayout*			m_pTextLayout;

	//--------------------------------------------------------------------------------------
	// 도트 퍼 인치(Dots per inch) :  디스플레이 해상도의 측정 단위, 
	//                                1 제곱인치 공간 안에 만들어진 점이나 화소의 수
	//--------------------------------------------------------------------------------------
	float						m_fDPIScaleX;
	float						m_fDPIScaleY;
	FLOAT						m_fdpiX;
	FLOAT						m_fdpiY;

	//--------------------------------------------------------------------------------------
	// Write에 사용할 기타 변수
	//--------------------------------------------------------------------------------------
	DWRITE_FONT_WEIGHT			m_fontWeight;
	DWRITE_FONT_STYLE			m_fontStyle;
	BOOL						m_bFontUnderline;
	wstring						m_wszFontFamily;
	float						m_fontSize;
	wstring						m_wszText;
	UINT32						m_cTextLength;
public:
	//--------------------------------------------------------------------------------------
	// 초기화
	//--------------------------------------------------------------------------------------
	bool						Set(IDXGISurface1* pSurface);
	bool						Init();
	//--------------------------------------------------------------------------------------
	// 렌더링
	//--------------------------------------------------------------------------------------
	bool						Begin();
	HRESULT						DrawText(RT_F rc, TCHAR* pText, D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	HRESULT						DrawText(D2D1_POINT_2F pos,  D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	bool						End();
	//--------------------------------------------------------------------------------------
	// 전체 소멸
	//--------------------------------------------------------------------------------------
	bool						Release();
	//--------------------------------------------------------------------------------------
	// 객체 생성 및 소멸
	//--------------------------------------------------------------------------------------
	HRESULT						CreateDeviceIndependentResources();
	void						DiscardDeviceIndependentResources();
	HRESULT						CreateDeviceResources(IDXGISurface1* pSurface);
	void						DiscardDeviceResources();
	//--------------------------------------------------------------------------------------
	// 각 속성값 저장
	//--------------------------------------------------------------------------------------
	HRESULT						SetText(D2D1_POINT_2F size, wchar_t* text, D2D1::ColorF Color);
	HRESULT						SetFont(wchar_t* fontFamily);
	HRESULT						SetFontSize(float size);
	HRESULT						SetBold(bool bold);
	HRESULT						SetItalic(bool italic);
	HRESULT						SetUnderline(bool underline);
	//--------------------------------------------------------------------------------------
	// 화면 사이즈 변경
	//--------------------------------------------------------------------------------------
	void						OnSize(IDXGISurface1* pSurface);
public:
	DirectWrite();
	~DirectWrite();
};

