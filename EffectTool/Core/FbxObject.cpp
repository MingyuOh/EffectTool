#include "FbxObject.h"

bool Compare(const pair<float, int>& a, const pair<float, int>& b)
{
	if (a.first == b.first)
		return a.second > b.second;
	return a.first > b.first;
}

namespace DXCore {
	bool FbxObject::LoadObject(string fileName, string shaderName)
	{
		m_pFbxManager = FbxManager::Create();
		m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
		m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
		bool bResult = m_pFbxImporter->Initialize(fileName.c_str());
		bResult = m_pFbxImporter->Import(m_pFbxScene);
		FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);

		FbxNode* pRootNode = m_pFbxScene->GetRootNode();
		PreProcess(pRootNode);

		for (int iMaterial = 0; iMaterial < m_pFbxMaterialList.size(); iMaterial++)
		{
			Material* pMaterial = m_pFbxMaterialList[iMaterial];
			LoadMaterial(pMaterial);
		}

		ParseAnimation();
		ParseNode(pRootNode, nullptr);
		ParseAnimationNode();

		for (int iMesh = 0; iMesh < m_pFbxMeshList.size(); iMesh++)
		{
			Mesh* pMesh = m_pFbxMeshList[iMesh];
			for (int iBone = 0; iBone < pMesh->m_pFbxNodeList.size(); iBone++)
			{
				Mesh* pLinkMesh = GetFindIndex(pMesh->m_pFbxNodeList[iBone]);
				pMesh->m_pMeshLinkList.push_back(pLinkMesh);
			}

			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (int iSubMesh = 0; iSubMesh < m_pFbxMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
				{
					Mesh* pSubMesh = m_pFbxMeshList[iMesh]->m_pSubMesh[iSubMesh];
					if (SUCCEEDED(pSubMesh->Create(_T(""), mtw(shaderName), mtw(shaderName))))
					{
						if (pSubMesh->ComputeBoungigBox(&TMatrix()))
						{
							// 랜더 바운딩 박스 생성
							SAFE_NEW(pSubMesh->m_pShape, BoundingBoxShape);
							pSubMesh->m_pShape->CreateOBBBox(
								pSubMesh->m_BoundingBox.fLength[0],
								pSubMesh->m_BoundingBox.fLength[1],
								pSubMesh->m_BoundingBox.fLength[2],
								pSubMesh->m_BoundingBox.vCenter,
								pSubMesh->m_BoundingBox.vAxis[0], pSubMesh->m_BoundingBox.vAxis[1], pSubMesh->m_BoundingBox.vAxis[2]
							);
							if (FAILED(pSubMesh->m_pShape->Create(_T(""), _T("../data/shader/LineVertexShader.vsh"), _T("../data/shader/LinePixelShader.psh"))))
							{
								MessageBox(0, _T("BoundingBoxShape 실패"), _T("Fatal error"), MB_OK);
								return 0;
							}
						}
					}
				}
			}
			else
			{
				if (SUCCEEDED(pMesh->Create(_T(""), mtw(shaderName), mtw(shaderName))))
				{
					if (pMesh->ComputeBoungigBox(&TMatrix()))
					{
						SAFE_NEW(pMesh->m_pShape, BoundingBoxShape);
						pMesh->m_pShape->CreateOBBBox(
							pMesh->m_BoundingBox.fLength[0],
							pMesh->m_BoundingBox.fLength[1],
							pMesh->m_BoundingBox.fLength[2],
							pMesh->m_BoundingBox.vCenter,
							pMesh->m_BoundingBox.vAxis[0], pMesh->m_BoundingBox.vAxis[1], pMesh->m_BoundingBox.vAxis[2]
						);
						if (FAILED(pMesh->m_pShape->Create(_T(""), _T("../data/shader/LineVertexShader.vsh"), _T("../data/shader/LinePixelShader.psh"))))
						{
							MessageBox(0, _T("BoundingBoxShape 실패"), _T("Fatal error"), MB_OK);
							return 0;
						}
					}
				}
			}
		}

