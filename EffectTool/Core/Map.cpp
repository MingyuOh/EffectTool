#include "Map.h"

//--------------------------------------------------------------------------------------
// 정점 버퍼 생성
//--------------------------------------------------------------------------------------
bool Map::CreateVertexData()
{
	CreateMap(m_iNumRows, m_iNumCols, 1.0f);
	if (m_VertexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Map::CreateVertexBuffer()
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
	return g_pd3dDevice->CreateBuffer(&cbc, &InitData, &m_pVertexBuffer);
}

//--------------------------------------------------------------------------------------
// 인덱스 버퍼 생성
//--------------------------------------------------------------------------------------
bool Map::CreateIndexData()
{
	if (m_IndexList.empty())
	{
		return false;
	}
	return true;
}

HRESULT	Map::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	m_iNumIndex = m_IndexList.size();

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
	return hr;
}

//--------------------------------------------------------------------------------------
// 맵 생성
//--------------------------------------------------------------------------------------
bool Map::CreateMap(UINT iWidth, UINT iHeight, float fDistance)
{
	// 대용량의 사이즈를 저정한다.( pow(2.0f, 10.0f) + 1 이상)
	if (iWidth > 1025 || iHeight > 1025)
	{
		iWidth = 1025; // 정점개수 105만개,
		iHeight = 1025; // 페이스 개수 209만개 
	}

	int iTotalVertex = iWidth * iHeight;
	m_MapInfo.iWidth = m_iNumCols;
	m_MapInfo.iHeight = m_iNumRows;
	m_MapInfo.fSellDistance = fDistance;
	m_MapInfo.fScaleHeight = 0.1f;

	m_iNumCells = (m_iNumRows - 1) * (m_iNumCols - 1);
	m_iNumFaces = m_iNumCells * 2;

	m_VertexList.resize(iTotalVertex);
	
	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iCol = 0; iCol < iWidth; iCol++)
		{
			int iIndex = iRow * iWidth + iCol;

			m_VertexList[iIndex].p.x = (iCol - (iWidth / 2.0f)) * fDistance;//((float)(iWidth - 1) / 2))* fDistance;
			m_VertexList[iIndex].p.y = m_fHeightArray.empty() ? 0.0f : m_fHeightArray[iIndex] * m_MapInfo.fScaleHeight;
			m_VertexList[iIndex].p.z = (-(iRow - (iHeight / 2.0f))) * fDistance; // (-((float)iRow - ((float)(iHeight - 1) / 2))) * fDistance;

			m_VertexList[iIndex].n = TVector3(0.0f, 1.0f, 0.0f);
			m_VertexList[iIndex].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexList[iIndex].t.x = iCol * (1.0f / (iWidth - 1));
			m_VertexList[iIndex].t.y = iRow * (1.0f / (iHeight - 1));
		}
	}

	int iTotalIndex = (iWidth - 1) * (iHeight - 1) * 2 * 3;
	m_IndexList.resize(iTotalIndex);
	int iCurrent = 0;

	for (int iRow = 0; iRow < iHeight - 1; iRow++)
	{
		for (int iCol = 0; iCol < iWidth - 1; iCol++)
		{
			m_IndexList[iCurrent++] = iRow * iWidth + iCol;
			m_IndexList[iCurrent++] = iRow * iWidth + iCol + 1;
			m_IndexList[iCurrent++] = (iRow + 1) * iWidth + iCol;

			m_IndexList[iCurrent++] = (iRow + 1) * iWidth + iCol;
			m_IndexList[iCurrent++] = iRow * iWidth + iCol + 1;
			m_IndexList[iCurrent++] = (iRow + 1) * iWidth + iCol + 1;
		}
	}

	UpdateNormal();
	return true;
}

//--------------------------------------------------------------------------------------
// 정점 노말 생성
//--------------------------------------------------------------------------------------
void Map::UpdateNormal()
{
	InitFaceNormals();
	GenerateNormalLookUpTable();
	CalcPerVertexNormalsLookup();
}

