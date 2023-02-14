#include "Line.h"

bool Line::CreateVertexData()
{
	m_LineVertexList.resize(2);
	m_LineVertexList[0].p = TVector3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].c = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].p = TVector3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].c = TVector4(1.0f, 0.0f, 0.0f, 1.0f);

	m_iNumVertex = m_LineVertexList.size();

	if (m_LineVertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Line::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PC_VERTEX) * m_iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	CD3D11_BUFFER_DESC cbc(sizeof(PC_VERTEX) * m_iNumVertex, D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_LineVertexList.at(0);
	hr =  g_pd3dDevice->CreateBuffer(&cbc, &InitData, &m_pVertexBuffer);

	return hr;
}

bool Line::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;

	m_iNumIndex = m_IndexList.size();

	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Line::CreateIndexBuffer()
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

HRESULT	Line::LoadShader(T_STR vertexShaderName, T_STR pixelShaderName, T_STR geometryShader)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = NULL;

	D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_pVSBlob, &error);
	hr = g_pd3dDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS);

	// Compile the PS from the file
	ID3DBlob* pPSBlob = NULL;
	D3DCompileFromFile(pixelShaderName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBlob, &error);
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPS);

	SAFE_RELEASE(pPSBlob);

	return hr;
}

HRESULT	Line::CreateVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pVertexLayout);

	SAFE_RELEASE(m_pVSBlob);

	return hr;
}

bool Line::Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor)
{
	PC_VERTEX vertices[2];
	vertices[0].p = vStart;
	vertices[0].c = vColor;
	vertices[1].p = vEnd;
	vertices[1].c = vColor;
	// 동적 리소스 갱신 방법 1 : D3D11_USAGE_DEFAULT 사용
	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &vertices, 0, 0);
	return Render(pContext);
}

bool Line::Render(ID3D11DeviceContext* pContext)
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
	pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

Line::Line()
{
}


Line::~Line()
{
}
