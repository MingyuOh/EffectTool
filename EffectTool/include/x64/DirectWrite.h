#pragma once
#include "CoreSTD.h"

class DirectWrite
{
public:
	//--------------------------------------------------------------------------------------
	// Write�� ����� DX ����
	//--------------------------------------------------------------------------------------
	ID2D1RenderTarget*			m_pRenderTarget;
	ID2D1Factory*				m_pD2DFactory;
	IDWriteFactory*				m_pDWriteFactory;
	ID2D1SolidColorBrush*		m_pBlackBrush;
	IDWriteTextFormat*			m_pTextFormat;
	IDWriteTextLayout*			m_pTextLayout;

	//--------------------------------------------------------------------------------------
	// ��Ʈ �� ��ġ(Dots per inch) :  ���÷��� �ػ��� ���� ����, 
	//                                1 ������ġ ���� �ȿ� ������� ���̳� ȭ���� ��
	//--------------------------------------------------------------------------------------
	float						m_fDPIScaleX;
	float						m_fDPIScaleY;
	FLOAT						m_fdpiX;
	FLOAT						m_fdpiY;

	//--------------------------------------------------------------------------------------
	// Write�� ����� ��Ÿ ����
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
	// �ʱ�ȭ
	//--------------------------------------------------------------------------------------
	bool						Set(IDXGISurface1* pSurface);
	bool						Init();
	//--------------------------------------------------------------------------------------
	// ������
	//--------------------------------------------------------------------------------------
	bool						Begin();
	HRESULT						DrawText(RT_F rc, TCHAR* pText, D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	HRESULT						DrawText(D2D1_POINT_2F pos,  D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	bool						End();
	//--------------------------------------------------------------------------------------
	// ��ü �Ҹ�
	//--------------------------------------------------------------------------------------
	bool						Release();
	//--------------------------------------------------------------------------------------
	// ��ü ���� �� �Ҹ�
	//--------------------------------------------------------------------------------------
	HRESULT						CreateDeviceIndependentResources();
	void						DiscardDeviceIndependentResources();
	HRESULT						CreateDeviceResources(IDXGISurface1* pSurface);
	void						DiscardDeviceResources();
	//--------------------------------------------------------------------------------------
	// �� �Ӽ��� ����
	//--------------------------------------------------------------------------------------
	HRESULT						SetText(D2D1_POINT_2F size, wchar_t* text, D2D1::ColorF Color);
	HRESULT						SetFont(wchar_t* fontFamily);
	HRESULT						SetFontSize(float size);
	HRESULT						SetBold(bool bold);
	HRESULT						SetItalic(bool italic);
	HRESULT						SetUnderline(bool underline);
	//--------------------------------------------------------------------------------------
	// ȭ�� ������ ����
	//--------------------------------------------------------------------------------------
	void						OnSize(IDXGISurface1* pSurface);
public:
	DirectWrite();
	~DirectWrite();
};

