#include "QuadTreeIndex.h"

//--------------------------------------------------------------------------------------
// Init
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::Build(DWORD dwWidth, DWORD dwHeight)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	SetRowCount(m_pMap->m_iNumRows);
	SetColumnCount(m_pMap->m_iNumCols);

	m_iNumFace = m_pMap->m_iNumFaces;
	
	m_indexList.resize(m_iNumFace * 3 * 2);
	SetRootNode(CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1));

	InitLOD(GetRootNode(), GetMaxDepthLimit());

	if (BuildTree(GetRootNode()))
	{
		for (int iNode = 0; iNode < GetRootNode()->GetChildList().size(); iNode++)
		{
			SetNeighborNode(GetRootNode()->GetChildList().at(iNode));
		}
		SetLOD(m_dwWidth, GetMaxDepth());
		ComputeStaticLodIndex(m_iNumCell);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Create Node
//--------------------------------------------------------------------------------------
Node* QuadTreeIndex::CreateNode(Node* pParent, DWORD dwTopLeft, DWORD dwTopRight, DWORD dwBottomLeft, DWORD dwBottomRight)
{
	Node* pNode = 0;
	SAFE_NEW(pNode, Node);
	assert(pNode);

	pNode->SetIsLeaf(FALSE);

	pNode->GetChildList().reserve(4);
	pNode->GetCornerList().reserve(4);
	pNode->GetCornerIndexList().reserve(4);

	pNode->SetCornerIndex(dwTopLeft);
	pNode->SetCornerIndex(dwTopRight);
	pNode->SetCornerIndex(dwBottomLeft);
	pNode->SetCornerIndex(dwBottomRight);

	ComputeBoungigBox(pNode);

	////////////
	// 1 // 2 //
	////////////
	// 3 // 4 //
	////////////
	// 일 경우 
	// 1번 노드 = (0, 0), 2번 노드 = (1, 0)
	// 3번 노드 = (0, 1), 4번 노드 = (1, 1) 
	POINT position;
	ldiv_t divValue = ldiv(dwTopLeft, m_dwWidth);
	position.x = divValue.rem / (dwTopRight - dwTopLeft);	// 나머지 -> X
	position.y = divValue.quot / (dwTopRight - dwTopLeft);	// 몫     -> Y
	pNode->SetPositionIndex(position);

	if (pParent)
	{
		pNode->SetDepth(pParent->GetDepth() + 1);
		if ((DWORD)GetMaxDepth() < pNode->GetDepth())
		{
			SetMaxDepth(pNode->GetDepth());
		}
	}

	return pNode;
}

//--------------------------------------------------------------------------------------
// Divide QuadTree
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::SubDivide(Node* pNode)
{
	assert(pNode);

	DWORD dwOffsetX = pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT) - pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT);
	DWORD dwOffsetY = (pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT) - pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT)) / m_dwWidth;

	DWORD dwWidthSplit = CheckSize(dwOffsetX);
	DWORD dwHeightSplit = CheckSize(dwOffsetY);

	if ((DWORD)GetMaxDepthLimit() <= pNode->GetDepth())
	{
		pNode->SetIsLeaf(TRUE);
		m_DrawNodeList.push_back(pNode);
		CreateVertexList(pNode);
		return false;
	}

	if (dwWidthSplit < GetMinDivideSize() || dwHeightSplit < GetMinDivideSize())
	{
		pNode->SetIsLeaf(TRUE);
		m_DrawNodeList.push_back(pNode);
		CreateVertexList(pNode);
		return false;
	}

	DWORD dwCenter = (pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT) + dwWidthSplit) + (dwHeightSplit * m_dwWidth);

	DWORD dwMiddleLeft = (pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT) + (dwHeightSplit * m_dwWidth));
	DWORD dwMiddleRight = (pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT) + (dwHeightSplit * m_dwWidth));
	DWORD dwMiddleBottom = (pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT) + dwWidthSplit);
	DWORD dwMiddleTop = (pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT) + (dwWidthSplit));

	///////////////
	// 1  ///  2 //
	///////////////
	// 3  ///  4 //
	///////////////
	pNode->SetChild(CreateNode(pNode,
		pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT),
							  dwMiddleTop,
							  dwMiddleLeft,
							  dwCenter));

	pNode->SetChild(CreateNode(pNode,
					dwMiddleTop,
					pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT),
					dwCenter,
					dwMiddleRight));

	pNode->SetChild(CreateNode(pNode,
					dwMiddleLeft,
					dwCenter,
					pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT),
					dwMiddleBottom));

	pNode->SetChild(CreateNode(pNode,
					dwCenter,
					dwMiddleRight,
					dwMiddleBottom,
					pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_RIGHT)));

	return true;
}

