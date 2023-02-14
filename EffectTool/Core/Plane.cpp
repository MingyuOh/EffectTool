#include "Plane.h"

bool DXCore::Plane::CreateVertexData()
{
	m_VertexList.resize(4);

	if (m_iVertexType == 1)
	{
		m_VertexList[0] = { TVector3(-1.0f, 0.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
		m_VertexList[1] = { TVector3(1.0f, 0.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
		m_VertexList[2] = { TVector3(-1.0f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f) };
		m_VertexList[3] = { TVector3(1.0f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	}
	else
	{
		m_VertexList[0] = { TVector3(-1.0f, 1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f) };
		m_VertexList[1] = { TVector3(1.0f, 1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f) };
		m_VertexList[2] = { TVector3(-1.0f, -1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f) };
		m_VertexList[3] = { TVector3(1.0f, -1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f) };
	}

	if (m_VertexList.empty() == true)
	{
		return false;
	}
	return true;
}

HRESULT	DXCore::Plane::CreateVertexBuffer()
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
	hr = g_pd3dDevice->CreateBuffer(&cbc, &InitData, &m_pVertexBuffer);

	return hr;
}

bool DXCore::Plane::CreateIndexData()
{
	m_IndexList.resize(6);
	m_iNumIndex = m_IndexList.size();
	int iIndex = 0;

	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 3;

	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	DXCore::Plane::CreateIndexBuffer()
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

bool DXCore::Plane::SetTextureArray()
{
	T_STR multiTexture[]
	{
		L"../data/Texture/MultiTexture/DetailMap_Red.dds",
		L"../data/Texture/MultiTexture/DetailMap_Green.dds",
		L"../data/Texture/MultiTexture/DetailMap_Blue.dds",
		L"../data/Texture/MultiTexture/DetailMap_Alpha.dds"
	};

	Texture* pTexture = new Texture();
	for (int index = 0; index < _countof(multiTexture); index++)
	{
		pTexture->LoadDDS(multiTexture[index]);
	}

	return true;
}

DXCore::Plane::Plane()
{
	m_iVertexType = 0;
}


DXCore::Plane::~Plane()
{
}
