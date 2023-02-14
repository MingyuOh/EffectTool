#include "BoundingBoxShape.h"

bool BoundingBoxShape::CreateVertexData()
{
	m_BoundingVertexList.resize(8);
	m_BoundingVertexList[0] = { m_BoundingBox.vPos[0], m_BoundingBox.vColor };
	m_BoundingVertexList[1] = { m_BoundingBox.vPos[1], m_BoundingBox.vColor };
	m_BoundingVertexList[2] = { m_BoundingBox.vPos[2], m_BoundingBox.vColor };
	m_BoundingVertexList[3] = { m_BoundingBox.vPos[3], m_BoundingBox.vColor };

	m_BoundingVertexList[4] = { m_BoundingBox.vPos[4], m_BoundingBox.vColor };
	m_BoundingVertexList[5] = { m_BoundingBox.vPos[5], m_BoundingBox.vColor };
	m_BoundingVertexList[6] = { m_BoundingBox.vPos[6], m_BoundingBox.vColor };
	m_BoundingVertexList[7] = { m_BoundingBox.vPos[7], m_BoundingBox.vColor };

	m_iNumVertex = m_BoundingVertexList.size();

	if (m_BoundingVertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	BoundingBoxShape::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PC_VERTEX) * m_iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	bd.CPUAccessFlags = 0;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_BoundingVertexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	return hr;
}

bool BoundingBoxShape::CreateIndexData()
{
	m_IndexList.resize(32);
	int index = 0;
	// 앞
	m_IndexList[index++] = 0; m_IndexList[index++] = 1;
	m_IndexList[index++] = 1; m_IndexList[index++] = 2;
	m_IndexList[index++] = 2; m_IndexList[index++] = 3;
	m_IndexList[index++] = 3; m_IndexList[index++] = 0;

	// 뒤
	m_IndexList[index++] = 4; m_IndexList[index++] = 5;
	m_IndexList[index++] = 5; m_IndexList[index++] = 6;
	m_IndexList[index++] = 6; m_IndexList[index++] = 7;
	m_IndexList[index++] = 7; m_IndexList[index++] = 4;

	// 위
	m_IndexList[index++] = 1; m_IndexList[index++] = 5;
	m_IndexList[index++] = 6; m_IndexList[index++] = 2;

	// 아래
	m_IndexList[index++] = 0; m_IndexList[index++] = 4;
	m_IndexList[index++] = 7; m_IndexList[index++] = 3;

	// 왼쪽
	m_IndexList[index++] = 5; m_IndexList[index++] = 1;
	m_IndexList[index++] = 0; m_IndexList[index++] = 4;

	// 오른쪽
	m_IndexList[index++] = 2; m_IndexList[index++] = 6;
	m_IndexList[index++] = 7; m_IndexList[index++] = 3;

	m_iNumIndex = m_IndexList.size();

	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	BoundingBoxShape::CreateIndexBuffer()
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

HRESULT	BoundingBoxShape::LoadShader(T_STR vertexShaderName, T_STR pixelShaderName, T_STR geometryShader)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = NULL;

	D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_pVSBlob, &error);
	g_pd3dDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS);

	// Compile the PS from the file
	ID3DBlob* pPSBlob = NULL;
	D3DCompileFromFile(pixelShaderName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBlob, &error);
	g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPS);

	SAFE_RELEASE(pPSBlob);
	return hr;
}

HRESULT	BoundingBoxShape::CreateVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	g_pd3dDevice->CreateInputLayout(layout, numElements, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pVertexLayout);

	SAFE_RELEASE(m_pVSBlob);
	return hr;
}

bool BoundingBoxShape::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);

	UINT stride = sizeof(PC_VERTEX);
	UINT offset = 0;
	D3D11_MAPPED_SUBRESOURCE resource;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
}

BoundingBoxShape::BoundingBoxShape()
{
	m_BoundingBox.vColor = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
}


BoundingBoxShape::~BoundingBoxShape()
{
}
