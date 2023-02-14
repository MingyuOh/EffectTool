#include "Object.h"

HRESULT	Object::Create(T_STR textureName, T_STR vertexShaderName, T_STR pixelShaderName, T_STR geometryShaderName)
{
	HRESULT hr = S_OK;

	if (CreateVertexData() == true)
	{
		if (FAILED(CreateVertexBuffer()))
		{
			MessageBox(0, _T("CreateVertexBuffer  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
			return hr;
		}
		m_pVertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Object class vertex buffer") - 1, "Object class vertex buffer");
	}

	if (CreateIndexData() == true)
	{
		if (FAILED(CreateIndexBuffer()))
		{
			MessageBox(0, _T("CreateIndexBuffer  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
			return hr;
		}
		m_pIndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Object class index buffer") - 1, "Object class index buffer");
	}

	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(0, _T("CreateConstantBuffer  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
		return hr;
	}
	m_pConstantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Object class constant buffer") - 1, "Object class constant buffer");

	if (FAILED(LoadShader(vertexShaderName, pixelShaderName, geometryShaderName)))
	{
		MessageBox(0, _T("LoadShader  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
		return hr;
	}
	else
	{
		if (FAILED(CreateVertexLayout()))
		{
			MessageBox(0, _T("CreateVertexLayout  ½ÇÆÐ"), _T("Fatal error"), MB_OK);
			return hr;
		}
		m_pVertexLayout->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Object class vertex layout") - 1, "Object class vertex layout");
	}

	if (FAILED(LoadTexture(textureName)))
	{
		MessageBox(0, _T("Texture Load ½ÇÆÐ"), _T("Fatal error"), MB_OK);
		return hr;
	}

	return hr;
}

bool Object::CreateVertexData()
{
	if (m_VertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Object::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	if (m_VertexList.empty() == true)
	{
		return S_FALSE;
	}

	m_iNumVertex = m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_VertexList.at(0);
	return g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
}

bool Object::CreateIndexData()
{
	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Object::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	if (m_IndexList.size() <= 0) return hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return hr;

	m_iNumIndex = m_IndexList.size();
	return hr;
}


HRESULT	Object::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(VSCBMatrix);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_cbData;

	hr = g_pd3dDevice->CreateBuffer(&cbDesc, &data, &m_pConstantBuffer);
	return hr;
}

HRESULT	Object::LoadShader(T_STR vertexShaderName, T_STR pixelShaderName, T_STR geometryShaderName)
{
	HRESULT hr = S_OK; 
	ID3DBlob* error = nullptr;

	hr = D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_pVSBlob, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS);

	// Compile the PS from the file
	ID3DBlob* pPSBlob = NULL;
	hr = D3DCompileFromFile(pixelShaderName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBlob, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pMainPS);
	//m_pMainPS = m_pPS;

	// Compile the GS from the file
	ID3DBlob* pGSBlob = NULL;
	if (geometryShaderName.compare(_T("")) != 0)
	{
		hr = D3DCompileFromFile(geometryShaderName.c_str(), nullptr, nullptr, "GS", "gs_5_0", 0, 0, &pGSBlob, &error);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
			return hr;
		}
		g_pd3dDevice->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &m_pGS);
	}

	SAFE_RELEASE(pPSBlob);
	SAFE_RELEASE(pGSBlob);
	return hr;
}

HRESULT	Object::CreateVertexLayout()
{
	HRESULT hr = S_OK;
	// Create our vertex input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pVertexLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	SAFE_RELEASE(m_pVSBlob);
	return hr;
}

HRESULT	Object::LoadTexture(T_STR szFilePath)
{
	HRESULT hr = S_OK;
	if (szFilePath.empty() == true)
	{
		return hr;
	}
	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture();
	}

	string szFileName = wtm(szFilePath);
	char Drive[MAX_PATH] = { 0, };
	char Dir[MAX_PATH] = { 0, };
	char FileName[MAX_PATH] = { 0, };
	char Ext[MAX_PATH] = { 0, };

	_splitpath(szFileName.c_str(), Drive, Dir, FileName, Ext);
	Ext[4] = '\0';

	m_pTexture->LoadTexture(szFilePath, mtw(Ext));

	return hr;
}

bool Object::ComputeBoungigBox(TMatrix* matWorld)
{
	if (m_VertexList.empty())
	{
		return false;
	}

	TVector3 vAxis[3];
	m_BoundingBox.vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	m_BoundingBox.vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	m_BoundingBox.vAxis[2] = TVector3(0.0f, 0.0f, 1.0f);

	TMatrix mat = TMatrix::Identity;
	if (matWorld != nullptr)
	{
		mat = *matWorld;
	}

	// ±âÀúº¤ÅÍ È¸Àü
	D3DXVec3TransformCoord(&m_BoundingBox.vAxis[0], &m_BoundingBox.vAxis[0], &mat);
	D3DXVec3TransformCoord(&m_BoundingBox.vAxis[1], &m_BoundingBox.vAxis[1], &mat);
	D3DXVec3TransformCoord(&m_BoundingBox.vAxis[2], &m_BoundingBox.vAxis[2], &mat);

	m_BoundingBox.vMax = m_VertexList[0].p;
	m_BoundingBox.vMin = m_VertexList[0].p;
	for (int index = 1; index < m_VertexList.size(); index++)
	{
		// Max position
		if (m_BoundingBox.vMax.x < m_VertexList[index].p.x)
		{
			m_BoundingBox.vMax.x = m_VertexList[index].p.x;
		}
		if (m_BoundingBox.vMax.y < m_VertexList[index].p.y)
		{
			m_BoundingBox.vMax.y = m_VertexList[index].p.y;
		}
		if (m_BoundingBox.vMax.z < m_VertexList[index].p.z)
		{
			m_BoundingBox.vMax.z = m_VertexList[index].p.z;
		}

		// Min position
		if (m_BoundingBox.vMin.x > m_VertexList[index].p.x)
		{
			m_BoundingBox.vMin.x = m_VertexList[index].p.x;
		}
		if (m_BoundingBox.vMin.y > m_VertexList[index].p.y)
		{
			m_BoundingBox.vMin.y = m_VertexList[index].p.y;
		}
		if (m_BoundingBox.vMin.z > m_VertexList[index].p.z)
		{
			m_BoundingBox.vMin.z = m_VertexList[index].p.z;
		}
	}

	m_BoundingBox.vCenter.x = (m_BoundingBox.vMax.x + m_BoundingBox.vMin.x) / 2;
	m_BoundingBox.vCenter.y = (m_BoundingBox.vMax.y + m_BoundingBox.vMin.y) / 2;
	m_BoundingBox.vCenter.z = (m_BoundingBox.vMax.z + m_BoundingBox.vMin.z) / 2;

	m_BoundingBox.fLength[0] = fabsf(m_BoundingBox.vCenter.x - m_BoundingBox.vMax.x);
	m_BoundingBox.fLength[1] = fabsf(m_BoundingBox.vCenter.y - m_BoundingBox.vMax.y);
	m_BoundingBox.fLength[2] = fabsf(m_BoundingBox.vCenter.z - m_BoundingBox.vMax.z);

	return true;
}

bool Object::UpdateBoundingBox(CollisionBox& box, TMatrix& matrix)
{
	return true;
}

bool Object::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != NULL)
	{
		m_matWorld = *matWorld;
	}
	
	if (matView != NULL)
	{
		m_matView = *matView;
	}

	if (matProj != NULL)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_cbData.matNormal = m_cbData.matNormal.Transpose();

	return true;
}

void Object::SetPosition(TVector3 vPos)
{
	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
}

bool Object::Init()
{
	m_matWorld = TMatrix::Identity;
	m_matView = TMatrix::Identity;
	m_matProj = TMatrix::Identity;
	return true;
}

bool Object::Frame()
{
	m_vLook.x = m_matWorld._13;
	m_vLook.y = m_matWorld._23;
	m_vLook.z = m_matWorld._33;

	return true;
}

bool Object::PreRender(ID3D11DeviceContext* pContext)
{
	if (m_VertexList.size() < 0)
	{
		return false;
	}

	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pMainPS, NULL, 0);
	if (m_pGS != nullptr)
	{
		pContext->GSSetShader(m_pGS, NULL, 0);
	}
	pContext->IASetInputLayout(m_pVertexLayout);

	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);


	return true;
}

