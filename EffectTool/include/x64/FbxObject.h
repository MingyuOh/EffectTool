#pragma once
#include "Mesh.h"

namespace DXCore {
	class FbxObject
	{
	public:
		FbxManager*			m_pFbxManager;
		FbxImporter*		m_pFbxImporter;
		FbxScene*			m_pFbxScene;
		vector<FbxNode*>	m_pFbxNodeList;
		vector<Material*>	m_pFbxMaterialList;
		vector<Mesh*>		m_pFbxMeshList;
		vector<TMatrix>		m_matBindPoseList;
		VSCBMatrix			m_cbData;
		TMatrix				m_matWorld;
		float				m_fElapseTime;
		float				m_fStartTime;
		float				m_fEndTime;
		float				m_fSamplingTime;
		int					m_iAnimIndex;
		bool				m_bAnimPlay;
	public:
		bool				LoadObject(string fileName, string shaderName);
		void				LoadMaterial(Material* pMaterial);
		int					GetRootMaterial(FbxSurfaceMaterial* pFbxMaterial);
		TMatrix				DxConvertMatrix(TMatrix matrix);
		TMatrix				ConvertMatrix(FbxMatrix& matrix);
		TMatrix				ConvertAMatrix(FbxAMatrix& matrix);
		Mesh*				GetFindIndex(FbxNode* pNode);
	public:
		void				PreProcess(FbxNode* pNode);
		void				ParseNode(FbxNode* pNode, Mesh* pParentMesh);
		void				ParseMesh(FbxNode* pNode, Mesh* pMesh);
		TMatrix				ParseTransform(FbxNode* pNode, TMatrix& matParent);
	public:
		void				SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
		void				SetPosition(TVector3 vPos);
		void				SetDirection(TVector3 vDir);
		void				SetPixelShader(ID3D11PixelShader* ps = nullptr);
		bool				Frame();
		bool				Render(ID3D11DeviceContext* pContext);
		bool				Release();
	public:
		FbxVector2			ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
		FbxVector4			ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets, int controlPointIndex, int iVertexIndex);
		FbxColor			ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets, DWORD dwDCCIndex, DWORD dwVertexIndex);
	public:
		void				ParseAnimation();
		void				ParseAnimationStack(FbxString* szData);
		void				ParseAnimationNode();
		bool				ParseMeshSkinning(FbxMesh* pFbxMesh, Mesh* pMesh, SkinData* pSkindata);
	public:
		FbxObject();
		~FbxObject();
	};
}
