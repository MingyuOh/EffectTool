#include "ParticleSystem.h"

//--------------------------------------------------------------------------------------
// 인스턴스 버퍼 생성
//--------------------------------------------------------------------------------------
bool ParticleSystem::CreateInstanceBuffer()
{
	SAFE_NEW_ARRAY(m_pInstData, Instance, m_iEffectCount);
	for (int iCnt = 0; iCnt < m_iEffectCount; iCnt++)
	{
		D3DXMatrixTranslation(&m_pInstData[iCnt].matWorld, m_Particle[iCnt].m_vPos.x, m_Particle[iCnt].m_vPos.y, m_Particle[iCnt].m_vPos.z);
		D3DXMatrixTranspose(&m_pInstData[iCnt].matWorld, &m_pInstData[iCnt].matWorld);
		m_pInstData[iCnt].m_vColor = m_Particle[iCnt].m_vColor;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Instance) * m_iEffectCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)m_pInstData;
	g_pd3dDevice->CreateBuffer(&bd, &InitData, m_pInstanceBuffer.GetAddressOf());
	return true;
}

//--------------------------------------------------------------------------------------
// 레이아웃 및 쉐이더 생성
//--------------------------------------------------------------------------------------
bool ParticleSystem::LoadShaderAndInputLayout(T_STR vertexShaderName, T_STR PixelShaderName, T_STR geometryShaderName)
{
	HRESULT hr = S_OK;
	
	ComPtr<ID3DBlob> pVSBuf = NULL;
	ID3DBlob* error = 0;
	if (FAILED(D3DCompileFromFile(vertexShaderName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &pVSBuf, &error)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer,
			(char*)error->GetBufferPointer(),
			256);
		MessageBox(NULL, buffer, _T("pVSBuffer"), MB_OK);
	}
	g_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_Effect.m_pVS);

	// Compile the PS from the file
	ComPtr<ID3DBlob> pPSBuf = NULL;
	if (FAILED(D3DCompileFromFile(PixelShaderName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBuf, &error)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer,
			(char*)error->GetBufferPointer(),
			256);
		MessageBox(NULL, buffer, _T("pPSBuffer"), MB_OK);
	}
	g_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_Effect.m_pMainPS);

	//// Compile the GS from the file
	//ComPtr<ID3DBlob> pGSBuf = NULL;
	//D3DX11CompileFromFile(geometryShaderName, NULL, NULL, "GS", "gs_5_0", 0, NULL, NULL, &pGSBuf, NULL, NULL);
	//m_pd3dDevice->CreateGeometryShader(pGSBuf->GetBufferPointer(), pGSBuf->GetBufferSize(), NULL, &m_Effect->m_pGS);

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// 인스턴스 버퍼
		{ "mTransform",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	   1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 1, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	   1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 3, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "PARTICLECOLOR",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	
	if (FAILED(hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), m_pLayout.GetAddressOf())))
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// 디바이스 / 카메라 초기화 함수
//--------------------------------------------------------------------------------------
bool ParticleSystem::Set(Camera* pMainCamera)
{
	//m_pd3dDevice.Attach(pDevice);
	//m_pImmediateContext.Attach(pImmediateContext);
	m_pCamera = pMainCamera;
	return true;
}

//--------------------------------------------------------------------------------------
// Create
//--------------------------------------------------------------------------------------
bool ParticleSystem::Create()
{
	m_Particle.resize(m_iEffectCount);
	D3DXMatrixIdentity(&m_matWorld);

	CreateInstanceBuffer();
	LoadShaderAndInputLayout(L"../data/Shader/EffectInstanceShader.hlsl", L"../data/Shader/EffectShader.hlsl", L"../data/Shader/GeometryShader.gsh");

	return true;
}

