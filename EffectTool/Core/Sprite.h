#pragma once
#include "Plane.h"

struct RectUV
{
	TVector2 uv[4];
};

class Sprite : public DXCore::Plane
{
public:
	vector<RectUV>		m_uvList;
	int					m_iIndex;

	float				m_fOffsetTime;
	float				m_fLifeTime;
	int					m_iCurrentFrame;
	float				m_fCurrentTime;
public:
	//--------------------------------------------------------------------------------------
	// Rect�� �ʱ�ȭ
	//--------------------------------------------------------------------------------------
	void				SetRect(int iRow, int iCol, float fOffset);
	//--------------------------------------------------------------------------------------
	// �ð��� ���� �̹��� ��ȯ �Լ�
	//--------------------------------------------------------------------------------------
	void				Update();
	//--------------------------------------------------------------------------------------
	// ���̾ƿ� �� ���̴� ����
	//--------------------------------------------------------------------------------------
	/*HRESULT			LoadShaderAndInputLayout(T_STR vertexShaderName = NULL,
		T_STR PixelShaderName = NULL, T_STR geometryShaderName = NULL);*/
	//--------------------------------------------------------------------------------------
	// Rect�� ��ȯ �Լ�
	//--------------------------------------------------------------------------------------
	TVector2			GetDrawRect(int iUV);
public:
	Sprite();
	~Sprite();
};

