#pragma once
#include "QuadTree.h"
#include "Camera.h"
typedef vector<DWORD>	DWORD_VECTOR;

class StaticData
{
public:
	DWORD							dwLevel;
	vector<DWORD_VECTOR>			IndexList;
	vector<ID3D11Buffer*>			IndexBufferList;
public:
	void Release()
	{
		for (int iBuffer = 0; iBuffer < IndexBufferList.size(); iBuffer++)
		{
			IndexBufferList[iBuffer]->Release();
			IndexBufferList[iBuffer] = nullptr;
		}
		IndexBufferList.clear();
	}
	StaticData() 
	{ 
		dwLevel = 0; 
	}
	~StaticData() 
	{
		Release();
	}
};

class QuadTreeIndex : public QuadTree
{
private:
	Map*					m_pMap;
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	vector<DWORD>			m_indexList;
	int						m_iNumFace;
	vector<StaticData*>		m_pdwLodIndexArray;
	ID3D11Buffer*			m_IndexBuffer;
public:
	//--------------------------------------------------------------------------------------
	// Build
	//--------------------------------------------------------------------------------------
	bool					Build(DWORD dwWidth, DWORD dwHeight);
	//--------------------------------------------------------------------------------------
	// Create Node
	//--------------------------------------------------------------------------------------
	Node*					CreateNode(Node* pParent, DWORD dwTopLeft, DWORD dwTopRight, DWORD dwBottomLeft, DWORD dwBottomRight);
	//--------------------------------------------------------------------------------------
	// Divide QuadTree
	//--------------------------------------------------------------------------------------
	bool					SubDivide(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Set LOD index buffer
	//--------------------------------------------------------------------------------------
	int						SetLodIndexBuffer(Node* pNode, DWORD& dwCurrentIndex,
												DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType);
	//--------------------------------------------------------------------------------------
	// A function to get the height value of a vertex through node information
	//------------------------------------------------------------------------------------------
	TVector2				GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	//--------------------------------------------------------------------------------------
	// Update boundingbox
	//--------------------------------------------------------------------------------------
	void					UpdateBoundingBox(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Compute boundingbox
	//--------------------------------------------------------------------------------------
	void					ComputeBoungigBox(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Compute LOD index
	//--------------------------------------------------------------------------------------
	bool					ComputeStaticLodIndex(int iMaxCells);
	//--------------------------------------------------------------------------------------
	// Get index count at current lod level
	//--------------------------------------------------------------------------------------
	DWORD					GetIndexCounter(DWORD dwData, DWORD dwNumCell);
	//--------------------------------------------------------------------------------------
	// Create a list by referencing the vertex list in the map
	//--------------------------------------------------------------------------------------
	bool					CreateVertexList(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Create index list through leaf node information
	//--------------------------------------------------------------------------------------
	int						UpdateIndexList(Node* pNode, DWORD dwCurrentIndex, DWORD dwNumLevel);
	//--------------------------------------------------------------------------------------
	// Create vertex buffer
	//--------------------------------------------------------------------------------------
	HRESULT					CreateVertexBuffer(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Create index buffer
	//--------------------------------------------------------------------------------------
	HRESULT					CreateIndexBuffer(StaticData* pData = nullptr);
	//--------------------------------------------------------------------------------------
	// Init
	//--------------------------------------------------------------------------------------
	bool					Init(ID3D11Device* pd3dDevice, Map* pMap, Camera* pCamera);
	//--------------------------------------------------------------------------------------
	// Frame
	//--------------------------------------------------------------------------------------
	bool					Frame();
	//--------------------------------------------------------------------------------------
	// Render
	//--------------------------------------------------------------------------------------
	bool					Render(ID3D11DeviceContext* pContext);
	//--------------------------------------------------------------------------------------
	// Release
	//--------------------------------------------------------------------------------------
	bool					Release();
public:
	QuadTreeIndex();
	~QuadTreeIndex();
};