//--------------------------------------------------------------------------------------
// Set LOD index buffer
//--------------------------------------------------------------------------------------
int	QuadTreeIndex::SetLodIndexBuffer(Node* pNode, DWORD& dwCurrentIndex, DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType)
{
	int iNumFaces = 0;

	if (dwType == 0)
	{
		m_indexList[dwCurrentIndex + 0] = dwA;
		m_indexList[dwCurrentIndex + 1] = dwB;
		m_indexList[dwCurrentIndex + 2] = dwC;
		
		iNumFaces += 1;
		dwCurrentIndex += 3;
		return iNumFaces;
	}
	// 좌우
	if (dwType == 8 || dwType == 2)
	{
  		DWORD dwCenter = (dwA + dwB) / 2;
		m_indexList[dwCurrentIndex + 0] = dwC;
		m_indexList[dwCurrentIndex + 1] = dwA;
		m_indexList[dwCurrentIndex + 2] = dwCenter;
		m_indexList[dwCurrentIndex + 3] = dwC;
		m_indexList[dwCurrentIndex + 4] = dwCenter;
		m_indexList[dwCurrentIndex + 5] = dwB;
		
		iNumFaces += 2;
		dwCurrentIndex += 6;
		return iNumFaces;
	}
	// 상하
	if (dwType == 1 || dwType == 4)
	{
		DWORD dwCenter = (dwB + dwC) / 2;
		m_indexList[dwCurrentIndex + 0] = dwA;
		m_indexList[dwCurrentIndex + 1] = dwB;
		m_indexList[dwCurrentIndex + 2] = dwCenter;
		m_indexList[dwCurrentIndex + 3] = dwA;
		m_indexList[dwCurrentIndex + 4] = dwCenter;
		m_indexList[dwCurrentIndex + 5] = dwC;
		
		iNumFaces += 2;
		dwCurrentIndex += 6;
		return iNumFaces;
	}
	// 좌상, 우하
	if (dwType == 9 || dwType == 6)
	{
		DWORD dwTopCenter = (dwB + dwC) / 2;
		DWORD dwLeftCenter = (dwA + dwB) / 2;
		m_indexList[dwCurrentIndex + 0] = dwLeftCenter;
		m_indexList[dwCurrentIndex + 1] = dwB;
		m_indexList[dwCurrentIndex + 2] = dwTopCenter;

		m_indexList[dwCurrentIndex + 3] = dwLeftCenter;
		m_indexList[dwCurrentIndex + 4] = dwTopCenter;
		m_indexList[dwCurrentIndex + 5] = dwA;

		m_indexList[dwCurrentIndex + 6] = dwA;
		m_indexList[dwCurrentIndex + 7] = dwTopCenter;
		m_indexList[dwCurrentIndex + 8] = dwC;

		iNumFaces += 3;
		dwCurrentIndex += 9;
		return iNumFaces;
	}

	return iNumFaces;
}
//--------------------------------------------------------------------------------------
// A function to get the height value of a vertex through node information
//--------------------------------------------------------------------------------------
TVector2 QuadTreeIndex::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	assert(m_pMap);

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	TVector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y;
			}
			if (m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y;
			}
		}
	}
	return vHeight;
}

//--------------------------------------------------------------------------------------
// Update boundingbox
//--------------------------------------------------------------------------------------
void QuadTreeIndex::UpdateBoundingBox(Node* pNode)
{
	ComputeBoungigBox(pNode);
	for (int iNode = 0; iNode < pNode->GetChildList().size(); iNode++)
	{
		UpdateBoundingBox(pNode->GetChild(iNode));
	}
}

//--------------------------------------------------------------------------------------
// Compute boundingbox
//--------------------------------------------------------------------------------------
void QuadTreeIndex::ComputeBoungigBox(Node* pNode)
{
	TVector2 vHeight = GetHeightFromNode(pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT),
										 pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT),
										 pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT),
										 pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_RIGHT));
	pNode->m_BoundingBox.vMax = m_pMap->m_VertexList[pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_RIGHT)].p;
	pNode->m_BoundingBox.vMin = m_pMap->m_VertexList[pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT)].p;
	pNode->m_BoundingBox.vMax.y = vHeight.x;
	pNode->m_BoundingBox.vMin.y = vHeight.y;

	pNode->m_BoundingBox.vCenter = (pNode->GetBoundingBox().vMax + pNode->GetBoundingBox().vMin);
	pNode->m_BoundingBox.vCenter.x /= 2.0f;
	pNode->m_BoundingBox.vCenter.y /= 2.0f;
	pNode->m_BoundingBox.vCenter.z /= 2.0f;
	pNode->m_BoundingBox.fLength[0] = pNode->m_BoundingBox.vMax.x - pNode->m_BoundingBox.vCenter.x;
	pNode->m_BoundingBox.fLength[1] = pNode->m_BoundingBox.vMax.y - pNode->m_BoundingBox.vCenter.y;
	pNode->m_BoundingBox.fLength[2] = pNode->m_BoundingBox.vMax.z - pNode->m_BoundingBox.vCenter.z;
}

