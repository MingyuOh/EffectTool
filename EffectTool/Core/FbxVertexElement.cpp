#include "FbxObject.h"

namespace DXCore
{
	FbxVector2 FbxObject::ReadTextureCoord(FbxMesh* pFbxMesh, 
		DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, 
		int vertexIndex, int uvIndex)
	{
		FbxVector2 uv(0, 0);
		if (dwVertexTextureCount <= 0 || pUVSet == nullptr)
		{
			return uv;
		}
		int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
		FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

		// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점
		// 텍스처 좌표와 같은 레이어들은 제어점 또는 정점으로 구분
		switch (pUVSet->GetMappingMode())
		{
			// 제어점 당 1개의 텍스처 좌표
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pUVSet->GetReferenceMode())
			{
				// 배열에서 직접 Get
			case FbxLayerElementUV::eDirect:
			{
				FbxVector2 fbxUV = pUVSet->GetDirectArray().GetAt(vertexIndex);
				uv.mData[0] = fbxUV.mData[0];
				uv.mData[1] = fbxUV.mData[1];
				break;
			}
			// 배열에 해당당하는 인덱스를 통해 Get
			case FbxLayerElement::eIndexToDirect:
			{
				int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
				FbxVector2 fbxUV = pUVSet->GetDirectArray().GetAt(id);
				uv.mData[0] = fbxUV.mData[0];
				uv.mData[1] = fbxUV.mData[1];
				break;
			}
			}
			break;
		}
		// 정점 당 1개의 매핑 좌표
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pUVSet->GetReferenceMode())
			{
			case FbxLayerElementUV::eDirect:
			case FbxLayerElementUV::eIndexToDirect:
			{
				uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
				uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
				break;
			}
			}
			break;
		}
		}
		return uv;
	}

	FbxVector4 FbxObject::ReadNormal(const FbxMesh* mesh, 
		DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets, 
		int controlPointIndex, int iVertexIndex)
	{
		FbxVector4 result(0, 0, 0);
		if (dwVertexNormalCount < 1)
		{
			return result;
		}
		int iVertexNormalLayer = mesh->GetElementNormalCount();

		// 노말 획득
		const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

		// 노말 벡터를 저장할 벡터
		switch (VertexNormalSets->GetMappingMode())
		{
		// 제어점 마다 1개의 매핑 좌표가 있다.
			case FbxGeometryElement::eByControlPoint:
			{
				// 제어점 맵핑
				switch (VertexNormalSets->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[0]);
						result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[2]);
						result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[1]);
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int index = VertexNormalSets->GetIndexArray().GetAt(controlPointIndex);
						result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
						result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
						result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
						break;
					}
				}
				break;
			}
			// 정점 마다 1개의 매핑 좌표가 있다.
			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (vertexNormal->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[0]);
						result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[2]);
						result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[1]);
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int index = VertexNormalSets->GetIndexArray().GetAt(iVertexIndex);
						result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
						result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
						result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
						break;
					}
				}
				break;
			}
		}
		return result;
	}

	FbxColor FbxObject::ReadColor(const FbxMesh* mesh, 
		DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex)
	{
		FbxColor value(1, 1, 1, 1);
		if (dwVertexColorCount < 1)
		{
			return value;
		}
		int iVertexColorCountLayer = mesh->GetElementVertexColorCount();
		const FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor(0);

		if (dwVertexColorCount > 0 && VertexColorSets != nullptr)
		{
			switch (VertexColorSets->GetMappingMode())
			{
				case FbxGeometryElement::eByControlPoint:
				{
					switch (VertexColorSets->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
						{
							value = VertexColorSets->GetDirectArray().GetAt(dwDCCIndex);
							break;
						}
						case FbxGeometryElement::eIndexToDirect:
						{
							int index = VertexColorSets->GetIndexArray().GetAt(dwDCCIndex);
							value = VertexColorSets->GetDirectArray().GetAt(index);
							break;
						}
					}
				break;
				}
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (VertexColorSets->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
						{
							value = VertexColorSets->GetDirectArray().GetAt(dwVertexIndex);
							break;
						}
						case FbxGeometryElement::eIndexToDirect:
						{
							int index = VertexColorSets->GetIndexArray().GetAt(dwVertexIndex);
							value = VertexColorSets->GetDirectArray().GetAt(index);
							break;
						}
					}
					break;
				}
			}
		}
		return value;
	}
}