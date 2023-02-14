#pragma once
#include "Map.h"

class HeightMap :public Map
{
public:
	float		m_fMaxHeight;
public:
	//--------------------------------------------------------------------------------------
	// ���� �� �ҷ����� �Լ�
	//--------------------------------------------------------------------------------------
	bool		LoadHeightMap(ID3D11DeviceContext* pContext, T_STR strLoad);
	float		Lerp(float a, float b, float t);
	void		SetMaxHeight();
	float		GetHeight(int iRow, int iCol);
	float		GetHeight(TVector3 vPos);
public:
	HeightMap();
	virtual ~HeightMap();
};

