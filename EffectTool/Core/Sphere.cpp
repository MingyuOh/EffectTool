#include "Sphere.h"

//--------------------------------------------------------------------------------------
// 정점 버퍼 생성
//--------------------------------------------------------------------------------------
bool Sphere::CreateVertexData()
{
	CreateSphere(20, 1.0f);

	if (m_VertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Sphere::CreateVertexBuffer()
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

//--------------------------------------------------------------------------------------
// 인덱스 버퍼 생성
//--------------------------------------------------------------------------------------
bool Sphere::CreateIndexData()
{
	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Sphere::CreateIndexBuffer()
{
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Sphere 정점 생성 함수
//--------------------------------------------------------------------------------------
void Sphere::MakeSphereVertex(vector<TVector3>& VertexArray, UINT sliceCount, float fRadius)
{
	VertexArray.clear();
	VertexArray.resize(sliceCount * sliceCount);

	for (int jCnt = 0; jCnt < sliceCount; jCnt++)
	{
		float theta = (PI * jCnt) / sliceCount;
		for (int iCnt = 0; iCnt < sliceCount; iCnt++)
		{
			int iIndex = jCnt * sliceCount + iCnt;
			float phi = (2 * PI * iCnt) / sliceCount;
			VertexArray[iIndex].x = fRadius * sin(theta) * cos(phi);
			VertexArray[iIndex].z = fRadius * sin(theta) * sin(phi);
			VertexArray[iIndex].y = fRadius * cos(theta);
		}
	}
}

//--------------------------------------------------------------------------------------
// Sphere 생성 함수
//--------------------------------------------------------------------------------------
void Sphere::CreateSphere(UINT Slice, float fRadius)
{
	vector<TVector3> VertexArray;
	m_VertexList.resize(Slice * Slice * 6);
	MakeSphereVertex(VertexArray, Slice, fRadius);

	int iNext = 0;
	for (int jCnt = 0; jCnt < Slice; jCnt++)
	{
		for (int iCnt = 0; iCnt < Slice; iCnt++)
		{
			if (iCnt == Slice - 1)
				iNext = 0;
			else
				iNext = iCnt + 1;

			PNCT_VERTEX v[6];
			v[0].p = VertexArray[jCnt * Slice + iCnt];
			v[1].p = VertexArray[jCnt * Slice + iNext];

			if (jCnt != Slice - 1)
				v[2].p = VertexArray[(jCnt + 1) * Slice + iCnt];
			else
				v[2].p = TVector3(0, -1, 0);

			v[3].p = v[2].p;
			v[4].p = v[1].p;

			if (jCnt != Slice - 1)
				v[5].p = VertexArray[(jCnt + 1) * Slice + iNext];
			else
				v[5].p = TVector3(0, -1, 0);

			for (int iVer = 0; iVer < 6; iVer++)
			{
				// 구 단위벡터
				TVector3 vNormal = v[iVer].p;
				vNormal.Normalize();
				v[iVer].n = vNormal;
				// 구 컬러 좌표
				v[iVer].c = TVector4(v[iVer].n.x, v[iVer].n.y, v[iVer].n.z, 1);
				// 구 텍스처 좌표
				v[iVer].t = TVector2(atan2(v[iVer].n.z, v[iVer].n.x) / (PI * 2.0f) + 0.5f, asinf(-v[iVer].n.y) / PI + 0.5f);
			}


			if (fabs(v[1].t.x - v[0].t.x) > 0.9f)
			{
				if (fabs(v[1].t.x) <= 0.01f)
				{
					v[1].t.x = 1.0f;
				}
				if (fabs(v[0].t.x) <= 0.01f)
				{
					v[0].t.x = 1.0f;
				}
			}

			if (fabs(v[2].t.x - v[0].t.x) > 0.9f)
			{
				if (fabs(v[2].t.x) <= 0.01f)
				{
					v[2].t.x = 1.0f;
				}
				if (fabs(v[0].t.x) <= 0.01f)
				{
					v[0].t.x = 1.0f;
				}
			}

			if (fabs(v[4].t.x - v[3].t.x) > 0.9f)
			{
				if (fabs(v[4].t.x) <= 0.01f)
				{
					v[4].t.x = 1.0f;
				}
				if (fabs(v[3].t.x) <= 0.01f)
				{
					v[3].t.x = 1.0f;
				}
			}

			if (fabs(v[5].t.x - v[3].t.x) > 0.9f)
			{
				if (fabs(v[5].t.x) <= 0.01f)
				{
					v[5].t.x = 1.0f;
				}
				if (fabs(v[3].t.x) <= 0.01f)
				{
					v[3].t.x = 1.0f;
				}
			}

			m_VertexList.push_back(v[0]);
			m_VertexList.push_back(v[1]);
			m_VertexList.push_back(v[2]);
			m_VertexList.push_back(v[3]);
			m_VertexList.push_back(v[4]);
			m_VertexList.push_back(v[5]);
		}
	}
	VertexArray.clear();
}

//--------------------------------------------------------------------------------------
// 렌더링 재정의 함수
//--------------------------------------------------------------------------------------
bool Sphere::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	//m_pImmediateContext->GSSetShader(m_pGS, NULL, 0);

	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;

	// 상수버퍼 갱신
	m_pTexture->Apply(pContext);
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->Draw(m_VertexList.size(), 0);
	return true;
}

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}
