#include "Box.h"

bool Box::CreateVertexData()
{
	m_VertexList.resize(24);
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞
	m_VertexList[0] = { TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[1] = { TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[2] = { TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[3] = { TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f) };

	// 뒤
	m_VertexList[4] = { TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[5] = { TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[6] = { TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[7] = { TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f) };

	// 오른쪽
	m_VertexList[8] = { TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[9] = { TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[10] = { TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[11] = { TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f) };

	// 왼쪽
	m_VertexList[12] = { TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[13] = { TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[14] = { TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[15] = { TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f) };

	// 위
	m_VertexList[16] = { TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[17] = { TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[18] = { TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[19] = { TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f) };

	// 아래
	m_VertexList[20] = { TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
	m_VertexList[21] = { TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
	m_VertexList[22] = { TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	m_VertexList[23] = { TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f) };

	m_iNumVertex = sizeof(m_VertexList) / sizeof(m_VertexList[0]);

	if (m_VertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Box::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	CD3D11_BUFFER_DESC cbc(sizeof(PNCT_VERTEX) *  m_VertexList.size(), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_VertexList.at(0);
	return g_pd3dDevice->CreateBuffer(&cbc, &InitData, &m_pVertexBuffer);
}

bool Box::CreateIndexData()
{
	m_IndexList.resize(36);
	m_iNumIndex = m_IndexList.size();
	int iIndex = 0;
	// 앞
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;

	// 뒤
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 5; m_IndexList[iIndex++] = 6;
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 6; m_IndexList[iIndex++] = 7;

	// 오른쪽
	m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 9; m_IndexList[iIndex++] = 10;
	m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;

	// 왼쪽
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;

	// 위
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;

	// 아래
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Box::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = m_IndexList.size() * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = &m_IndexList.at(0);

	hr = g_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer);
	return hr;
}

Box::Box()
{
}


Box::~Box()
{
}