//--------------------------------------------------------------------------------------
// Create Bounding Box
//--------------------------------------------------------------------------------------
bool ParticleSystem::CreateBoundingBox(TVector3 vBoundingBoxMin, TVector3 vBoundingBoxMax)
{
	m_BoundingBoxShape.m_BoundingBox.vMax = vBoundingBoxMax;
	m_BoundingBoxShape.m_BoundingBox.vMin = vBoundingBoxMin;
	m_BoundingBoxShape.CreateAABBBox(vBoundingBoxMax, vBoundingBoxMin);

	m_BoundingPlane[0].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[5],
		m_BoundingBoxShape.m_BoundingBox.vPos[0],
		m_BoundingBoxShape.m_BoundingBox.vPos[1]);	// 좌 평면(left)
	m_BoundingPlane[1].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[3],
		m_BoundingBoxShape.m_BoundingBox.vPos[6],
		m_BoundingBoxShape.m_BoundingBox.vPos[2]);	// 우 평면(right)
	m_BoundingPlane[2].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[5],
		m_BoundingBoxShape.m_BoundingBox.vPos[2],
		m_BoundingBoxShape.m_BoundingBox.vPos[6]);	// 상 평면(top)
	m_BoundingPlane[3].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[0],
		m_BoundingBoxShape.m_BoundingBox.vPos[7],
		m_BoundingBoxShape.m_BoundingBox.vPos[3]);	// 하 평면(bottom)
	m_BoundingPlane[4].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[0],
		m_BoundingBoxShape.m_BoundingBox.vPos[2],
		m_BoundingBoxShape.m_BoundingBox.vPos[1]);	// 근 평면(near)
	m_BoundingPlane[5].CreatePlane(
		m_BoundingBoxShape.m_BoundingBox.vPos[6],
		m_BoundingBoxShape.m_BoundingBox.vPos[4],
		m_BoundingBoxShape.m_BoundingBox.vPos[5]);	// 원 평면(far)

	m_BoundingBoxShape.Create(_T(""), _T("../data/shader/LineVertexShader.vsh"), _T("../data/shader/LinePixelShader.psh"));

	return true;
}

