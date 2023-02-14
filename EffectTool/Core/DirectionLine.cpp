#include "DirectionLine.h"

bool DirectionLine::CreateVertexData()
{
	m_LineVertexList.resize(6);
	m_LineVertexList[0] = PC_VERTEX(TVector3(0.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[1] = PC_VERTEX(TVector3(1000.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[2] = PC_VERTEX(TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[3] = PC_VERTEX(TVector3(0.0f, 1000.0f, 0.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[4] = PC_VERTEX(TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f));
	m_LineVertexList[5] = PC_VERTEX(TVector3(0.0f, 0.0f, 1000.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f));

	m_iNumVertex = m_LineVertexList.size();
	if (m_LineVertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	DirectionLine::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CreateVertexData()))
	{
		return hr;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PC_VERTEX) * m_iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	CD3D11_BUFFER_DESC cbc(sizeof(PC_VERTEX) * m_iNumVertex, D3D11_BIND_VERTEX_BUFFER);
	
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_LineVertexList.at(0);
	return g_pd3dDevice->CreateBuffer(&cbc, &InitData, &m_pVertexBuffer);
}

bool DirectionLine::CreateIndexData()
{
	DWORD indicies[] =
	{
		0, 1,
		2, 3,
		4, 5
	};

	m_iNumIndex = sizeof(indicies) / sizeof(indicies[0]);

	m_IndexList.resize(m_iNumIndex);
	for (int index = 0; index < m_iNumIndex; index++)
	{
		m_IndexList[index] = indicies[index];
	}

	if (m_IndexList.empty())
	{
		return false;
	}
	return true;

}

HRESULT	DirectionLine::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = m_iNumIndex * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = &m_IndexList.at(0);

	g_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer);
	return hr;
}

bool DirectionLine::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	//m_pImmediateContext->GSSetShader(m_pGS, NULL, 0);

	UINT stride = sizeof(PC_VERTEX);
	UINT offset = 0;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	// 상수버퍼 갱신

	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pContext->DrawIndexed(m_iNumIndex, 0, 0);
	return true;
}

DirectionLine::DirectionLine()
{
}


DirectionLine::~DirectionLine()
{
}
