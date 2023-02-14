#include "GeomObj.h"

//--------------------------------------------------------------------------------------
// 버텍스 버퍼 생성
//--------------------------------------------------------------------------------------
HRESULT	GeomObj::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	m_VertexList.resize(m_Object.m_iNumFace * 3);

	TMatrix matInverse;
	matInverse = m_matWorld.Invert();

	for (int iFace = 0; iFace < m_Object.m_iNumFace; iFace++)
	{
		// 정점
		if (m_Object.m_iNumVertex > 0)
		{
			m_VertexList[iFace * 3 + 0].p = m_Object.m_VertexArray[m_Object.m_FaceArray[iFace].iIndex[0]];
			m_VertexList[iFace * 3 + 1].p = m_Object.m_VertexArray[m_Object.m_FaceArray[iFace].iIndex[1]];
			m_VertexList[iFace * 3 + 2].p = m_Object.m_VertexArray[m_Object.m_FaceArray[iFace].iIndex[2]];

			m_VertexList[iFace * 3 + 0].p = TVector3::Transform(m_VertexList[iFace * 3 + 0].p, matInverse);
			m_VertexList[iFace * 3 + 1].p = TVector3::Transform(m_VertexList[iFace * 3 + 1].p, matInverse);
			m_VertexList[iFace * 3 + 2].p = TVector3::Transform(m_VertexList[iFace * 3 + 2].p, matInverse);
		}

		// 색상
		if (m_Object.m_iNumColorVertex > 0)
		{
			m_VertexList[iFace * 3 + 0].c = TVector4(m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[0]].x,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[0]].y,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[0]].z,
													1.0f);
			m_VertexList[iFace * 3 + 1].c = TVector4(m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[1]].x,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[1]].y,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[1]].z,
													1.0f);
			m_VertexList[iFace * 3 + 2].c = TVector4(m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[2]].x,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[2]].y,
													m_Object.m_ColorVertexArray[m_Object.m_ColorFaceArray[iFace].iIndex[2]].z,
													1.0f);
		}
		else
		{
			// 일시적
			m_VertexList[iFace * 3 + 0].c = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
			m_VertexList[iFace * 3 + 1].c = TVector4(0.0f, 1.0f, 0.0f, 1.0f);
			m_VertexList[iFace * 3 + 2].c = TVector4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		// 텍스쳐
		if (m_Object.m_iNumTexVertex > 0)
		{
			m_VertexList[iFace * 3 + 0].t = TVector2(m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[0]].x,
													 m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[0]].y);
			m_VertexList[iFace * 3 + 0].t = TVector2(m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[1]].x,
													 m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[1]].y);
			m_VertexList[iFace * 3 + 0].t = TVector2(m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[2]].x,
													 m_Object.m_TexVertexArray[m_Object.m_TexFaceArray[iFace].iIndex[2]].y);
		}

		// 노말정점
		m_VertexList[iFace * 3 + 0].n = m_Object.m_FaceNormalArray[m_Object.m_FaceArray[iFace].iIndex[0]].vVertex[0];
		m_VertexList[iFace * 3 + 1].n = m_Object.m_FaceNormalArray[m_Object.m_FaceArray[iFace].iIndex[1]].vVertex[1];
		m_VertexList[iFace * 3 + 2].n = m_Object.m_FaceNormalArray[m_Object.m_FaceArray[iFace].iIndex[2]].vVertex[2];
	}

	// 서브매터리얼이 존재 할 경우
	if (m_VBArray.size() > 0)
	{
		m_subVertexList.resize(m_VBArray.size());
		for (int iFace = 0; iFace < m_Object.m_iNumFace; iFace++)
		{
			int iSubMtrl = m_Object.m_FaceArray[iFace].iSubMtrl;
			m_subVertexList[iSubMtrl].push_back(m_VertexList[iFace * 3 + 0]);
			m_subVertexList[iSubMtrl].push_back(m_VertexList[iFace * 3 + 1]);
			m_subVertexList[iSubMtrl].push_back(m_VertexList[iFace * 3 + 2]);
		}

		for (int iVB = 0; iVB < m_VBArray.size(); iVB++)
		{
			SetUniqueBuffers(m_subVertexList[iVB], iVB);
			if (m_subUniqueVertexList[iVB].size() <= 0)
				continue;
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(PNCT_VERTEX) * m_subUniqueVertexList[iVB].size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &m_subUniqueVertexList[iVB].at(0);
			g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_VBArray[iVB]);
		}
	}
	else // 서브매터리얼이 존재하지 않을 경우
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_VertexList.at(0);
		g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// 인덱스 버퍼 생성
