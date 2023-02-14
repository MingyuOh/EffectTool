#include "HeightMap.h"

//--------------------------------------------------------------------------------------
// 높이 맵 불러오기
//--------------------------------------------------------------------------------------
bool HeightMap::LoadHeightMap(ID3D11DeviceContext* pContext, T_STR strLoad)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Resource> pResource = NULL;
	size_t maxsize = 0;

	if (FAILED(hr = DirectX::CreateWICTextureFromFileEx(g_pd3dDevice,
		strLoad.c_str(),
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		pResource.GetAddressOf(), nullptr)))
	{
		return hr;
	}

	ID3D11Texture2D* pTexture = NULL;
	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture);

	D3D11_TEXTURE2D_DESC pInfo;
	pTexture->GetDesc(&pInfo);
	int iEndRow = pInfo.Height, iEndCol = pInfo.Width;
	if (pInfo.Width % 2 == 0)
	{
		pInfo.Width++;
		iEndCol -= 1;
	}
	
	if (pInfo.Height % 2 == 0)
	{
		pInfo.Height++;
		iEndRow -= 1;
	}
	
	m_fHeightArray.resize(pInfo.Width * pInfo.Height);

	D3D11_MAPPED_SUBRESOURCE ms;
	UINT iIndex = D3D11CalcSubresource(0, 0, 1);
	if (SUCCEEDED(hr = pContext->Map(pTexture, iIndex, D3D11_MAP_READ, 0, &ms)))
	{
		UCHAR* pTexels = (UCHAR*)ms.pData;
		for (int row = 0; row < iEndRow; row++)
		{
			UINT rowStart = row * ms.RowPitch;
			for (int col = 0; col < iEndCol; col++)
			{
				UINT colStart = col * 4;
				UINT uRed = pTexels[rowStart + colStart + 0];
				UINT uGreen = pTexels[rowStart + colStart + 1];
				UINT uBlue = pTexels[rowStart + colStart + 2];
				m_fHeightArray[row * pInfo.Width + col] = uRed;
			}
		}
		pContext->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));
	}

	SetMaxHeight();
	m_iNumRows = pInfo.Height;
	m_iNumCols = pInfo.Width;

	SAFE_RELEASE(pTexture);

	return true;
}

void HeightMap::SetMaxHeight()
{
	for (auto& height : m_fHeightArray)
	{
		m_fMaxHeight = (m_fMaxHeight < height) ? height : m_fMaxHeight;
	}
}

float HeightMap::GetHeight(int iRow, int iCol)
{
	return m_VertexList[iRow * m_iNumRows + iCol].p.y;
}

float HeightMap::GetHeight(TVector3 vPos)
{
	float iWidthSell = (float)(m_MapInfo.iWidth - 1);
	float iHeightSell = (float)(m_MapInfo.iHeight - 1);
	float iSellSize = m_MapInfo.fSellDistance;
	float fSellX = (iWidthSell * iSellSize / 2.0f) + vPos.x;
	float fSellY = (iHeightSell * iSellSize / 2.0f) - vPos.z;
	fSellX /= iSellSize;
	fSellY /= iSellSize;

	int iSellX = ::floorf(fSellX);
	int iSellY = ::floorf(fSellY);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (iSellX < 0) 
		iSellX = 0;
	if (iSellY < 0) 
		iSellY = 0;
	if((float)(iWidthSell - 2) < iSellX)
		iSellX = iWidthSell - 2;
	if ((float)(iHeightSell - 2) < iSellY)
		iSellY = iHeightSell - 2;

	float A = GetHeight(iSellY, iSellX);
	float B = GetHeight(iSellY, iSellX + 1);
	float C = GetHeight(iSellY + 1, iSellX);
	float D = GetHeight(iSellY + 1, iSellX + 1);

	float fU = fSellX - iSellX;
	float fV = fSellY - iSellY;

	float fHeight = 0.0f;
	if (fV < (1.0f - fU)) // 윗면
	{
		fHeight = A + Lerp(0.0f, B - A, fU) + Lerp(0.0f, C - A, fV);
	}
	else
	{
		fHeight = D + Lerp(0.0f, C - D, 1.0f - fU) + Lerp(0.0f, B - D, 1.0f - fV);
	}

	return fHeight;
}

float HeightMap::Lerp(float a, float b, float t)
{
	float fStart = a;
	float fEnd = b;
	float fDelta = t;
	return fStart + ((fEnd - fStart) * fDelta);
}

HeightMap::HeightMap()
{
	m_fMaxHeight = 0.0f;
}

HeightMap::~HeightMap()
{
}