//--------------------------------------------------------------------------------------
// Compute LOD index
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::ComputeStaticLodIndex(int iMaxCells)
{
	m_pdwLodIndexArray.reserve(m_iPatchLodCount + 1);

	// Level 0
	StaticData* pStaticData = nullptr;
	SAFE_NEW(pStaticData, StaticData);
	pStaticData->dwLevel = 0;
	pStaticData->IndexList.resize(1);

	for (DWORD dwRow = 0; dwRow < (DWORD)iMaxCells; dwRow++)
	{
		for (DWORD dwCol = 0; dwCol < (DWORD)iMaxCells; dwCol++)
		{
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwNextCol);
		}
	}
	m_pdwLodIndexArray.push_back(pStaticData);
	CreateIndexBuffer(pStaticData);

	// 레벨 1부터 ~ m_iPatchLodCount + 1
	for (DWORD dwLevel = 0; dwLevel < (DWORD)m_iPatchLodCount; dwLevel++)
	{
		// 최소 패치의 크기가( 0 - 1 - 2 ) 정점 3개 이기 때문에 레벨=0 일 경우 오프셋=2이어야 한다.
		DWORD dwOffset = (DWORD)(pow(2.0f, float(dwLevel) + 1));
		DWORD dwNumCell = iMaxCells / dwOffset;

		StaticData* pStaticData = nullptr;
		SAFE_NEW(pStaticData, StaticData);
		pStaticData->dwLevel = dwLevel + 1;
		pStaticData->IndexList.resize(16);

		for (int iData = 0; iData < 16; iData++)
		{
			DWORD dwIndexCounter = GetIndexCounter(iData, dwNumCell);
			pStaticData->IndexList[iData].resize(dwIndexCounter);
			int iIndex = 0;

			for (DWORD dwRow = 0; dwRow < (DWORD)iMaxCells; dwRow += dwOffset)
			{
				for (DWORD dwCol = 0; dwCol < (DWORD)iMaxCells; dwCol += dwOffset)
				{
					DWORD dwTL = dwRow * (iMaxCells + 1) + dwCol;
					DWORD dwTR = dwTL + dwOffset;
					DWORD dwBL = dwOffset * (iMaxCells + 1) + dwTL;
					DWORD dwBR = dwBL + dwOffset;
					DWORD dwCP = (dwTL + dwBR) / 2;

					if (iData != 0 &&	// 0번 버퍼(모든 방향 균열 없음)가 아닐 경우에만
						(dwRow == 0 ||									// 1행
							dwRow == ((dwNumCell - 1) * dwOffset) ||	// 마지막 행
							dwCol == 0 ||								// 1열
							dwCol == ((dwNumCell - 1) * dwOffset))		// 마지막 열
						)
					{
						if ((iData & 1) && dwRow == 0)
						{
							DWORD dwUpperCenter = (dwTL + dwTR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						if ((iData & 2) && (dwCol == (dwNumCell - 1) * dwOffset))
						{
							DWORD dwRightCenter = (dwTR + dwBR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						if ((iData & 4) && (dwRow == (dwNumCell - 1) * dwOffset))
						{
							DWORD dwLowerCenter = (dwBR + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						if ((iData & 8) && (dwCol == 0))
						{
							DWORD dwLeftCenter = (dwTL + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
					}
					else
					{
						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
						pStaticData->IndexList[iData][iIndex++] = dwTR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTR;
						pStaticData->IndexList[iData][iIndex++] = dwBR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBR;
						pStaticData->IndexList[iData][iIndex++] = dwBL;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBL;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
					}
				}
			}
		}
		CreateIndexBuffer(pStaticData);
		m_pdwLodIndexArray.push_back(pStaticData);
	}
	return true;
}

//--------------------------------------------------------------------------------------
	// Get index count at current lod level
	//--------------------------------------------------------------------------------------
DWORD QuadTreeIndex::GetIndexCounter(DWORD dwData, DWORD dwNumCell)
{
	// 총 개수 = (셀 가로 개수 * 셀 세로 개수  * 4 )  +  
	//            셀 가로 개수 또는 셀 세로 개수 * dwLevelCount ) * 3;
	// dwLevelCount는  8,4,2,1 방향으로 각각 추가 될수 있는 페이스
	DWORD dwNumFaces = dwNumCell * dwNumCell * 4;
	DWORD dwLevelCount = 0;

	if (dwData & 8) dwLevelCount++;
	if (dwData & 4) dwLevelCount++;
	if (dwData & 2) dwLevelCount++;
	if (dwData & 1) dwLevelCount++;

	dwNumFaces = dwNumFaces + (dwNumCell * dwLevelCount);
	return dwNumFaces * 3;
}

//--------------------------------------------------------------------------------------
// Create a list by referencing the vertex list in the map
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::CreateVertexList(Node* pNode)
{
	DWORD dwTopLeft = pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT);
	DWORD dwTopRight = pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT);
	DWORD dwBottomLeft = pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT);
	DWORD dwBottomRight = pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_RIGHT);

	DWORD dwStartRow = dwTopLeft / m_dwWidth;
	DWORD dwEndRow = dwBottomLeft / m_dwWidth;
	DWORD dwStartCol = dwTopLeft % m_dwWidth;
	DWORD dwEndCol = dwTopRight % m_dwWidth;
	
	DWORD dwSize = (dwEndCol - dwStartCol + 1) * (dwEndRow - dwStartRow + 1);
	pNode->GetVertexList().reserve(dwSize);

	for (DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++)
		{
			DWORD dwCurrentIndex = dwRow * m_dwWidth + dwCol;
			/*PNCT_VERTEX vertex = m_pMap->m_VertexList[dwCurrentIndex];
			vertex.p.y = 0.0f;
			pNode->SetVertex(vertex);*/
			pNode->SetVertex(m_pMap->m_VertexList[dwCurrentIndex]);
		}
	}
	if (FAILED(CreateVertexBuffer(pNode)))
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// Create index list through leaf node information
//--------------------------------------------------------------------------------------
int QuadTreeIndex::UpdateIndexList(Node* pNode, DWORD dwCurrentIndex, DWORD dwNumLevel)
{
	int iNumFace = 0;

	DWORD dwStartRow = pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT) / m_dwWidth;
	DWORD dwEndRow = pNode->GetCornerIndex(QUADTREECORNER::BOTTOM_LEFT) / m_dwWidth;
	DWORD dwStartCol = pNode->GetCornerIndex(QUADTREECORNER::TOP_LEFT) % m_dwWidth;
	DWORD dwEndCol = pNode->GetCornerIndex(QUADTREECORNER::TOP_RIGHT) % m_dwWidth;

	DWORD dwOffset = pow(2.0f, static_cast<float>(dwNumLevel));

	DWORD dwCountX = ((dwEndRow - dwStartRow) / dwOffset) - 1;
	DWORD dwCountY = ((dwEndRow - dwStartRow) / dwOffset) - 1;

	DWORD dwYCell = 0;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow += dwOffset, dwYCell++)
	{
		DWORD dwXCell = 0;
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol += dwOffset, dwXCell++)
		{
			DWORD dwNextRow = dwRow + dwOffset;
			DWORD dwNextCol = dwCol + dwOffset;

			// 셀의 상단 삼각형 균열
			if (dwXCell == 0 && dwYCell == 0)
			{
				DWORD dwType = (pNode->GetLodType() & 8) + (pNode->GetLodType() & 1);
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwNextRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwNextCol,
												dwType);
			}
			else if ((dwXCell == 0) && (pNode->GetLodType() & 8))
			{
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwNextRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwNextCol,
												8);
			}
			else if ((dwYCell == 0) && (pNode->GetLodType() & 1))
			{
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwNextRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwCol,
												dwRow * m_dwWidth + dwNextCol,
												1);
			}
			else
			{
				m_indexList[dwCurrentIndex + 0] = dwNextRow * m_dwWidth + dwCol;
				m_indexList[dwCurrentIndex + 1] = dwRow * m_dwWidth + dwCol;
				m_indexList[dwCurrentIndex + 2] = dwRow * m_dwWidth + dwNextCol;
				iNumFace += 1;
				dwCurrentIndex += 3;
			}

			// 셀의 하단 삼각형 균열
			if (dwXCell == dwCountX && dwYCell == dwCountY)
			{
				DWORD dwType = (pNode->GetLodType() & 2) + (pNode->GetLodType() & 4);
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwCol,
												dwType);
			}
			else if ((dwXCell == dwCountX) && (pNode->GetLodType() & 2))
			{
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwCol,
												2);
			}
			else if ((dwYCell == dwCountY) && (pNode->GetLodType() & 4))
			{
				iNumFace += SetLodIndexBuffer(pNode, dwCurrentIndex,
												dwRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwNextCol,
												dwNextRow * m_dwWidth + dwCol,
												4);
			}
			else
			{
				m_indexList[dwCurrentIndex + 0] = dwRow * m_dwWidth + dwNextCol;
				m_indexList[dwCurrentIndex + 1] = dwNextRow * m_dwWidth + dwNextCol;
				m_indexList[dwCurrentIndex + 2] = dwNextRow * m_dwWidth + dwCol;
				iNumFace += 1;
				dwCurrentIndex += 3;
			}
		}
	}

	return iNumFace;
}