//--------------------------------------------------------------------------------------
HRESULT	GeomObj::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	// 서브매터리얼이 존재 할 경우
	if (m_IBArray.size() > 0)
	{
		for (int iIB = 0; iIB < m_IBArray.size(); iIB++)
		{
			if (m_subIndexList[iIB].size() <= 0)
				continue;
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(DWORD) * m_subIndexList[iIB].size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &m_subIndexList[iIB].at(0);
			g_pd3dDevice->CreateBuffer(&bd, &InitData, &m_IBArray[iIB]);
		}
	}
	else
	{
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = m_IndexList.size() * sizeof(DWORD);
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibInitData;
		ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		ibInitData.pSysMem = &m_IndexList.at(0);

		g_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer);
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// 상수 버퍼 생성
//--------------------------------------------------------------------------------------
HRESULT	GeomObj::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(VSCBMatrix);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = g_pd3dDevice->CreateBuffer(&cbDesc, NULL, &this->m_pConstantBuffer);
	return hr;
}

//--------------------------------------------------------------------------------------
// 서브매터리얼 버퍼 생성
//--------------------------------------------------------------------------------------
void GeomObj::SetUniqueBuffers(vector<PNCT_VERTEX>& vertexArray, int iSub)
{
	m_subUniqueVertexList.resize(m_VBArray.size());
	m_subIndexList.resize(m_VBArray.size());
	for (int iTri = 0; iTri < vertexArray.size(); iTri++)
	{
		int iIndex = isEqualVertexList(m_subUniqueVertexList[iSub], vertexArray[iTri]);
		if (iIndex < 0)
		{
			m_subUniqueVertexList[iSub].push_back(vertexArray[iTri]);
			iIndex = m_subUniqueVertexList[iSub].size() - 1;
		}
		m_subIndexList[iSub].push_back(iIndex);
	}
}

//--------------------------------------------------------------------------------------
// 중복 정점 체크
//--------------------------------------------------------------------------------------
int GeomObj::isEqualVertexList(vector<PNCT_VERTEX>& UniqueVertexArray, PNCT_VERTEX& vertex)
{
	int iIndex = -1;
	for (int iVertex = 0; iVertex < UniqueVertexArray.size(); iVertex++)
	{
		if (UniqueVertexArray[iVertex] == vertex)
		{
			return iVertex;
		}
	}
	return iIndex;
}

bool GeomObj::Render(ID3D11DeviceContext* pContext)
{
	if (m_VBArray.size() > 0)
	{
		UINT stride = sizeof(PNCT_VERTEX);
		UINT offset = 0;
		for (int iSub = 0; iSub < m_VBArray.size(); iSub++)
		{
			if (m_subIndexList[iSub].size() <= 0)
				continue;

			pContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV[iSub]);
			pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
			pContext->IASetVertexBuffers(0, 1, &m_VBArray[iSub], &stride, &offset);
			pContext->IASetIndexBuffer(m_IBArray[iSub], DXGI_FORMAT_R32_UINT, 0);
			pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pContext->DrawIndexed(m_subIndexList[iSub].size(), 0, 0);
		}
	}
	else
	{
		UINT stride = sizeof(PNCT_VERTEX);
		UINT offset = 0;

		// 상수버퍼 갱신
		//ApplyTexture(pContext);
		pContext->UpdateSubresource(this->m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
		pContext->IASetVertexBuffers(0, 1, &this->m_pVertexBuffer, &stride, &offset);
		pContext->VSSetConstantBuffers(0, 1, &this->m_pConstantBuffer);
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pContext->Draw(m_VertexList.size(), 0);
	}
	return true;
}


GeomObj::GeomObj() 
{
	m_Object.m_matTrans = TMatrix::Identity;
	m_Object.m_matScale = TMatrix::Identity;
	m_Object.m_matRotation = TMatrix::Identity;
	m_Object.m_matCaculation = TMatrix::Identity;
	m_Object.m_iMaterialRef = -1;
	m_pParentObj = NULL;
	m_iParentID = -1;
}

GeomObj::~GeomObj() {}