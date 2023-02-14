#pragma once
#include "Object.h"

struct MapInfo
{
	UINT	 iWidth;
	UINT	 iHeight;
	float	 fSellDistance;
	float	 fScaleHeight;
	T_STR	 strTextureFile;
	T_STR	 strShaderFile;
};

class Map :public Object
{
public:
	vector<int>				m_NormalLookupTable;
	vector<TVector3>		m_FaceNormals;
	vector<float>			m_fHeightArray;
	MapInfo					m_MapInfo;
	UINT					m_iNumRows;
	UINT					m_iNumCols;
	UINT					m_iNumCells;
	UINT					m_iNumFaces;
public:
	//--------------------------------------------------------------------------------------
	// 정점 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool					CreateVertexData()override;
	HRESULT					CreateVertexBuffer()override;
	//--------------------------------------------------------------------------------------
	// 인덱스 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool					CreateIndexData() override;
	HRESULT					CreateIndexBuffer()override;
	//--------------------------------------------------------------------------------------
	// 맵 생성
	//--------------------------------------------------------------------------------------
	bool					CreateMap(UINT iWidth, UINT iHeight, float fDistance);
	//--------------------------------------------------------------------------------------
	// 정점 노말 생성
	//--------------------------------------------------------------------------------------
	void					UpdateNormal();
	//--------------------------------------------------------------------------------------
	// 페이스 노말 리스트 구축
	//--------------------------------------------------------------------------------------
	void					InitFaceNormals();
	//--------------------------------------------------------------------------------------
	// 정점 룩업테이블 구축
	//--------------------------------------------------------------------------------------
	void					GenerateNormalLookUpTable();
	//--------------------------------------------------------------------------------------
	// 페이스 개수만큼 계산
	//--------------------------------------------------------------------------------------
	void					CalcPerVertexNormalsLookup();
	//--------------------------------------------------------------------------------------
	// Init
	//--------------------------------------------------------------------------------------
	bool					Init(UINT iWidth = 0, UINT iHeight = 0);
	//--------------------------------------------------------------------------------------
	// Get function
	//--------------------------------------------------------------------------------------
	virtual		float		GetHeight(int iRow, int iCol);
	virtual		float		GetHeight(TVector3 vPos);
public:
	Map();
	~Map();
};

