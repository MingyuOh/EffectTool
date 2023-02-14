#include "PointParticle.h"


HRESULT PointParticle::LoadShaderAndInputLayout(T_STR vertexShaderName, T_STR PixelShaderName, T_STR geometryShaderName)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;

	ID3DBlob* pVSBuf = NULL;
	hr = D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &pVSBuf, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS);

	// Compile the PS from the file
	ID3DBlob* pPSBuf = NULL;
	hr = D3DCompileFromFile(PixelShaderName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBuf, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS);

	// Compile the GS from the file
	ID3DBlob* pGSBuf = NULL;
	hr = D3DCompileFromFile(geometryShaderName.c_str(), nullptr, nullptr, "GS", "gs_5_0", 0, 0, &pGSBuf, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreateGeometryShader(pGSBuf->GetBufferPointer(), pGSBuf->GetBufferSize(), NULL, &m_pGS);

	ID3DBlob* pGSPointBuf = NULL;
	hr = D3DCompileFromFile(geometryShaderName.c_str(), nullptr, nullptr, "GSAdvanceParticlesMain", "gs_5_0", 0, 0, &pGSPointBuf, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreateGeometryShader(pGSPointBuf->GetBufferPointer(), pGSPointBuf->GetBufferSize(), NULL, m_pGSAdvanceParticlesMain.GetAddressOf());

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TIMER", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout)))
	{
		return hr;
	}

	D3D11_SO_DECLARATION_ENTRY pDecl[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "NORMAL", 0, 0, 3, 0 },
		{ 0, "TIMER", 0, 0, 1, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
	};

	int iEntry = sizeof(pDecl);
	int size = ARRAYSIZE(pDecl);
	UINT stride = 8 * sizeof(float);
	UINT elems = sizeof(pDecl) / sizeof(D3D11_SO_DECLARATION_ENTRY);

	UINT strides[] = { sizeof(PARTICLE_VERTEX) };
	if (FAILED(hr = g_pd3dDevice->CreateGeometryShaderWithStreamOutput(
		(void*)pGSPointBuf->GetBufferPointer(),
		pGSPointBuf->GetBufferSize(),
		pDecl,
		4,
		strides,
		1,
		D3D11_SO_NO_RASTERIZED_STREAM,
		NULL,
		m_pGSStreamOutput.GetAddressOf())))
	{
		return hr;
	}


	hr = D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VSPassThroughmain", "vs_5_0", 0, 0, &pVSBuf, &error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(), "error", MB_OK);
		return hr;
	}
	g_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, m_pVSPassThroughMain.GetAddressOf());


	SAFE_RELEASE(pVSBuf);
	SAFE_RELEASE(pPSBuf);
	SAFE_RELEASE(pGSBuf);
	SAFE_RELEASE(pGSPointBuf);
	return hr;
}

HRESULT	PointParticle::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	PARTICLE_VERTEX vertices[] =
	{
		{
			TVector3(0, 0, 0),
			TVector3(0, 30, 0),
			float(0),
			UINT(0)
		}
	};

	m_iNumVertex = 1;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = sizeof(PARTICLE_VERTEX) * m_iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return hr;
	}

	D3D11_BUFFER_DESC vbdesc = 
	{
		1 * sizeof(PARTICLE_VERTEX),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,
		0,
		0
	};
	vbdesc.ByteWidth = MAX_PARTICLES * sizeof(PARTICLE_VERTEX);

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pParticleDrawFrom.GetAddressOf())))
	{
		return hr;
	}

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pParticleStreamTo.GetAddressOf())))
	{
		return hr;
	}

	return hr;
}

HRESULT	PointParticle::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	// 상수버퍼(각각 행렬 정보)
	Shape::CreateConstantBuffer();

	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = 1 * sizeof(PARTICLE_CONSTANT_BUFFER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	
	InitData.pSysMem = &m_cbParticleData;
	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, m_pConstantPointBuffer.GetAddressOf())))
	{
		return hr;
	}

	return hr;
}

bool PointParticle::Init()
{
	if (FAILED(CreateRandomTexture()))
	{
		return false;
	}
	m_iNumIndex = 1;
	return true;
}

bool PointParticle::Frame(ID3D11DeviceContext* pContext, float fElapsedTime, float fSecPerFrame)
{
	// Advance the Particles
	TVector3 vGravity(0, -9.8f, 0);
	AdvanceParticles(pContext, fElapsedTime, fSecPerFrame, vGravity);
	return true;
}

bool PointParticle::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pContext->IASetInputLayout(m_pVertexLayout);
	UINT stride = m_iNumVertex;
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	pContext->GSSetShader(m_pGS, NULL, 0);

	// Set IA parameters
	ID3D11Buffer* pBuffers[1] = { m_pParticleDrawFrom.Get() };
	stride = sizeof(PARTICLE_VERTEX);
	offset = 0;
	pContext->IASetVertexBuffers(0, 1, pBuffers, &stride, &offset);

	// Set Effects Parameters
	TMatrix matWorld, matBillboard;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matBillboard);

	matBillboard = m_pCamera->m_matView;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);
	matBillboard._41 = 0;
	matBillboard._42 = 0;
	matBillboard._43 = 0;
	matBillboard._44 = 1;
	m_matWorld = matWorld * matBillboard;

	// 상수버퍼 갱신
	SetMatrix(&m_matWorld, &m_pCamera->m_matView, &m_pCamera->m_matProj);
	m_cbData.vColor.w = 1;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	
	pContext->UpdateSubresource(m_pConstantPointBuffer.Get(), 0, NULL, &m_cbParticleData, 0, 0);
	pContext->GSSetConstantBuffers(1, 1, m_pConstantPointBuffer.GetAddressOf());
	pContext->PSSetShaderResources(1, 1, m_pRandomTexRV.GetAddressOf());

	pContext->DrawAuto();
	return true;
}

