#pragma once
#include "Shape.h"

class BoundingBoxShape : public Shape
{
private:
	vector<PC_VERTEX>		m_BoundingVertexList;
public:
	//--------------------------------------------------------------------------------------
	// ���� ���� ����
	//--------------------------------------------------------------------------------------
	bool			CreateVertexData() override;
	HRESULT			CreateVertexBuffer() override;
	//--------------------------------------------------------------------------------------
	// �ε��� ���� ����
	//--------------------------------------------------------------------------------------
	bool			CreateIndexData() override;
	HRESULT			CreateIndexBuffer() override;
	//--------------------------------------------------------------------------------------
	// ���̴� ����
	//--------------------------------------------------------------------------------------
	HRESULT			LoadShader(T_STR vertexShaderName = _T(""), T_STR pixelShaderName = _T(""), T_STR geometryShader = _T("")) override;
	//--------------------------------------------------------------------------------------
	// ���� ���̾ƿ� ����
	//--------------------------------------------------------------------------------------
	HRESULT			CreateVertexLayout() override;
public:
	bool			Render(ID3D11DeviceContext* pContext);
public:
	BoundingBoxShape();
	~BoundingBoxShape();
};