//--------------------------------------------------------------------------------------
// 파티클 업데이트 함수
//--------------------------------------------------------------------------------------
void ParticleSystem::ParticleUpdate(ID3D11DeviceContext* pContext)
{
	m_Effect.Update();

	m_Effect.m_VertexList[0].t = m_Effect.GetDrawRect(0);
	m_Effect.m_VertexList[1].t = m_Effect.GetDrawRect(1);
	m_Effect.m_VertexList[2].t = m_Effect.GetDrawRect(2);
	m_Effect.m_VertexList[3].t = m_Effect.GetDrawRect(3);
	pContext->UpdateSubresource(m_Effect.m_pVertexBuffer, 0, NULL, &m_Effect.m_VertexList.at(0), 0, 0);

	TMatrix matWorld, matRotate, matBillboard;
	TMatrix matView, matProj;
	D3DXMatrixIdentity(&matBillboard);
	// 빌보드행렬 적용
	if (m_bBillboard == TRUE)
	{
		matBillboard = m_pCamera->m_matView;
		D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);
		matBillboard._41 = 0;
		matBillboard._42 = 0;
		matBillboard._43 = 0;
		matBillboard._44 = 1;
	}

	// 직교투영 적용
	if (m_bOrthogonal == TRUE)
	{
		TVector3 vEye = TVector3(0.0f, 0.0f, -1.0f);
		TVector3 vAt = TVector3(0.0f, 0.0f, 0.0f);
		TVector3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
		int iRectWidth = g_iWindowWidth / 2;
		int iRectHeight = g_iWindowHeight / 2;
		// 화면 중앙이 원점으로 계산되기 때문에 넓이 및 높이가 -1 ~ 1 범위로 직교투영된다. 
		D3DXMatrixOrthoOffCenterLH(&matProj, -iRectWidth, iRectWidth, -iRectHeight, iRectHeight, 0.0f, 1000.0f);
	}

	for (int iCnt = 0; iCnt < m_Particle.size(); iCnt++)
	{
		TVector3 vPos = { 0.0f, 0.0f, 0.0f };
		D3DXMatrixIdentity(&matWorld);

		if (m_EffectType == EFFECT_TYPE::Straight)
		{
			// 방향벡터와 속도로부터 위치 갱신
			vPos = m_Particle[iCnt].m_vPos + (m_Particle[iCnt].m_vDirection * (m_Particle[iCnt].m_vSpeed * g_fSecPerFrame));
		}
		else if(m_EffectType == EFFECT_TYPE::Rotate)
		{
			matRotate._41 = m_Particle[iCnt].m_vPos.x;
			matRotate._42 = m_Particle[iCnt].m_vPos.y;
			matRotate._43 = m_Particle[iCnt].m_vPos.z;
			Rotate(matRotate, m_Particle[iCnt].m_fRadian);
		}
		else if (m_EffectType == EFFECT_TYPE::Circle)
		{
			vPos = m_Particle[iCnt].m_vPos
				+ ((Circle(m_Particle[iCnt].m_fRadian, g_fElapseTime * m_Particle[iCnt].m_vSpeed.x)
					+ m_Particle[iCnt].m_vDirection) * (m_Particle[iCnt].m_vSpeed * g_fSecPerFrame));
		}
		m_Particle[iCnt].m_vPos = vPos;

		// 이펙트 크기 적용
		D3DXMatrixScaling(&matWorld, m_Particle[iCnt].m_vScale.x, m_Particle[iCnt].m_vScale.y, m_Particle[iCnt].m_vScale.z);
		matWorld = matWorld * matBillboard * matRotate;

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		// 상수버퍼 갱신
		if(m_bOrthogonal == TRUE)
			m_Effect.SetMatrix(&matWorld, &matView, &matProj);
		else
			m_Effect.SetMatrix(&matWorld, &m_pCamera->m_matView, &m_pCamera->m_matProj);

		m_Effect.m_cbData.vColor.w = m_Effect.m_pTexture->m_pTextureSRV.size();

		// 인스턴스 상수버퍼 갱신
		m_pInstData[iCnt].m_vColor = m_Particle[iCnt].m_vColor;
		D3DXMatrixTranspose(&m_pInstData[iCnt].matWorld, &matWorld);
	}
}

//--------------------------------------------------------------------------------------
// 회전 함수
//--------------------------------------------------------------------------------------
void ParticleSystem::Rotate(TMatrix& matRotate, float fAngle)
{
	if (m_CoordinateType == COORDINATE_TYPE::RotationX)
	{
		D3DXMatrixRotationX(&matRotate, fAngle * g_fElapseTime);
	}
	else if (m_CoordinateType == COORDINATE_TYPE::RotationY)
	{
		D3DXMatrixRotationY(&matRotate, fAngle * g_fElapseTime);
	}
	else if(m_CoordinateType == COORDINATE_TYPE::RotationZ)
	{
		D3DXMatrixRotationZ(&matRotate, fAngle * g_fElapseTime);
	}
}

//--------------------------------------------------------------------------------------
// 원운동 함수
//--------------------------------------------------------------------------------------
TVector3 ParticleSystem::Circle(float fRadius, float fRadian)
{
	TVector3 vRet;
	if (m_CoordinateType == COORDINATE_TYPE::RotationX)
	{
		vRet.x = 0.0f;
		vRet.y = fRadius * sinf(fRadian);
		vRet.z = fRadius * cosf(fRadian);
	}
	else if (m_CoordinateType == COORDINATE_TYPE::RotationY)
	{
		vRet.x = fRadius * sinf(fRadian);
		vRet.y = 0.0f;
		vRet.z = fRadius * cosf(fRadian);
	}
	else
	{
		vRet.x = fRadius * sinf(fRadian);
		vRet.y = fRadius * cosf(fRadian);
		vRet.z = 0.0f;
	}
	return vRet;
}

