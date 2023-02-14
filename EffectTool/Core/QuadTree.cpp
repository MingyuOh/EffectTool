#include "QuadTree.h"

//--------------------------------------------------------------------------------------
// Create Node
//--------------------------------------------------------------------------------------
Node* QuadTree::CreateNode(Node* pParent, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	Node* pNode = NULL;
	SAFE_NEW(pNode, Node);

	pNode->GetChildList().reserve(4);
	pNode->GetCornerList().reserve(4);

	CollisionBox boundingBox;
	boundingBox.vMin = TVector3(fTopLeft, 0.0f, fBottomLeft);
	boundingBox.vMax = TVector3(fTopRight, 0.0f, fBottomRight);
	boundingBox.vCenter = (boundingBox.vMax - boundingBox.vMin);
	boundingBox.vCenter.x /= 2.0f;
	boundingBox.vCenter.y /= 2.0f;
	boundingBox.vCenter.z /= 2.0f;
	boundingBox.fLength[0] = boundingBox.vMax.x - boundingBox.vCenter.x;
	boundingBox.fLength[1] = boundingBox.vMax.y - boundingBox.vCenter.y;
	boundingBox.fLength[2] = boundingBox.vMax.z - boundingBox.vCenter.z;
	
	pNode->SetBoundingBox(boundingBox);

	pNode->SetCorner(TVector3(pNode->GetBoundingBox().vMin.x, 0.0f, pNode->GetBoundingBox().vMax.z));
	pNode->SetCorner(pNode->GetBoundingBox().vMax);
	pNode->SetCorner(pNode->GetBoundingBox().vMin);
	pNode->SetCorner(TVector3(pNode->GetBoundingBox().vMax.x, 0.0f, pNode->GetBoundingBox().vMin.z));

	if (pParent)
	{
		pNode->SetDepth(pParent->GetDepth() + 1);
		if (kQuadtreeMaxDepthLimit < pNode->GetDepth())
		{
			m_iMaxDepth = pNode->GetDepth();
		}
	}
	return pNode;
}

//--------------------------------------------------------------------------------------
// Delete Node
//--------------------------------------------------------------------------------------
bool QuadTree::DeleteNode(Node* pNode)
{
	assert(pNode);
	delete pNode;

	return true;
}

