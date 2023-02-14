#pragma once
#include "Node.h"

typedef vector<Node*>		NODE_VECTOR;

class MapLOD
{
protected:
	Camera*					m_pCamera;
	vector<NODE_VECTOR>		m_LevelList;
	vector<Node*>			m_DrawPatchNodeList;
	vector<Node*>			m_DrawNodeList;
	float					m_fExpansionRatio;		// LOD ����(ȭ����������(0 ~ 1 ���� ���)
	int						m_iNumCell;				// Ʈ�� ������ ���̿� ���� �� ����
	int						m_iPatchLodCount;		// LOD ��������(z=pow(x,y)���� y = log(z)/log(x))
	bool					m_bThresHoldValue;
public:
	ID3D11Device*			m_pd3dDevice;
public:
	//--------------------------------------------------------------------------------------
	// Init
	//--------------------------------------------------------------------------------------
	virtual void			InitLOD(Node* pRootNode, int iMaxDepth);
	//--------------------------------------------------------------------------------------
	// Add draw patch node
	//--------------------------------------------------------------------------------------
	virtual void			AddDrawPatchNode(Node* pNode);
	//--------------------------------------------------------------------------------------
	// Update
	//--------------------------------------------------------------------------------------
	virtual void			Update(Camera* pCamera = nullptr);
	//--------------------------------------------------------------------------------------
	// Set funtion
	//--------------------------------------------------------------------------------------
	virtual void			SetLOD(DWORD dwWIdth, int iNumDepth);
	virtual void			SetExpansionRatio(int iRatio);
	//--------------------------------------------------------------------------------------
	// Get function
	//--------------------------------------------------------------------------------------
	virtual DWORD			GetLodSubIndex(Node* pNode);
	virtual DWORD			GetLodType(Node* pNode);
	virtual float			GetExpansionRatio(TVector3 vCenter);
	virtual float			GetExpansionRatio(TVector3 vMax, TVector3 vMin);
	virtual void			GetDrawPatchNode();
public:
	MapLOD();
	virtual ~MapLOD();
};

