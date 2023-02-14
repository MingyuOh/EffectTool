#include "Mesh.h"

HRESULT	Mesh::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	Object::CreateVertexBuffer();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCTIW_VERTEX) * m_WeightList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &m_WeightList.at(0);

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIWVertrexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

HRESULT	Mesh::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	hr = Object::CreateConstantBuffer();

	if (m_VertexList.size() <= 0)
	{
		return hr;
	}
	else
	{
		int kk = 0;
	}

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(AnimMatrix);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_matAnimMatrix;

	hr = g_pd3dDevice->CreateBuffer(&cbDesc, &data, &m_pAnimConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;

}

HRESULT	Mesh::CreateVertexLayout()
{
	HRESULT hr = S_OK;

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	g_pd3dDevice->CreateInputLayout(layout, numElements, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pVertexLayout);

	m_pVSBlob->Release();
	return hr;
}

bool Mesh::PreRender(ID3D11DeviceContext* pContext)
{
	if (m_VertexList.size() <= 0)
	{
		return false;
	}

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->UpdateSubresource(m_pAnimConstantBuffer, 0, NULL, &m_matAnimMatrix, 0, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->VSSetConstantBuffers(1, 1, &m_pAnimConstantBuffer);

	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pMainPS, NULL, 0);
	if (m_pGS != nullptr)
	{
		pContext->GSSetShader(m_pGS, NULL, 0);
	}
	pContext->IASetInputLayout(m_pVertexLayout);

	UINT strides[2] = { sizeof(PNCT_VERTEX), sizeof(PNCTIW_VERTEX) };
	UINT offsets[2] = { 0, 0 };

	ID3D11Buffer* vertexBuffers[2] = { m_pVertexBuffer, m_pIWVertrexBuffer };
	pContext->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return true;
}

bool Mesh::Release()
{
	Object::Release();

	if(m_pShape) m_pShape->Release();
	SAFE_RELEASE(m_pIWVertrexBuffer);
	SAFE_RELEASE(m_pAnimConstantBuffer);

	for (auto& data : m_pSubMesh)
	{
		data->Release();
		SAFE_DEL(data);
	}

	return true;
}

Mesh::Mesh()
{
	SAFE_ZERO(m_pShape);
	SAFE_ZERO(m_pIWVertrexBuffer);
	SAFE_ZERO(m_pAnimConstantBuffer);
	m_ClassType = OBJECT_CLASS_TYPE::CLASS_GEOM;
}

Mesh::~Mesh()
{
}