//--------------------------------------------------------------------------------------
// 페이스 노말 리스트 구축
//--------------------------------------------------------------------------------------
void Map::InitFaceNormals()
{
	m_FaceNormals.resize(m_iNumFaces);
	for (int iFace = 0; iFace < m_iNumFaces; iFace++)
	{
		DWORD i0 = m_IndexList[iFace * 3 + 0];
		DWORD i1 = m_IndexList[iFace * 3 + 1];
		DWORD i2 = m_IndexList[iFace * 3 + 2];

		TVector3 v0 = m_VertexList[i0].p;
		TVector3 v1 = m_VertexList[i1].p;
		TVector3 v2 = m_VertexList[i2].p;

		TVector3 e0 = v1 - v0;
		TVector3 e1 = v2 - v0;

		m_FaceNormals[iFace] = e0.Cross(e1);
		m_FaceNormals[iFace].Normalize();
	}
}

//--------------------------------------------------------------------------------------
// 정점 룩업테이블 구축
//--------------------------------------------------------------------------------------
void Map::GenerateNormalLookUpTable()
{
	// 정점당 최대 6개 인덱스를 저장
	m_NormalLookupTable.resize(m_VertexList.size() * 6);

	// 룩업테이블 초기화
	for (int iCnt = 0; iCnt < m_NormalLookupTable.size(); iCnt++)
	{
		m_NormalLookupTable[iCnt] = -1;
	}

	for (int iFace = 0; iFace < m_iNumFaces; iFace++)
	{
		// 페이스당 정점 3개
		for (int iVertex = 0; iVertex < 3; iVertex++)
		{
			// 정점당 최대 페이스 6개
			for (int iLt = 0; iLt < 6; iLt++)
			{
				int iIndex = m_IndexList[iFace * 3 + iVertex];
				if (m_NormalLookupTable[iIndex * 6 + iLt] == -1)
				{
					m_NormalLookupTable[iIndex * 6 + iLt] = iFace;
					break;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// 페이스 개수만큼 계산
//--------------------------------------------------------------------------------------
void Map::CalcPerVertexNormalsLookup()
{
	for (int iVertex = 0; iVertex < m_VertexList.size(); iVertex++)
	{
		TVector3 avgNormal(0.0f, 0.0f, 0.0f);
		for (int iFace = 0; iFace < 6; iFace++)
		{
			int iFaceIndex = m_NormalLookupTable[iVertex * 6 + iFace];
			if (iFaceIndex != -1)
			{
				avgNormal += m_FaceNormals[iFaceIndex];
			}
			else
			{
				break;
			}
		}

		avgNormal.Normalize();
		m_VertexList[iVertex].n = avgNormal;
	}

	// 컬러값 저장
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].c.x = m_VertexList[iVertexIndex].n.x;
			m_VertexList[iVertexIndex].c.y = m_VertexList[iVertexIndex].n.y;
			m_VertexList[iVertexIndex].c.z = m_VertexList[iVertexIndex].n.z;
			m_VertexList[iVertexIndex].c.w = 1.0f;
		}
	}
}

bool Map::Init(UINT iWidth, UINT iHeight)
{
	m_iNumRows = iWidth;
	m_iNumCols = iHeight;

	Object::Init();

	return true;
}

float Map::GetHeight(TVector3 vPos)
{
	return 0.0f;
}

float Map::GetHeight(int iRow, int iCol)
{
	return  m_VertexList[iRow * m_iNumRows + iCol].p.y;
}

Map::Map()
{
	ZeroMemory(&m_MapInfo, sizeof(MapInfo));
	m_iNumRows  = 0;
	m_iNumCols  = 0;
	m_iNumCells = 0;
	m_iNumFaces = 0;
}


Map::~Map()
{
}