//--------------------------------------------------------------------------------------
// Create vertex buffer
//--------------------------------------------------------------------------------------
HRESULT QuadTreeIndex::CreateVertexBuffer(Node* pNode)
{
	HRESULT hr = S_OK;

	vector<PNCT_VERTEX> vertexList = pNode->GetVertexList();

	if (vertexList.empty() == true)
	{
		return S_OK;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VERTEX) * vertexList.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vertexList.at(0);

	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &pNode->m_pVertexBuffer);

	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// Create index buffer
//--------------------------------------------------------------------------------------
HRESULT	QuadTreeIndex::CreateIndexBuffer(StaticData* pData)
{
	HRESULT hr = S_OK;

	if (pData == nullptr)
	{
		return S_OK;
	}

	pData->IndexBufferList.resize(pData->IndexList.size());
	for (int iData = 0; iData < pData->IndexList.size(); iData++)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(DWORD) * pData->IndexList[iData].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &pData->IndexList[iData].at(0);

		hr = m_pd3dDevice->CreateBuffer(&bd, &data, &pData->IndexBufferList[iData]);
	}

	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// Init
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::Init(ID3D11Device* pd3dDevice, Map* pMap, Camera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pMap = pMap;
	m_pCamera = pCamera;

	Build(pMap->m_iNumRows, pMap->m_iNumCols);
	return true;
}

