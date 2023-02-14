#pragma once
#include "Shape.h"

class Line :public Shape
{
public:
	vector<PC_VERTEX>		m_LineVertexList;
public:
	//--------------------------------------------------------------------------------------
	// 정점 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool			CreateVertexData() override;
	HRESULT			CreateVertexBuffer() override;
	//--------------------------------------------------------------------------------------
	// 인덱스 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool			CreateIndexData() override;
	HRESULT			CreateIndexBuffer() override;
	//--------------------------------------------------------------------------------------
	// 셰이더 생성
	//--------------------------------------------------------------------------------------
	HRESULT			LoadShader(T_STR vertexShaderName = _T(""),T_STR pixelShaderName = _T(""), T_STR geometryShader = _T("")) override;
	//--------------------------------------------------------------------------------------
	// 정점 레이아웃 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateVertexLayout() override;
	//--------------------------------------------------------------------------------------
	// 재정의 함수
	//--------------------------------------------------------------------------------------
	bool			Render(ID3D11DeviceContext* pContext) override;
	bool			Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor) override;
public:
	Line();
	virtual ~Line();
};

