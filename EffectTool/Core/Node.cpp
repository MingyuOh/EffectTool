#include "Node.h"

//--------------------------------------------------------------------------------------
// Verify enable put an object in node function
//--------------------------------------------------------------------------------------
bool Node::EnableExistObjectInNode(Node* pNode, Object* object)
{
	if (pNode->m_BoundingBox.vMin.x <= object->m_BoundingBox.vMin.x &&
		pNode->m_BoundingBox.vMax.x >= object->m_BoundingBox.vMax.x &&
		pNode->m_BoundingBox.vMin.y <= object->m_BoundingBox.vMin.y &&
		pNode->m_BoundingBox.vMax.y >= object->m_BoundingBox.vMax.y &&
		pNode->m_BoundingBox.vMin.z <= object->m_BoundingBox.vMin.z &&
		pNode->m_BoundingBox.vMax.z >= object->m_BoundingBox.vMax.z)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------
// Verify exsist object in node function
//--------------------------------------------------------------------------------------
bool Node::isExistObjectInNode(Object* object)
{
	for (auto& obj : m_pObjectList)
	{
		if (obj == object)
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------------------
// Add object in node function
//--------------------------------------------------------------------------------------
void Node::AddObjectInNode(Object* object)
{
	m_pObjectList.push_back(object);
}

//--------------------------------------------------------------------------------------
// Delete object in objectList
//--------------------------------------------------------------------------------------
void Node::DeleteObjectInNode(Object* object)
{
	m_pObjectList.erase(remove(m_pObjectList.begin(), m_pObjectList.end(), object), m_pObjectList.end());
}

//--------------------------------------------------------------------------------------
// Set Function
//--------------------------------------------------------------------------------------
void Node::SetBoundingBox(const CollisionBox boundingBox)
{
	m_BoundingBox = boundingBox;
}

void Node::SetDepth(const DWORD dwDepth)
{
	m_dwDepth = dwDepth;
}

void Node::SetIsLeaf(const bool bIsLeaf)
{
	m_bIsLeaf = bIsLeaf;
}

void Node::SetCorner(const TVector3 point)
{
	m_cornerList.push_back(point);
}

void Node::SetCornerList(const vector<TVector3> cornerList)
{
	m_cornerList = cornerList;
}

void Node::SetCornerIndex(const DWORD dwCornerIndex)
{
	m_cornerIndexList.push_back(dwCornerIndex);
}

void Node::SetCornerIndexList(const vector<DWORD> dwCornerIndexList)
{
	m_cornerIndexList = dwCornerIndexList;
}

void Node::SetChild(Node* child)
{
	m_childList.push_back(child);
}

void Node::SetChildList(const vector<Node*> childList)
{
	m_childList = childList;
}

void Node::SetObject(Object* object)
{
	m_pObjectList.push_back(object);
}

void Node::SetObjectList(const vector<Object*> objectList)
{
	m_pObjectList = objectList;
}

void Node::SetIndex(int index)
{
	m_iIndex = index;
}

void Node::SetVertex(PNCT_VERTEX vertex)
{
	m_vertexList.push_back(vertex);
}

void Node::SetVertexList(const vector<PNCT_VERTEX> vertecies)
{
	m_vertexList = vertecies;
}

void Node::SetIndexList(const vector<DWORD> indexList)
{
	m_indexList = indexList;
}

void Node::SetIndexList(const DWORD index)
{
	m_indexList.push_back(index);
}

void Node::SetIndexList(const DWORD index, const DWORD value)
{
	m_indexList.at(index) = value;
}

void Node::SetLodLevel(DWORD dwLodLevel)
{
	m_dwLodLevel = dwLodLevel;
}

void Node::SetLodType(DWORD dwLodType)
{
	m_dwLodType = dwLodType;
}

void Node::SetNeighborList(Node* pNode)
{
	m_NeighborNodeList.push_back(pNode);
}

void Node::SetNeighborList(Node* pNode, int index)
{
	m_NeighborNodeList[index] = pNode;
}

void Node::SetNeighborList(vector<Node*> neighborList)
{
	m_NeighborNodeList = neighborList;
}

void Node::SetPositionIndex(const POINT position)
{
	m_PositionIndex = position;
}

//--------------------------------------------------------------------------------------
// Get Function
//--------------------------------------------------------------------------------------
CollisionBox Node::GetBoundingBox()
{
	return m_BoundingBox;
}

DWORD Node::GetDepth()
{
	return m_dwDepth;
}

bool Node::GetIsLeaf()
{
	return m_bIsLeaf;
}

TVector3& Node::GetCorner(const int index)
{
	return m_cornerList.at(index);
}

vector<TVector3>& Node::GetCornerList()
{
	return m_cornerList;
}

DWORD Node::GetCornerIndex(const DWORD index)
{
	return m_cornerIndexList.at(index);
}

vector<DWORD>& Node::GetCornerIndexList()
{
	return m_cornerIndexList;
}

Node* Node::GetChild(const int index)
{
	return m_childList.at(index);
}

vector<Node*>& Node::GetChildList()
{
	return m_childList;
}

Object* Node::GetCustomObject(const int index)
{
	return m_pObjectList.at(index);
}

int Node::GetIndex()
{
	return m_iIndex;
}

vector<Object*>& Node::GetObjectList()
{
	return m_pObjectList;
}

PNCT_VERTEX Node::GetVertex(const int index)
{
	return m_vertexList.at(index);
}

vector<PNCT_VERTEX>& Node::GetVertexList()
{
	return m_vertexList;
}

vector<DWORD> Node::GetIndexList()
{
	return m_indexList;
}

DWORD Node::GetIndexList(const DWORD value)
{
	return m_indexList.at(value);
}

DWORD Node::GetLodLevel()
{
	return m_dwLodLevel;
}

DWORD Node::GetLodType()
{
	return m_dwLodType;
}

Node* Node::GetNeighbor(int index)
{
	return m_NeighborNodeList.at(index);
}

vector<Node*>& Node::GetNeighborList()
{
	return m_NeighborNodeList;
}

POINT Node::GetPositionIndex()
{
	return m_PositionIndex;
}

Node::Node()
{
	m_iIndex = 0;
	m_dwDepth = 0;
	m_dwLodLevel = 0;
	m_dwLodType = 0;
	m_PositionIndex = { 0, };
	m_bIsLeaf = false;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}

Node::~Node()
{
	if (m_cornerList.empty() == false)
	{
		m_cornerList.clear();
	}

	if (m_cornerIndexList.empty() == false)
	{
		m_cornerIndexList.clear();
	}

	for (int index = 0; index < 4; index++)
	{
		if (m_childList.empty() == false && m_childList.at(index) != nullptr)
		{
			SAFE_DEL(m_childList[index]);
			m_childList.at(index) = nullptr;
		}
	}

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}