void ParticleSystem::CheckEffectRangeFromBox()
{
	float time = (EFFECT_EXTINCTION_DISTANCE / 2.0f) * g_fSecPerFrame;
	
	for (int iParticle = 0; iParticle < m_iEffectCount; iParticle++)
	{
		// 보간
		if (m_bRepeatEffect == false && !ClassifyPointInInterpolate(&m_Particle[iParticle].m_vPos))
		{
			TVector4 vColor(0.0f, 0.0f, 0.0f, 0.0f);
			D3DXVec4Lerp(&m_Particle[iParticle].m_vColor,
				&m_Particle[iParticle].m_vColor,
				&vColor,
				time);
		}

		// 제거
		if (!ClassifyPoint(&m_Particle[iParticle].m_vPos))
		{
			// 반복 이펙트
			if (m_bRepeatEffect == true)
			{
				m_Particle[iParticle].m_vPos = m_Particle[iParticle].m_vFirstPos;
				continue;
			}

			m_Particle.erase(m_Particle.begin() + iParticle);
			m_iEffectCount--;
		}
	}
}

void ParticleSystem::EffectLifeTimeCheck()
{
	// 이펙트 시간이 -1이면 무한 반복
	if (m_fLifeTime == -1)
	{
		return;
	}

	for (int iParticle = 0; iParticle < m_iEffectCount; iParticle++)
	{
		float time = (m_fLifeTime / 2.0f) * g_fSecPerFrame;
		// ElapseTime은 실제 구현시 다르게 적용되어야한다. 
		// m_EffectTime을 만들어 secTime을 누적시킨 값으로 처리하자!
		// 라이프타임이 1보다 클 경우만 보간
		if (m_fLifeTime >= 1.0f)
		{
			// 보간
			if (g_fElapseTime >= m_fLifeTime - 1.5f)
			{
				TVector4 vColor(0.0f, 0.0f, 0.0f, 0.0f);
				D3DXVec4Lerp(&m_Particle[iParticle].m_vColor,
					&m_Particle[iParticle].m_vColor,
					&vColor,
					time);
			}
		}

		// 제거
		if (g_fElapseTime >= m_fLifeTime)
		{
			m_Particle.erase(m_Particle.begin() + iParticle);
			m_iEffectCount--;
		}
	}
}

//--------------------------------------------------------------------------------------
// 충돌처리 함수
// 평면으로부터 떨어진 정점의 거리를 구하는 함수
// 점 제외 처리 ( 주의 : 평면노말은 내부을 바라보고 있다. )
//--------------------------------------------------------------------------------------
bool ParticleSystem::ClassifyPoint(TVector3* pv)
{
	float m_pStreamlaneToPoint = 0.0f;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		m_pStreamlaneToPoint = (m_BoundingPlane[iPlane].fA * pv->x) +
			(m_BoundingPlane[iPlane].fB * pv->y) +
			(m_BoundingPlane[iPlane].fC * pv->z) +
			m_BoundingPlane[iPlane].fD;
		if (m_pStreamlaneToPoint < 0.0f)
			return false;
	}
	return true;
}

bool ParticleSystem::ClassifyPointInInterpolate(TVector3* pv)
{
	float m_pStreamlaneToPoint = 0.0f;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		m_pStreamlaneToPoint = (m_BoundingPlane[iPlane].fA * pv->x) +
			(m_BoundingPlane[iPlane].fB * pv->y) +
			(m_BoundingPlane[iPlane].fC * pv->z) +
			m_BoundingPlane[iPlane].fD;
		if (m_pStreamlaneToPoint <= EFFECT_EXTINCTION_DISTANCE)
			return false;
	}
	return true;
}