bool Object::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext, m_iNumIndex);
	return true;
}

bool Object::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int index = 0; index < m_pTexture->m_pTextureSRV.size(); index++)
	{
		if (m_pTexture->m_pTextureSRV[index] == nullptr)
		{
			continue;
		}
		pContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV[index]);
	}

	if (iNumIndex == 0)
	{
		iNumIndex = m_iNumIndex;
	}
	if (iNumIndex != 0)
	{
		pContext->DrawIndexed(iNumIndex, 0, 0);
	}
	else
	{
		pContext->Draw(m_iNumVertex, 0);
	}

	return true;
}

bool Object::Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 dwColor)
{
	return true;
}

bool Object::Release()
{
	SAFE_RELEASE(m_pVertexLayout);		// Á¤Á¡·¹ÀÌ¾Æ¿ô ¼Ò¸ê
	SAFE_RELEASE(m_pVertexBuffer);		// Á¤Á¡¹öÆÛ ¼Ò¸ê
	SAFE_RELEASE(m_pIndexBuffer);		// ÀÎµ¦½º¹öÆÛ ¼Ò¸ê
	SAFE_RELEASE(m_pConstantBuffer);	// »ó¼ö¹öÆÛ ¼Ò¸ê
	SAFE_RELEASE(m_pVS);				// Á¤Á¡ ¼ÎÀÌ´õ ¼Ò¸ê
	if (m_pMainPS != nullptr)
	{
		SAFE_RELEASE(m_pMainPS);			// »ç¿ë ÁßÀÎ ÇÈ¼¿ ¼ÎÀÌ´õ ¼Ò¸ê
	}
	if (m_pPS != nullptr)
	{
		SAFE_RELEASE(m_pPS);				// ÇÈ¼¿ ¼ÎÀÌ´õ ¼Ò¸ê
	}
	SAFE_RELEASE(m_pGS);				// ±âÇÏ ¼ÎÀÌ´õ ¼Ò¸ê
	if(m_pTexture) m_pTexture->Release();
	return true;
}

Object::Object()
{
	SAFE_ZERO(m_pVertexBuffer);
	SAFE_ZERO(m_pIndexBuffer);
	SAFE_ZERO(m_pVertexLayout);
	SAFE_ZERO(m_pConstantBuffer);
	SAFE_ZERO(m_pVS);
	SAFE_ZERO(m_pPS);
	SAFE_ZERO(m_pMainPS);
	SAFE_ZERO(m_pGS);
	SAFE_ZERO(m_pVSBlob);
	SAFE_ZERO(m_pTexture);
	m_iNumVertex = 0;
	m_iNumIndex = 0;
	m_InitBox.vMax = TVector3(1.0f, 1.0f, 1.0f);
	m_InitBox.vMin = TVector3(-1.0f, -1.0f, -1.0f);
}

Object::~Object() {}