		m_pFbxScene->Destroy();
		m_pFbxImporter->Destroy();
		m_pFbxManager->Destroy();
		return bResult;
	}

	void FbxObject::LoadMaterial(Material* pMaterial)
	{
		if (pMaterial == nullptr)
		{
			return;
		}

		if (pMaterial->pSubMaterial.size() > 0)
		{
			for (int iSubMaterial = 0; iSubMaterial < pMaterial->pSubMaterial.size(); iSubMaterial++)
			{
				Material* pSubMaterial = pMaterial->pSubMaterial[iSubMaterial];
				FbxSurfaceMaterial* pFbxMaterial = pSubMaterial->pFbxSurfaceMaterial;
				FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
				if (prop.IsValid() == true)
				{
					int iTextureCount = prop.GetSrcObjectCount<FbxTexture>();
					for (int iTexture = 0; iTexture < iTextureCount; iTexture++)
					{
						FbxTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(iTexture);
						if (pTexture == nullptr)
						{
							continue;
						}
						FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTexture);

						string szFileName;
						char Drive[MAX_PATH]	= { 0, };
						char Dir[MAX_PATH]		= { 0, };
						char FileName[MAX_PATH] = { 0, };
						char Ext[MAX_PATH]		= { 0, };
						if (fileTexture->GetFileName() != nullptr)
						{
							_splitpath(fileTexture->GetFileName(), Drive, Dir, FileName, Ext);
							Ext[4] = '\0';
							szFileName = FileName;
							if (_stricmp(Ext, ".tga") == 0)
							{
								strcpy_s(Ext, ".dds");
							}
							szFileName += Ext;
						}
						
						T_STR szTextureName = g_szDataPath;
						szTextureName += _T("texture/");
						szTextureName += mtw(szFileName);

						pSubMaterial->texture.LoadTexture(szTextureName, mtw(Ext));
					}
				}
			}
		}
		else
		{
			FbxSurfaceMaterial* pFbxMaterial = pMaterial->pFbxNode->GetMaterial(0);
			FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (prop.IsValid() == true)
			{
				int iTextureCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int iTexture = 0; iTexture < iTextureCount; iTexture++)
				{
					FbxTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(iTexture);
					if (pTexture == nullptr)
					{
						continue;
					}
					FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTexture);

					string szFileName;
					char Drive[MAX_PATH] = { 0, };
					char Dir[MAX_PATH] = { 0, };
					char FileName[MAX_PATH] = { 0, };
					char Ext[MAX_PATH] = { 0, };
					if (fileTexture->GetFileName() != nullptr)
					{
						_splitpath(fileTexture->GetFileName(), Drive, Dir, FileName, Ext);
						Ext[4] = '\0';
						szFileName = FileName;
						if (_stricmp(Ext, ".tga") == 0)
						{
							strcpy_s(Ext, ".dds");
						}
						szFileName += Ext;
					}

					T_STR szTextureName = g_szDataPath;
					szTextureName += _T("texture/");
					szTextureName += mtw(szFileName);

					pMaterial->texture.LoadTexture(szTextureName, mtw(Ext));
				}
			}
		}
	}

	int	FbxObject::GetRootMaterial(FbxSurfaceMaterial* pFbxMaterial)
	{
		for (int iMaterial = 0; iMaterial < m_pFbxMaterialList.size(); iMaterial++)
		{
			if (m_pFbxMaterialList[iMaterial]->pFbxSurfaceMaterial == pFbxMaterial)
			{
				return iMaterial;
			}
		}
		return -1;
	}

	TMatrix	FbxObject::DxConvertMatrix(TMatrix matrix)
	{
		TMatrix mat;

		mat._11 = matrix._11; mat._12 = matrix._13; mat._13 = matrix._12;
		mat._21 = matrix._31; mat._22 = matrix._33; mat._23 = matrix._32;
		mat._31 = matrix._21; mat._32 = matrix._23; mat._33 = matrix._22;
		mat._41 = matrix._41; mat._42 = matrix._43; mat._43 = matrix._42;

		mat._14 = mat._24 = mat._34 = 0.0f;
		mat._44 = 1.0f;

		return mat;
	}

	TMatrix	FbxObject::ConvertMatrix(FbxMatrix& matrix)
	{
		TMatrix mat;
		float* pMatArray = reinterpret_cast<float*>(&mat);
		double* pSrcArray = reinterpret_cast<double*>(&matrix);

		for (int index = 0; index < 16; index++)
		{
			pMatArray[index] = pSrcArray[index];
		}

		return mat;
	}

	TMatrix FbxObject::ConvertAMatrix(FbxAMatrix& matrix)
	{
		TMatrix mat;
		float* pMatArray = reinterpret_cast<float*>(&mat);
		double* pSrcArray = reinterpret_cast<double*>(&matrix);
		for (int index = 0; index < 16; index++)
		{
			pMatArray[index] = pSrcArray[index];
		}

		return mat;
	}

	Mesh* FbxObject::GetFindIndex(FbxNode* pNode)
	{
		for (int iNode = 0; iNode < m_pFbxMeshList.size(); iNode++)
		{
			Mesh* pMesh = m_pFbxMeshList[iNode];
			if (pMesh->m_pFbxNode == pNode)
			{
				return pMesh;
			}
		}
		return nullptr;
	}

	void FbxObject::PreProcess(FbxNode* pNode)
	{
		if (pNode && (pNode->GetCamera() || pNode->GetLight())) 
		{ 
			return;
		}

		m_pFbxNodeList.push_back(pNode);

		int iNumFbxMaterial = pNode->GetMaterialCount();
		FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);

		if (GetRootMaterial(pFbxMaterial) == -1)
		{
			if (iNumFbxMaterial > 1)
			{
				Material* pMaterial = new Material(pNode, pFbxMaterial);
				for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
				{
					FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
					_ASSERT(pFbxSubMaterial != nullptr);
					Material* pSubMaterial = new Material(pNode, pFbxSubMaterial);
					pMaterial->pSubMaterial.push_back(pSubMaterial);
				}
				m_pFbxMaterialList.push_back(pMaterial);
			}
			else
			{
				if (pFbxMaterial != nullptr)
				{
					Material* pMaterial = new Material(pNode, pFbxMaterial);
					m_pFbxMaterialList.push_back(pMaterial);
				}
			}
		}

		int iNumChild = pNode->GetChildCount();
		for (int iNode = 0; iNode < iNumChild; iNode++)
		{
			FbxNode* pChildNode = pNode->GetChild(iNode);
			PreProcess(pChildNode);
		}
	}

	void FbxObject::ParseNode(FbxNode* pNode, Mesh* pParentMesh)
	{
		if (pNode->GetCamera() || pNode->GetLight())
		{
			return;
		}

		Mesh* pMesh = new Mesh();
		TMatrix matParent;

		pMesh->m_pFbxNode = pNode;
		pMesh->m_szName = mtw(pNode->GetName());

		if (pParentMesh != nullptr)
		{
			pMesh->m_szParentName = pParentMesh->m_szName;
			matParent = pParentMesh->m_matWorld;
		}
		pMesh->m_pParent = pParentMesh;
		pMesh->m_matWorld = ParseTransform(pNode, matParent);

		if (pNode->GetMesh())
		{
			ParseMesh(pNode, pMesh);
			pMesh->m_ClassType = OBJECT_CLASS_TYPE::CLASS_GEOM;
		}
		else
		{
			pMesh->m_ClassType = OBJECT_CLASS_TYPE::CLASS_BONE;
		}
		m_pFbxMeshList.push_back(pMesh);

		int iNumChild = pNode->GetChildCount();
		for (int iNode = 0; iNode < iNumChild; iNode++)
		{
			FbxNode* pChildNode = pNode->GetChild(iNode);
			ParseNode(pChildNode, pMesh);
		}
	}

	void FbxObject::ParseMesh(FbxNode* pNode, Mesh* pMesh)
	{
		FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
		pMesh->m_iMaterialRef = GetRootMaterial(pFbxMaterial);

		string name = pNode->GetName();
		FbxMesh* pFbxMesh = pNode->GetMesh();
		vector<string> fbxFileTexureList;

		if (pFbxMesh != nullptr)
		{
			int iNumControlPoint = pFbxMesh->GetControlPointsCount();

			// 스키닝 오브젝트 여부
			// pFbxMesh에 영향을 미치는 행렬(노드)의 전체 개수
			// If 행렬 [0] = FbxNode ~ 행렬[3] = 4개
			//    정점 [0] -> 인덱스[N] (Max N = 4)
			SkinData skinData;
			bool bSkinnedMesh = ParseMeshSkinning(pFbxMesh, pMesh, &skinData);

			if (bSkinnedMesh == true)
			{
				_ASSERT(skinData.m_VertexList.size() == iNumControlPoint);
				for (int outerIndex = 0; outerIndex < skinData.m_VertexList.size(); outerIndex++)
				{
					vector<pair<float, int>> list;
					for (int innerIndex = 0; innerIndex < skinData.m_VertexList[outerIndex].m_IndexList.size(); innerIndex++)
					{
						list.push_back(make_pair(
							skinData.m_VertexList[outerIndex].m_WeightList[innerIndex],
							skinData.m_VertexList[outerIndex].m_IndexList[innerIndex]
						));
					}

					sort(list.begin(), list.end(), Compare);

					for (int innerIndex = 0; innerIndex < list.size(); innerIndex++)
					{
						skinData.m_VertexList[outerIndex].m_WeightList[innerIndex] = list[innerIndex].first;
						skinData.m_VertexList[outerIndex].m_IndexList[innerIndex] = list[innerIndex].second;
					}
				}
			}

			pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
			pMesh->m_LayerList.resize(pMesh->m_iNumLayer);

			for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
			{
				FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
				if (pLayer->GetVertexColors() != nullptr)
				{
					pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
				}
				if (pLayer->GetNormals() != nullptr)
				{
					pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
				}
				if (pLayer->GetUVs() != nullptr)
				{
					pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
				}
				if (pLayer->GetMaterials() != nullptr)
				{
					pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
				}
			}

			FbxAMatrix matGeometry;
			FbxVector4 rotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 translation = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matGeometry.SetT(translation);
			matGeometry.SetR(rotation);
			matGeometry.SetS(scale);

			int iNumPolygon = pFbxMesh->GetPolygonCount();
			FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
			int iBasePolyIndex = 0;
			int iNumFbxMaterial = pNode->GetMaterialCount();
			if (iNumFbxMaterial > 1)
			{
				pMesh->m_pSubMesh.resize(iNumFbxMaterial);
				for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
				{
					pMesh->m_pSubMesh[iSub] = new Mesh();
				}
			}

			FbxLayerElementMaterial* fbxSubMaterial = pMesh->m_LayerList[0].pMaterial;
			FbxLayerElementUV* VertexUVList = pMesh->m_LayerList[0].pUV;
			FbxLayerElementVertexColor* VertexColorList = pMesh->m_LayerList[0].pColor;
			FbxLayerElementNormal* VertexNormalList = pMesh->m_LayerList[0].pNormal;

			for (int iPoly = 0; iPoly < iNumPolygon; iPoly++)
			{
				int iSubMaterialIndex = 0;
				if (fbxSubMaterial != nullptr)
				{
					switch (fbxSubMaterial->GetMappingMode())
					{
						case FbxLayerElement::eByPolygon:
						{
							switch (fbxSubMaterial->GetReferenceMode())
							{
							case FbxLayerElement::eDirect:
							{
								iSubMaterialIndex = iPoly;
								break;
							}
							case FbxLayerElement::eIndex:
							case FbxLayerElement::eIndexToDirect:
							{
								iSubMaterialIndex = fbxSubMaterial->GetIndexArray().GetAt(iPoly);
								break;
							}
							}
						break;
						}
						default:
						{
							iSubMaterialIndex = 0;
							break;
						}
					}
				}

				// 삼각형 또는 사각형
				int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
				int iNumTriangle = iPolySize - 2;
				int iVertexIndex[3];
				for (int iTriangle = 0; iTriangle < iNumTriangle; iTriangle++)
				{
					// 위치 인덱스
					iVertexIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
					iVertexIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
					iVertexIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);

					for (int iIndex = 0; iIndex < 3; iIndex++)
					{
						PNCT_VERTEX vertex;
						PNCTIW_VERTEX iwVertex;
						FbxVector4 pos = pVertexPositions[iVertexIndex[iIndex]];
						FbxVector4 vPos = matGeometry.MultT(pos);

						ZeroMemory(&iwVertex, sizeof(PNCTIW_VERTEX));

						vertex.p.x = vPos.mData[0];
						vertex.p.y = vPos.mData[2];
						vertex.p.z = vPos.mData[1];

						if (VertexUVList != nullptr)
						{
							// UV 인덱스(3DMax는왼쪽 하단이 원점)
							int uvIndex[3];
							uvIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
							uvIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
							uvIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);
							FbxVector2 uv = ReadTextureCoord(
								pFbxMesh, 1, VertexUVList,
								iVertexIndex[iIndex], uvIndex[iIndex]);
							vertex.t.x = uv.mData[0];
							vertex.t.y = 1.0f - uv.mData[1];
						}
						if (VertexColorList != nullptr)
						{
							int iColorIndex[3];
							iColorIndex[0] = iBasePolyIndex + 0;
							iColorIndex[1] = iBasePolyIndex + iTriangle + 2;
							iColorIndex[2] = iBasePolyIndex + iTriangle + 1;

							FbxColor color = ReadColor(
								pFbxMesh, 1, VertexColorList,
								iVertexIndex[iIndex], iColorIndex[iIndex]);
							vertex.c.x = color.mRed;
							vertex.c.y = color.mGreen;
							vertex.c.z = color.mBlue;
							vertex.c.w = 1.0f;
						}
						else // 임시
						{
							vertex.c.x = 1.0f;
							vertex.c.y = 1.0f;
							vertex.c.z = 1.0f;
							vertex.c.w = 1.0f;
						}
						if (VertexNormalList != nullptr)
						{
							int iNormalIndex[3];
							iNormalIndex[0] = iBasePolyIndex + 0;
							iNormalIndex[1] = iBasePolyIndex + iTriangle + 2;
							iNormalIndex[2] = iBasePolyIndex + iTriangle + 1;
							FbxVector4 normal = ReadNormal(
								pFbxMesh, 1, VertexNormalList,
								iVertexIndex[iIndex], iNormalIndex[iIndex]);
							vertex.n.x = normal.mData[0];
							vertex.n.y = normal.mData[2];
							vertex.n.z = normal.mData[1];
						}
						// 인덱스 및 가중치 저장
						int iRealIndex = iVertexIndex[iIndex];
						if (bSkinnedMesh)
						{
							int iNum = skinData.m_VertexList[iRealIndex].m_IndexList.size();
							for (int count = 0; count < min(iNum, 4); count++)
							{
								iwVertex.index[count] = skinData.m_VertexList[iRealIndex].m_IndexList[count];
								iwVertex.weight[count] = skinData.m_VertexList[iRealIndex].m_WeightList[count];
							}
						}
						// 비스키닝 오브젝트 -> 스키닝 오브젝트
						else
						{
							iwVertex.index[0] = 0;
							iwVertex.weight[0] = 1.0f;
							for (int i = 1; i < 4; i++)
							{
								iwVertex.index[i] = 0;
								iwVertex.weight[i] = 0.0f;
							}
						}

						if (iNumFbxMaterial > 1)
						{
							pMesh->m_pSubMesh[iSubMaterialIndex]->m_VertexList.push_back(vertex);
							pMesh->m_pSubMesh[iSubMaterialIndex]->m_WeightList.push_back(iwVertex);
						}
						else
						{
							pMesh->m_VertexList.push_back(vertex);
							pMesh->m_WeightList.push_back(iwVertex);
						}
					}
				}
				iBasePolyIndex += iPolySize;
			}
		}
	}

	TMatrix FbxObject::ParseTransform(FbxNode* pNode, TMatrix& matParent)
	{
		FbxVector4 rotationLocal = pNode->LclRotation.Get();
		FbxVector4 translationLocal = pNode->LclTranslation.Get();
		FbxVector4 scaleLocal = pNode->LclScaling.Get();
		FbxMatrix matTransform(translationLocal, rotationLocal, scaleLocal);
		TMatrix matLocal = DxConvertMatrix(ConvertMatrix(matTransform));
		TMatrix matWorld = matLocal * matParent;
		return matWorld;
	}

	void FbxObject::SetMatrix(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj)
	{
		if (pMatWorld != nullptr)
		{
			m_cbData.matWorld = *pMatWorld;
		}
		if (pMatView != nullptr)
		{
			m_cbData.matView = *pMatView;
		}
		if (pMatProj != nullptr)
		{
			m_cbData.matProj = *pMatProj;
		}
		m_cbData.matNormal = m_cbData.matNormal.Transpose();
	}

	void FbxObject::SetPosition(TVector3 vPos)
	{
		m_matWorld._41 = vPos.x;
		m_matWorld._42 = vPos.y;
		m_matWorld._43 = vPos.z;
	}

	void FbxObject::SetDirection(TVector3 vDir)
	{
		m_matWorld._13 = vDir.x;
		m_matWorld._23 = vDir.y;
		m_matWorld._33 = vDir.z;
	}

	void  FbxObject::SetPixelShader(ID3D11PixelShader* ps)
	{
		for (int iMesh = 0; iMesh < m_pFbxMeshList.size(); iMesh++)
		{
			Mesh* pMesh = m_pFbxMeshList[iMesh];
			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (int iSubMesh = 0; iSubMesh < m_pFbxMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
				{
					Mesh* pSubMesh = m_pFbxMeshList[iMesh]->m_pSubMesh[iSubMesh];
					if (ps == nullptr)
					{
						pSubMesh->m_pMainPS = pSubMesh->m_pPS;
					}
					else
					{
						pSubMesh->m_pMainPS = ps;
					}
				}
			}
			else
			{
				if (ps == nullptr)
				{
					pMesh->m_pMainPS = pMesh->m_pPS;
				}
				else
				{
					pMesh->m_pMainPS = ps;
				}
			}
		}
	}

	bool FbxObject::Frame()
	{
		m_fElapseTime += g_fSecPerFrame * 1.0f;
		m_iAnimIndex = m_fElapseTime * 30.0f;
		if (m_fEndTime < m_fElapseTime)
		{
			m_iAnimIndex = 0;
			m_fElapseTime = 0;
		}

		return true;
	}

	bool FbxObject::Render(ID3D11DeviceContext* pContext)
	{
		for (int iObject = 0; iObject < m_pFbxMeshList.size(); iObject++)
		{
			Mesh* pMesh = m_pFbxMeshList[iObject];
			if (pMesh->m_ClassType != OBJECT_CLASS_TYPE::CLASS_GEOM)
			{
				continue;
			}
			TMatrix matScale;
			D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 0.25f);
			D3DXMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[0], &pMesh->m_AnimationTrack[m_iAnimIndex]);

			for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
			{
				TMatrix matAnim = pMesh->m_matBindPoseList[iBone] * pMesh->m_pMeshLinkList[iBone]->m_AnimationTrack[m_iAnimIndex];// *matScale;
				D3DXMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[iBone], &matAnim);
			}

			Material* pMaterial = nullptr;
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
				{
					if (pMesh->m_pSubMesh[iSub]->m_VertexList.size() <= 0)
					{
						continue;
					}
					Material* pSubMaterial = m_pFbxMaterialList[pMesh->m_iMaterialRef]->pSubMaterial[iSub];
					if (pSubMaterial->texture.m_pTextureSRV.empty() == false)
					{
						pContext->PSSetShaderResources(0, 1, &pSubMaterial->texture.m_pTextureSRV.at(0));
					}
					// 스키닝도 서브메터리얼 사용 가능
					D3DXMatrixTranspose(&pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[0], &pMesh->m_AnimationTrack[m_iAnimIndex]);
					for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
					{
						pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[iBone] = pMesh->m_matAnimMatrix.matAnimation[iBone];// *matScale;
					}

					pMesh->m_pSubMesh[iSub]->SetMatrix(&m_matWorld, &m_cbData.matView, &m_cbData.matProj);
					pMesh->m_pSubMesh[iSub]->Render(pContext);

					// 바운딩박스
					if (pMesh->m_pSubMesh[iSub]->m_matBindPoseList.empty())
					{
						pMesh->m_pSubMesh[iSub]->m_pShape->SetMatrix(&pMesh->m_AnimationTrack[m_iAnimIndex], &m_cbData.matView, &m_cbData.matProj);
					}
					else
					{
						TMatrix matAnim = pMesh->m_matBindPoseList[0] * pMesh->m_pMeshLinkList[0]->m_AnimationTrack[m_iAnimIndex];// *matScale;
						pMesh->m_pShape->SetMatrix(&matAnim, &m_cbData.matView, &m_cbData.matProj);
					}
					pMesh->m_pSubMesh[iSub]->m_pShape->Render(pContext);
				}
			}
			else
			{
				if (pMesh->m_iMaterialRef >= 0)
				{
					pMaterial = m_pFbxMaterialList[pMesh->m_iMaterialRef];
				}
				if (pMaterial != nullptr)
				{
					pContext->PSSetShaderResources(0, 1, &pMaterial->texture.m_pTextureSRV.at(0));
				}
				pMesh->SetMatrix(&m_matWorld, &m_cbData.matView, &m_cbData.matProj);
				pMesh->Render(pContext);

				// 바운딩박스
				if (pMesh->m_matBindPoseList.empty())
				{
					pMesh->m_pShape->SetMatrix(&pMesh->m_AnimationTrack[m_iAnimIndex], &m_cbData.matView, &m_cbData.matProj);
				}
				else
				{
					TMatrix matAnim = pMesh->m_matBindPoseList[0] * pMesh->m_pMeshLinkList[0]->m_AnimationTrack[m_iAnimIndex];// *matScale;
					pMesh->m_pShape->SetMatrix(&matAnim, &m_cbData.matView, &m_cbData.matProj);
				}
				pMesh->m_pShape->Render(pContext);
			}
		}
		return true;
	}

	bool FbxObject::Release()
	{
		for (int iObj = 0; iObj < m_pFbxMeshList.size(); iObj++)
		{
			m_pFbxMeshList[iObj]->Release();
			SAFE_DEL(m_pFbxMeshList[iObj]);
		}
		for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
		{
			m_pFbxMaterialList[iObj]->Release();
			SAFE_DEL(m_pFbxMaterialList[iObj]);
		}
		return true;
	}

	FbxObject::FbxObject()
	{
		SAFE_ZERO(m_pFbxManager);
		SAFE_ZERO(m_pFbxImporter);
		SAFE_ZERO(m_pFbxScene);
		m_fStartTime = 0.0f;
		m_fEndTime = 0.0f;
		m_fSamplingTime = 0.0f;
		m_fElapseTime = 0.0f;
		m_bAnimPlay = false;
		m_iAnimIndex = 0;
	}

	FbxObject::~FbxObject()
	{

	}
}