HRESULT	PointParticle::CreateResource()
{
	m_bFirst = true;
	return S_OK;
}

//--------------------------------------------------------------------------------------
// This helper function creates a 1D texture full of random vectors.  The shader uses
// the current time value to index into this texture to get a random vector.
//--------------------------------------------------------------------------------------
HRESULT PointParticle::CreateRandomTexture()
{
	HRESULT hr = S_OK;

	int iNumRnadValues = 1024;
	srand(timeGetTime());

	// Create Data
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = new float[iNumRnadValues * 4];
	if (!InitData.pSysMem)
		return E_OUTOFMEMORY;
	InitData.SysMemPitch = iNumRnadValues * 4 * sizeof(float);
	InitData.SysMemSlicePitch = iNumRnadValues * 4 * sizeof(float);

	for (int i = 0; i < iNumRnadValues * 4; i++)
		((float*)InitData.pSysMem)[i] = float((rand() % 10000) - 5000);

	// Create Texture
	D3D11_TEXTURE1D_DESC dstex;
	dstex.Width = iNumRnadValues;
	dstex.MipLevels = 1;
	dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	dstex.Usage = D3D11_USAGE_DEFAULT;
	dstex.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	dstex.CPUAccessFlags = 0;
	dstex.MiscFlags = 0;
	dstex.ArraySize = 1;
	if (FAILED(hr = g_pd3dDevice->CreateTexture1D(&dstex, &InitData, m_pRandomTexture.GetAddressOf())))
	{
		return hr;
	}

	SAFE_DELETE_ARRAY(InitData.pSysMem);

	// Create the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = dstex.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	SRVDesc.Texture2D.MipLevels = dstex.MipLevels;
	if (FAILED(hr = g_pd3dDevice->CreateShaderResourceView(m_pRandomTexture.Get(), &SRVDesc, m_pRandomTexRV.GetAddressOf())))
	{
		return hr;
	}

	return hr;
}

bool PointParticle::AdvanceParticles(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime, TVector3 vGravity)
{
	// Set IA parameters
	ID3D11Buffer* pBuffers[1];
	if (m_bFirst)
	{
		pBuffers[0] = m_pVertexBuffer;
	}
	else
	{
		pBuffers[0] = m_pParticleDrawFrom.Get();
	}

	UINT stride = sizeof(PARTICLE_VERTEX);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, pBuffers, &stride, &offset);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Point to the correct output buffer
	pBuffers[0] = m_pParticleStreamTo.Get();
	pContext->SOSetTargets(1, pBuffers, &offset);

	// Set Effects Parameters
	m_cbParticleData.m_fGlobalTime = fGlobalTime;
	m_cbParticleData.m_fElapsedTime = fElapsedTime;
	vGravity *= fElapsedTime;

	m_cbParticleData.m_vFrameGravity.x = vGravity.x;
	m_cbParticleData.m_vFrameGravity.y = vGravity.y;
	m_cbParticleData.m_vFrameGravity.z = vGravity.z;
	m_cbParticleData.m_vFrameGravity.w = 1.0f;

	m_cbParticleData.m_fSecondsPerFirework = 0.4f;
	m_cbParticleData.m_fNumEmber1s = 100;
	m_cbParticleData.m_fMaxEmber2s = 15.0f;

	pContext->UpdateSubresource(m_pConstantPointBuffer.Get(), 0, NULL, &m_cbParticleData, 0, 0);
	pContext->GSSetConstantBuffers(1, 1, m_pConstantPointBuffer.GetAddressOf());
	pContext->GSSetShaderResources(1, 1, m_pRandomTexRV.GetAddressOf());

	pContext->VSSetShader(m_pVSPassThroughMain.Get(), NULL, 0);
	pContext->GSSetShader(m_pGSStreamOutput.Get(), NULL, 0);
	pContext->PSSetShader(nullptr, NULL, 0);

	// Draw
	if (m_bFirst)
		pContext->Draw(1, 0);
	else
		pContext->DrawAuto();

	// Get back to normal
	pBuffers[0] = NULL;
	pContext->SOSetTargets(1, pBuffers, &offset);

	// Swap particle buffers
	ComPtr<ID3D11Buffer> pTemp = m_pParticleDrawFrom;
	m_pParticleDrawFrom = m_pParticleStreamTo;
	m_pParticleStreamTo = pTemp;

	m_bFirst = false;
	return true;
}

void PointParticle::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

PointParticle::PointParticle()
{
	m_bFirst = true;
}


PointParticle::~PointParticle()
{
}
