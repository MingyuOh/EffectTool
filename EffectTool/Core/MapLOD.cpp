#include "MapLOD.h"

//--------------------------------------------------------------------------------------
// Init
//--------------------------------------------------------------------------------------
void MapLOD::InitLOD(Node* pRootNode, int iMaxDepth)
{
	int iLength = iMaxDepth + 1;
	m_LevelList.resize(iLength);
	for (int iLevel = 1; iLevel < iLength; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pRootNode);
}

//--------------------------------------------------------------------------------------
// Add draw patch node
//--------------------------------------------------------------------------------------
void MapLOD::AddDrawPatchNode(Node* pNode)
{
	if (pNode->GetIsLeaf() == true)
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}

	for (int iNode = 0; iNode < pNode->GetChildList().size(); iNode++)
	{
		AddDrawPatchNode(pNode->GetChild(iNode));
	}
}

//--------------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------------
void MapLOD::Update(Camera* pCamera)
{
	m_pCamera = pCamera;
}

//--------------------------------------------------------------------------------------
// Set funtion
//--------------------------------------------------------------------------------------
void MapLOD::SetLOD(DWORD dwWIdth, int iNumDepth)
{
	m_iNumCell = static_cast<int>((dwWIdth - 1) / pow(2.0f, static_cast<float>(iNumDepth)));
	m_iPatchLodCount = static_cast<int>(log(static_cast<float>(m_iNumCell) / log(2.0f)));
}


void MapLOD::SetExpansionRatio(int iRatio)
{
	m_fExpansionRatio = iRatio;
}

//--------------------------------------------------------------------------------------
// Get function
//--------------------------------------------------------------------------------------
DWORD MapLOD::GetLodSubIndex(Node* pNode)
{
	float fRatio = GetExpansionRatio(pNode->GetBoundingBox().vCenter);
	DWORD dwCurrentRatio = (DWORD)(fRatio * m_iPatchLodCount);
	DWORD dwMaxRatio = (DWORD)(fRatio * m_iPatchLodCount + 0.5f);
	DWORD dwMinRatio = (DWORD)(fRatio * m_iPatchLodCount - 0.5f);
	if (m_bThresHoldValue == true)
	{
		if (pNode->GetLodLevel() < dwCurrentRatio)
		{
			if (pNode->GetLodLevel() < dwMinRatio)
			{
				pNode->SetLodLevel(dwCurrentRatio);
			}
		}
		else if (pNode->GetLodLevel() > dwCurrentRatio)
		{
			if (pNode->GetLodLevel() > dwMaxRatio)
			{
				pNode->SetLodLevel(dwCurrentRatio);
			}
		}
	}
	else
	{
		pNode->SetLodLevel(dwCurrentRatio);
	}

	///예외처리
	if ((DWORD)m_iPatchLodCount < pNode->GetLodLevel())
	{
		pNode->SetLodLevel(-1);
	}

	return pNode->GetLodLevel();
}

DWORD MapLOD::GetLodType(Node* pNode)
{
	if (pNode->GetNeighborList().size() <= 0)
	{
		return 0;
	}

	DWORD dwType = 0;
	// 상단
	if (pNode->GetNeighbor(0) && pNode->GetNeighbor(0)->GetLodLevel() < pNode->GetLodLevel())
	{
		dwType += 1;
	}

	// 하단
	if (pNode->GetNeighbor(1) && pNode->GetNeighbor(1)->GetLodLevel() < pNode->GetLodLevel())
	{
		dwType += 4;
	}

	// 좌측
	if (pNode->GetNeighbor(2) && pNode->GetNeighbor(2)->GetLodLevel() < pNode->GetLodLevel())
	{
		dwType += 8;
	}

	// 우측
	if (pNode->GetNeighbor(3) && pNode->GetNeighbor(3)->GetLodLevel() < pNode->GetLodLevel())
	{
		dwType += 2;
	}

	pNode->SetLodType(dwType);
	return dwType;
}

float MapLOD::GetExpansionRatio(TVector3 vCenter)
{
	float fDistance = D3DXVec3Length(&(m_pCamera->m_vPos - vCenter));
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}

	float fRatio = fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}

float MapLOD::GetExpansionRatio(TVector3 vMax, TVector3 vMin)
{
	float fRatio = 0.0f;
	TVector3 vCenterViewPos, vSideViewPos;

	D3DXVec3TransformCoord(&vCenterViewPos, &vMax, &m_pCamera->m_matView);
	D3DXVec3TransformCoord(&vSideViewPos, &vMin, &m_pCamera->m_matView);

	TVector3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, &m_pCamera->m_matProj);
	D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, &m_pCamera->m_matProj);

	TVector2 vA = TVector2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	TVector2 vB = TVector2(vSideProjPos.x * 0.5f + 0.5f, vSideProjPos.y * 0.5f + 0.5f);

	fRatio = D3DXVec3Length(&(vCenterProjPos - vSideProjPos));

	return fRatio;
}

void MapLOD::GetDrawPatchNode()
{
	m_DrawPatchNodeList.clear();
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}
}

MapLOD::MapLOD()
{
	m_pd3dDevice = nullptr;
	m_pCamera = nullptr;
	m_bThresHoldValue = false;
	m_fExpansionRatio = 0.0f;
	m_iNumCell = 0;
	m_iPatchLodCount = 0;
}

MapLOD::~MapLOD()
{
}