//--------------------------------------------------------------------------------------
// Frame
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::Frame()
{
	m_DrawPatchNodeList.clear();

	// PreFrame함수 호출 및 오브젝트 컬링
	//if (!QuadTree::Frame()) return false;

	GetDrawPatchNode();

	m_iNumFace = 0;
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		UpdateBoundingBox(m_DrawPatchNodeList[iNode]);
		GetLodType(m_DrawPatchNodeList[iNode]);
		m_iNumFace += UpdateIndexList(m_DrawPatchNodeList[iNode], m_iNumFace * 3, m_DrawPatchNodeList[iNode]->GetLodLevel());
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Render
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::Render(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;

	int length = m_DrawPatchNodeList.size();

	for (auto& pNode : m_DrawPatchNodeList)
	{
		pContext->IASetVertexBuffers(0, 1, &pNode->m_pVertexBuffer, &stride, &offset);
		pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->GetLodLevel()]->IndexBufferList[pNode->GetLodType()], DXGI_FORMAT_R32_UINT, 0);
		m_pMap->m_pTexture->Apply(pContext);
		m_pMap->PostRender(pContext, m_pdwLodIndexArray[pNode->GetLodLevel()]->IndexList[pNode->GetLodType()].size());
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------------------
bool QuadTreeIndex::Release()
{
	for (int iPatch = 0; iPatch < m_pdwLodIndexArray.size(); iPatch++)
	{
		m_pdwLodIndexArray.at(iPatch)->Release();
	}
	if (m_IndexBuffer) m_IndexBuffer->Release();

	QuadTree::Release();

	return true;
}

QuadTreeIndex::QuadTreeIndex()
{
	m_pd3dDevice = nullptr;
	m_IndexBuffer = nullptr;
	m_pMap = nullptr;
	m_pCamera = nullptr;
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_iNumFace = 0;
}

QuadTreeIndex::~QuadTreeIndex()
{
	for (int iBuffer = 0; iBuffer < m_pdwLodIndexArray.size(); iBuffer++)
	{
		SAFE_DEL(m_pdwLodIndexArray[iBuffer]);
	}
	m_pdwLodIndexArray.clear();

	m_indexList.clear();
}
