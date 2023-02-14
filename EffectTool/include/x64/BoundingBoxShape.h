#pragma once
#include "Shape.h"

class BoundingBoxShape : public Shape
{
private:
	vector<PC_VERTEX>		m_BoundingVertexList;
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
	HRESULT			LoadShader(T_STR vertexShaderName = _T(""), T_STR pixelShaderName = _T(""), T_STR geometryShader = _T("")) override;
	//--------------------------------------------------------------------------------------
	// 정점 레이아웃 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateVertexLayout() override;
public:
	bool			Render(ID3D11DeviceContext* pContext);
public:
	BoundingBoxShape();
	~BoundingBoxShape();
};