//--------------------------------------------------------------------------------------
// Build
//--------------------------------------------------------------------------------------
bool QuadTree::Build(float fWidth, float fHeight)
{
	m_pRootNode = CreateNode(NULL, -fWidth / 2, fWidth / 2, -fHeight / 2, fHeight / 2);
	m_pRootNode->SetIndex(m_iNodeCount++);

	if (BuildTree(m_pRootNode))
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------
// Create QuadTree
//--------------------------------------------------------------------------------------
bool QuadTree::BuildTree(Node* pNode)
{
	if (SubDivide(pNode) == true)
	{
		for (int iNode = 0; iNode < pNode->GetChildList().size(); iNode++)
		{
			DWORD dwIndex = pNode->GetChild(iNode)->GetPositionIndex().y * pow(2.0f, static_cast<float>(pNode->GetChild(iNode)->GetDepth())) + pNode->GetChild(iNode)->GetPositionIndex().x;
			Node* pChildNode = pNode->GetChildList().at(iNode);

			m_LevelList[pChildNode->GetDepth()][dwIndex] = pChildNode;
			BuildTree(pChildNode);
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------
// Divide space
//--------------------------------------------------------------------------------------
bool QuadTree::SubDivide(Node* pNode)
{
	// 4등분 할 수 없으면 더이상 분할하지 않는다.
	assert(pNode);

	// 현재 노드의 실제 크기를 계산
	float fWidthSplit = (pNode->GetCorner(QUADTREECORNER::TOP_RIGHT).x - pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x) / 2;
	float fHeightSplit = (pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z - pNode->GetCorner(QUADTREECORNER::BOTTOM_LEFT).z) / 2;

	// 최대 깊이 한도 초과 또는 자식 노드가 지정된 분할크기보다 작다면 리프노드 지정
	if (kQuadtreeMaxDepthLimit <= pNode->GetDepth() ||
		fWidthSplit < kQuadtreeMinDivideSize || 
		fHeightSplit < kQuadtreeMinDivideSize)
	{
		pNode->SetIsLeaf(true);
		m_DrawNodeList.push_back(pNode);
		return false;
	}

	// left right bottom top
	// 왼쪽 상단 노드 ( 1번 노드 )
	pNode->SetChild(CreateNode(pNode,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x + fWidthSplit,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z - fHeightSplit,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z
	));
	pNode->SetIndex(m_iNodeCount++);

	// 오른쪽 상단 노드 ( 2번 노드 )
	pNode->SetChild(CreateNode(pNode,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x + fWidthSplit,
		pNode->GetCorner(QUADTREECORNER::TOP_RIGHT).x,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z - fHeightSplit,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z
	));
	pNode->SetIndex(m_iNodeCount++);

	// 왼쪽 하단 노드 ( 3번 노드 )
	pNode->SetChild(CreateNode(pNode,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x + fWidthSplit,
		pNode->GetCorner(QUADTREECORNER::BOTTOM_LEFT).z,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z - fHeightSplit
	));
	pNode->SetIndex(m_iNodeCount++);

	// 오른쪽 하단 노드 ( 4번 노드 )
	pNode->SetChild(CreateNode(pNode,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).x + fWidthSplit,
		pNode->GetCorner(QUADTREECORNER::TOP_RIGHT).x,
		pNode->GetCorner(QUADTREECORNER::BOTTOM_RIGHT).z,
		pNode->GetCorner(QUADTREECORNER::TOP_LEFT).z - fHeightSplit
	));
	pNode->SetIndex(m_iNodeCount++);

	return true;
}

//--------------------------------------------------------------------------------------
// Set neighbor node
//--------------------------------------------------------------------------------------
void QuadTree::SetNeighborNode(Node* pNode)
{
	FindNeighborNode(pNode);
	for (int iNode = 0; iNode < pNode->GetChildList().size(); iNode++)
	{
		SetNeighborNode(pNode->GetChildList().at(iNode));
	}
}

//--------------------------------------------------------------------------------------
// Find neighbor node
//--------------------------------------------------------------------------------------
void QuadTree::FindNeighborNode(Node* pNode)
{
	pNode->GetNeighborList().resize(4);

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, static_cast<float>(pNode->GetDepth()));

	DWORD dwNeighborCol, dwNeighborRow;
	// 상
	if (pNode->GetPositionIndex().y > 0)
	{
		dwNeighborCol = pNode->GetPositionIndex().x;
		dwNeighborRow = ( pNode->GetPositionIndex().y - 1 ) * dwNumPatchCount;
		pNode->SetNeighborList(m_LevelList[pNode->GetDepth()][dwNeighborRow + dwNeighborCol], 0);
	}
	// 하
	if (pNode->GetPositionIndex().y < dwNumPatchCount - 1)
	{
		dwNeighborCol = pNode->GetPositionIndex().x;
		dwNeighborRow = (pNode->GetPositionIndex().y + 1) * dwNumPatchCount;
		pNode->SetNeighborList(m_LevelList[pNode->GetDepth()][dwNeighborRow + dwNeighborCol], 1);
	}
	// 좌
	if (pNode->GetPositionIndex().x > 0)
	{
		dwNeighborCol = pNode->GetPositionIndex().x - 1;
		dwNeighborRow = pNode->GetPositionIndex().y * dwNumPatchCount;
		pNode->SetNeighborList(m_LevelList[pNode->GetDepth()][dwNeighborRow + dwNeighborCol], 2);
	}
	// 우
	if (pNode->GetPositionIndex().x < dwNumPatchCount - 1)
	{
		dwNeighborCol = pNode->GetPositionIndex().x + 1;
		dwNeighborRow = pNode->GetPositionIndex().y * dwNumPatchCount;
		pNode->SetNeighborList(m_LevelList[pNode->GetDepth()][dwNeighborRow + dwNeighborCol], 3);
	}
}

//--------------------------------------------------------------------------------------
// Find node
//--------------------------------------------------------------------------------------
Node* QuadTree::FindNodeInQuadTree(Node* pNode, Object* object)
{
	assert(pNode);

	while (pNode != nullptr && pNode->GetIsLeaf() == false)
	{
		for (int index = 0; index < 4; index++)
		{
			if (pNode->GetChild(index) != nullptr
				&& pNode->GetChild(index)->EnableExistObjectInNode(pNode, object) == true)
			{
				m_QuadTreeQueue.push(pNode->GetChild(index));
				break;
			}
		}
		if (m_QuadTreeQueue.empty() == true)
		{
			break;
		}
		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	}

	if (pNode == m_pRootNode)
	{
		pNode = nullptr;
	}

	return pNode;
}

//--------------------------------------------------------------------------------------
// Delete an object from the list of objects in a node
//--------------------------------------------------------------------------------------
bool QuadTree::DeleteObject(Node* pNode, Object* pObject)
{
	if (pNode->isExistObjectInNode(pObject) == true)
	{
		pNode->DeleteObjectInNode(pObject);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------
// Add object in quadtree
//--------------------------------------------------------------------------------------
bool QuadTree::AddInQuadTree(Node* pNode, Object* pObject)
{
	if (pNode != nullptr)
	{
		pNode->AddObjectInNode(pObject);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------------
bool QuadTree::UpdateQuadTree(Object* pObject)
{
	assert(pObject);

	/*Node* pNode = FindNodeInQuadTree(pObject->GetBoundingBox().GetBoundingBoxInfo());
	if (pNode != nullptr)
	{
		DeleteObject(pNode, pObject);
		AddInQuadTree(pNode, pObject);
		return true;
	}*/
	return false;
}

//--------------------------------------------------------------------------------------
// Split size calculation function
//--------------------------------------------------------------------------------------
DWORD QuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0)
		return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount++;
	}

	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);
	float fMin = fabs(fMinimum - fabs(dwSize - fMinimum));
	float fMax = fabs(fMaximum - fabs(dwSize - fMaximum));

	if (fMin <= fMax)
	{
		return static_cast<DWORD>(fMinimum);
	}
	return static_cast<DWORD>(fMaximum);
}

//--------------------------------------------------------------------------------------
// Render
//--------------------------------------------------------------------------------------
bool QuadTree::Render()
{
	return true;
}

//--------------------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------------------
bool QuadTree::Release()
{
	// 루트 노드를 타고 최대 깊이까지 모든 자식들을 지워야함
	SAFE_DEL(m_pRootNode);
	return true;
}

//--------------------------------------------------------------------------------------
// Set Function
//--------------------------------------------------------------------------------------
void QuadTree::SetRootNode(Node* pNode)
{
	m_pRootNode = pNode;
}

void QuadTree::SetMaxDepth(int iDepth)
{
	m_iMaxDepth = iDepth;
}

void QuadTree::SetRowCount(const int iRowNum)
{
	m_iNumRow = iRowNum;
}

void QuadTree::SetColumnCount(const int iColNum)
{
	m_iNumCol = iColNum;
}


//--------------------------------------------------------------------------------------
// Get Function
//--------------------------------------------------------------------------------------
Node* QuadTree::GetRootNode()
{
	return m_pRootNode;
}

vector<RECT> QuadTree::GetLeafNodePosition()
{
	vector<RECT> rectList;
	return rectList;
}

int QuadTree::GetMaxDepth()
{
	return m_iMaxDepth;
}

int QuadTree::GetRowCount()
{
	return m_iNumRow;
}

int QuadTree::GetColumnCount()
{
	return m_iNumCol;
}

int QuadTree::GetMinDivideSize()
{
	return m_iMinDivideSize;
}

int QuadTree::GetMaxDepthLimit()
{
	return m_iMaxDepthLimit;
}

QuadTree::QuadTree()
{
	m_pRootNode = nullptr;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = kQuadtreeMaxDepthLimit;
	m_iMinDivideSize = kQuadtreeMinDivideSize;
	m_iNodeCount = 0;
	m_iNumRow = 0;
	m_iNumCol = 0;
}


QuadTree::~QuadTree()
{
	Release();
}
