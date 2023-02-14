#pragma once
#include "Object.h"
#include "BoundingBoxShape.h"
#include "fbxsdk.h"

#pragma comment		(lib, "libfbxsdk-md.lib")
#pragma comment		(lib, "libfbxsdk.lib")
#pragma comment		(lib, "libxml2-md.lib")
#pragma comment		(lib, "zlib-md.lib")

const enum class OBJECT_CLASS_TYPE {
	CLASS_GEOM,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

struct Material
{
	FbxNode*				pFbxNode;
	FbxSurfaceMaterial*		pFbxSurfaceMaterial;
	Texture					texture;
	vector<Material*>		pSubMaterial;
	Material()
	{
		pFbxNode = nullptr;
		pFbxSurfaceMaterial = nullptr;
	}
	Material(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		this->pFbxNode = pFbxNode;
		this->pFbxSurfaceMaterial = pFbxMtrl;
	}
	void Release()
	{
		texture.Release();
		for (auto data : pSubMaterial)
		{
			data->Release();
			delete data;
		}
	}
};

struct Layer
{
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};

struct Weight
{
	vector<int>					m_IndexList;
	vector<float>				m_WeightList;
};

struct SkinData
{
	//vector<FbxNode*>			m_MatrixList;
	vector<Weight>				m_VertexList;
};

struct PNCTIW_VERTEX
{
	float   index[4];
	float   weight[4];
};

struct AnimMatrix
{
	TMatrix   matAnimation[255];
};

class Mesh : public Object
{
public:
	ID3D11Buffer*				m_pIWVertrexBuffer;
	ID3D11Buffer*				m_pAnimConstantBuffer;
	vector<FbxNode*>			m_pFbxNodeList;
	vector<Mesh*>				m_pMeshLinkList;
	vector<TMatrix>				m_matBindPoseList;
	vector<Layer>				m_LayerList;
	vector<Mesh*>				m_pSubMesh;
	vector<TMatrix>				m_AnimationTrack;
	vector<PNCTIW_VERTEX>		m_WeightList;
	FbxNode*					m_pFbxNode;
	Mesh*						m_pParent;
	AnimMatrix					m_matAnimMatrix;
	Shape*						m_pShape;
	T_STR						m_szName;
	T_STR						m_szParentName;
	OBJECT_CLASS_TYPE			m_ClassType;
	int							m_iNumLayer;
	int							m_iMaterialRef;
public:
	HRESULT			CreateVertexBuffer() override;
	HRESULT			CreateConstantBuffer() override;
	HRESULT			CreateVertexLayout() override;
	bool			PreRender(ID3D11DeviceContext* pContext) override;
	bool			Release() override;
public:
	Mesh();
	~Mesh();
};

