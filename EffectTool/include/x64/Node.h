#pragma once
#include "CoreSTD.h"
#include "HeightMap.h"
#include "Camera.h"

class Node
{
private:
	vector<TVector3>			m_cornerList;
	vector<DWORD>				m_cornerIndexList;
	vector<Object*>				m_pObjectList;
	vector<Node*>				m_childList;
	vector<DWORD>				m_indexList;
	vector<PNCT_VERTEX>			m_vertexList;
	DWORD						m_dwDepth;
	bool						m_bIsLeaf;
	int							m_iIndex;
	DWORD						m_dwLodLevel;			// LOD 레벨(0 ~ dwLodLevel)
	DWORD						m_dwLodType;			// 4방향 이웃노드에 대한 LOD 타입
	vector<Node*>				m_NeighborNodeList;		// 이웃 노드 리스트
	POINT						m_PositionIndex;
public:
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	CollisionBox				m_BoundingBox;
public:
	//--------------------------------------------------------------------------------------
	// Verify exsist object in node function
	//--------------------------------------------------------------------------------------
	bool					EnableExistObjectInNode(Node* pNode, Object* object);
	//--------------------------------------------------------------------------------------
	// Verify exsist object in node function
	//--------------------------------------------------------------------------------------
	bool					isExistObjectInNode(Object* object);
	//--------------------------------------------------------------------------------------
	// Add object in node function
	//--------------------------------------------------------------------------------------
	void					AddObjectInNode(Object* object);
	//--------------------------------------------------------------------------------------
	// Delete object in objectList
	//--------------------------------------------------------------------------------------
	void					DeleteObjectInNode(Object* object);
	//--------------------------------------------------------------------------------------
	// Set Function
	//--------------------------------------------------------------------------------------
	void					SetBoundingBox(const CollisionBox boundingBox);
	void					SetDepth(const DWORD dwDepth);
	void					SetIsLeaf(const bool bIsLeaf);
	void					SetCorner(const TVector3 corner);
	void					SetCornerList(const vector<TVector3> cornerList);
	void					SetCornerIndex(const DWORD dwCornerIndex);
	void					SetCornerIndexList(const vector<DWORD> dwCornerIndexList);
	void					SetChild(Node* child);
	void					SetChildList(const vector<Node*> childList);
	void					SetObject(Object* object);
	void					SetObjectList(const vector<Object*> objectList);
	void					SetIndex(int index);
	void					SetVertex(PNCT_VERTEX vertex);
	void					SetVertexList(const vector<PNCT_VERTEX> vertecies);
	void					SetIndexList(const vector<DWORD> indexList);
	void					SetIndexList(const DWORD index);
	void					SetIndexList(const DWORD index, const DWORD value);
	void					SetLodLevel(DWORD dwLodLevel);
	void					SetLodType(DWORD dwLodType);
	void					SetNeighborList(Node* pNode);
	void					SetNeighborList(Node* pNode, int index);
	void					SetNeighborList(vector<Node*> neighborList);
	void					SetPositionIndex(const POINT position);
	//--------------------------------------------------------------------------------------
	// Get Function
	//--------------------------------------------------------------------------------------
	CollisionBox			GetBoundingBox();
	DWORD					GetDepth();
	bool					GetIsLeaf();
	TVector3&				GetCorner(const int index);
	vector<TVector3>&		GetCornerList();
	DWORD					GetCornerIndex(const DWORD index);
	vector<DWORD>&			GetCornerIndexList();
	Node*					GetChild(const int index);
	vector<Node*>&			GetChildList();
	Object*					GetCustomObject(const int index);
	vector<Object*>&		GetObjectList();
	int						GetIndex();
	PNCT_VERTEX				GetVertex(const int index);
	vector<PNCT_VERTEX>&	GetVertexList();
	vector<DWORD>			GetIndexList();
	DWORD					GetIndexList(const DWORD value);
	DWORD					GetLodLevel();
	DWORD					GetLodType();
	Node*					GetNeighbor(int index);
	vector<Node*>&			GetNeighborList();
	POINT					GetPositionIndex();
public:
	Node();
	~Node();
};

