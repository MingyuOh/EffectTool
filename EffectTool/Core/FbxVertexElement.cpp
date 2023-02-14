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

		// �������� ����� 4�� ����, ������ ������ 6�� ����
		// �ؽ�ó ��ǥ�� ���� ���̾���� ������ �Ǵ� �������� ����
		switch (pUVSet->GetMappingMode())
		{
			// ������ �� 1���� �ؽ�ó ��ǥ
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pUVSet->GetReferenceMode())
			{
				// �迭���� ���� Get
			case FbxLayerElementUV::eDirect:
			{
				FbxVector2 fbxUV = pUVSet->GetDirectArray().GetAt(vertexIndex);
				uv.mData[0] = fbxUV.mData[0];
				uv.mData[1] = fbxUV.mData[1];
				break;
			}
			// �迭�� �ش���ϴ� �ε����� ���� Get
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
		// ���� �� 1���� ���� ��ǥ
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

		// �븻 ȹ��
		const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

		// �븻 ���͸� ������ ����
		switch (VertexNormalSets->GetMappingMode())
		{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
			case FbxGeometryElement::eByControlPoint:
			{
				// ������ ����
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
			// ���� ���� 1���� ���� ��ǥ�� �ִ�.
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