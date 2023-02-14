#pragma once
#include "Shape.h"

class Line :public Shape
{
public:
	vector<PC_VERTEX>		m_LineVertexList;
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
	HRESULT			LoadShader(T_STR vertexShaderName = _T(""),T_STR pixelShaderName = _T(""), T_STR geometryShader = _T("")) override;
	//--------------------------------------------------------------------------------------
	// ���� ���̾ƿ� ����
	//--------------------------------------------------------------------------------------
	HRESULT			CreateVertexLayout() override;
	//--------------------------------------------------------------------------------------
	// ������ �Լ�
	//--------------------------------------------------------------------------------------
	bool			Render(ID3D11DeviceContext* pContext) override;
	bool			Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor) override;
public:
	Line();
	virtual ~Line();
};