bool ParticleSystem::Init()
{
	HRESULT hr = S_OK;

	// 알파블랜딩
	D3D11_BLEND_DESC bd;
	bd.IndependentBlendEnable = FALSE;
	bd.AlphaToCoverageEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&bd, m_pAlphaBlend.GetAddressOf())))
	{
		return false;
	}

	// 알파 테스팅
	D3D11_BLEND_DESC bdTesting;
	bdTesting.IndependentBlendEnable = FALSE;
	bdTesting.AlphaToCoverageEnable = TRUE;
	bdTesting.RenderTarget[0].BlendEnable = TRUE;
	bdTesting.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bdTesting.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bdTesting.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bdTesting.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&bdTesting, m_pAlphaTesting.GetAddressOf())))
	{
		return false;
	}
	return true;
}

bool ParticleSystem::Frame(ID3D11DeviceContext* pContext)
{
	// 이펙트 시간 체크
	EffectLifeTimeCheck();

	// 이펙트 박스 범위 넘어가면 삭제
	CheckEffectRangeFromBox();

	if (m_iRow >= m_iCol)
	{
		m_Effect.SetRect(m_iRow, m_iCol, (float)m_iCol / 2);
	}
	else
	{
		m_Effect.SetRect(m_iRow, m_iCol, (float)m_iRow / 2);
	}

	ParticleUpdate(pContext);
	return true;
}

bool ParticleSystem::Render(ID3D11DeviceContext* pContext)
{
	// 바운딩박스 업데이트
	m_BoundingBoxShape.SetMatrix(nullptr, &m_pCamera->m_matView, &m_pCamera->m_matProj);
	m_BoundingBoxShape.Render(pContext);

	pContext->VSSetShader(m_Effect.m_pVS, NULL, 0);
	pContext->PSSetShader(m_Effect.m_pMainPS, NULL, 0);
	//m_pImmediateContext->GSSetShader(m_Effect->m_pGS, NULL, 0);
	pContext->IASetInputLayout(m_pLayout.Get());

	ID3D11Buffer* pBuffer[2] = { m_Effect.m_pVertexBuffer, m_pInstanceBuffer.Get() };

	UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(Instance) };
	UINT offset[2] = { 0, 0 };

	pContext->PSSetShaderResources(0, m_Effect.m_pTexture->m_pTextureSRV.size(), &m_Effect.m_pTexture->m_pTextureSRV.at(0));
	pContext->UpdateSubresource(m_Effect.m_pConstantBuffer, 0, NULL, &m_Effect.m_cbData, 0, 0);
	pContext->UpdateSubresource(m_pInstanceBuffer.Get(), 0, NULL, m_pInstData, 0, 0);
	// 두개의 버퍼가 넘어간다.
	pContext->IASetVertexBuffers(0, 2, pBuffer, stride, offset);
	pContext->IASetIndexBuffer(m_Effect.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_Effect.m_pConstantBuffer);
	pContext->PSSetConstantBuffers(0, 1, &m_Effect.m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	pContext->DrawIndexedInstanced(m_Effect.m_IndexList.size(), m_iEffectCount, 0, 0, 0);

	return true;
}

bool ParticleSystem::Release()
{
	SAFE_DELETE_ARRAY(m_pInstData);
	DeleteVector(m_Particle);
	m_Effect.Release();
	m_BoundingBoxShape.Release();
	return true;
}

ParticleSystem::ParticleSystem()
{
	m_pInstData			= NULL;
	m_iEffectCount		= 0;
	m_EffectType		= EFFECT_TYPE::Straight;
	m_CoordinateType	= COORDINATE_TYPE::RotationZ;
	m_iRow				= 1;
	m_iCol				= 1;
	m_fLifeTime			= 1.0f;
	m_iIndex			= 0;
	m_bAlphaBlending	= FALSE;
	m_bAlphaTesting		= FALSE;
	m_bBillboard		= TRUE;
	m_bRepeatEffect		= FALSE;
}


ParticleSystem::~ParticleSystem()
{
}
