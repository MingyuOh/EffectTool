#include "AseObject.h"
//#include "AtlConv.h"

//--------------------------------------------------------------------------------------
// 섹션 enum
//--------------------------------------------------------------------------------------
enum SECTION
{
	SECTION_ASE = 0,
	SECTION_COMMENT,
	SECTION_SCENE,
	SECTION_MATERIAL
};

//--------------------------------------------------------------------------------------
// Geometry Object enum
//--------------------------------------------------------------------------------------
enum GeomObject
{
	GEOM_NAME = 0,
	GEOM_PARENT,
	HELPER_CLASS,
	GEOM_TM,
	GEOM_MESH,
	BOUNDINGBOX_MIN // 최소랑 최대랑 같이 찾는다.
};

//--------------------------------------------------------------------------------------
// Material enum
//--------------------------------------------------------------------------------------
enum MaterialObject
{
	MATERIAL = 0,
	NUMSUBMTLS,
	SUBMATERIAL,
	MAP_SUBNO,
	ASE_BITMAP,
	GEOMOBJECT,
	HELPEROBJECT
};

//--------------------------------------------------------------------------------------
// 메쉬 토큰 ENUM
//--------------------------------------------------------------------------------------
enum MeshTokens
{
	MESH_NUMVERTEX = 0,
	MESH_VERTEX_LIST,
	MESH_NUMFACES,
	MESH_FACE_LIST,
	MESH_NUMTVERTEX,
	MESH_TVERTLIST,
	MESH_NUMTVFACES,
	MESH_TFACELIST,
	MESH_NUMCVERTEX,
	MESH_CVERTLIST,
	MESH_NUMCVFACES,
	MESH_CFACELIST,
	MESH_NORMAL,
	TM_ANIMATION,
	MESH_MATERIAL_REF,			// 종료
	MESH_WIREFRAME_COLOR		// 종료
};

//--------------------------------------------------------------------------------------
// Material enum
//--------------------------------------------------------------------------------------
enum AnimationObject
{
	CONTROL_POS_SAMPLE = 0,
	CONTROL_ROT_SAMPLE,
	CONTROL_SRC_SAMPLE,
	MATERIAL_REF,
	WIREFRAME_COLOR,
	GEOM_OBJECT,
	HELPER_OBJECT
};

static T_STR aseSectionHeaderToken[] =
{
	_T("*3DSMAX_ASCIIEXPORT"),
	_T("*COMMENT"),
	_T("*SCENE"),
	_T("*MATERIAL_LIST")
};

static T_STR aseGameObjectTokens[] =
{
	_T("*NODE_NAME"),
	_T("*NODE_PARENT"),
	_T("*HELPER_CLASS"),
	_T("*NODE_TM"),
	_T("*MESH"),
	_T("*BOUNDINGBOX_MIN"),
	_T("*GEOMOBJECT")
};

static T_STR aseMaterialTokens[] =
{
	_T("*MATERIAL"),
	_T("*NUMSUBMTLS"),
	_T("*SUBMATERIAL"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),
	_T("*HELPEROBJECT")
};

static T_STR aseMeshTokens[] =
{
	_T("*MESH_NUMVERTEX"),
	_T("*MESH_VERTEX_LIST"),
	_T("*MESH_NUMFACES"),
	_T("*MESH_FACE_LIST"),
	_T("*MESH_NUMTVERTEX"),
	_T("*MESH_TVERTLIST"),
	_T("*MESH_NUMTVFACES"),
	_T("*MESH_TFACELIST"),
	_T("*MESH_NUMCVERTEX"),
	_T("*MESH_CVERTLIST"),
	_T("*MESH_NUMCVFACES"),
	_T("*MESH_CFACELIST"),
	_T("*MESH_NORMALS"),
	_T("*TM_ANIMATION"),
	_T("*MATERIAL_REF"),		// 종료
	_T("*WIREFRAME_COLOR")		// 종료
};

static T_STR aseAnimationTokens[] =
{
	_T("*CONTROL_POS_TRACK"),
	_T("*CONTROL_ROT_TRACK"),
	_T("*CONTROL_SCALE_TRACK"),
	_T("*MATERIAL_REF"),		// 종료
	_T("*WIREFRAME_COLOR"),		// 종료
	_T("*GEOMOBJECT"),
	_T("*HELPEROBJECT")
};

//--------------------------------------------------------------------------------------
// 파일 로드
//--------------------------------------------------------------------------------------
bool AseObject::Load(T_STR szFileName)
{
	m_pStream = _tfopen(szFileName.c_str(), _T("rt"));
	if (m_pStream == NULL)
	{
		return false;
	}

	bool bLoad = true;
	while (bLoad)
	{
		int iNumHead = sizeof(aseSectionHeaderToken) / sizeof(aseSectionHeaderToken[0]);
		int iSection = GetSection(aseSectionHeaderToken, iNumHead);
		switch (iSection)
		{
		case SECTION_ASE:
		{
			Load3DMax();
		}break;
		case SECTION_COMMENT:
		{
			LoadComment();
		}break;
		case SECTION_SCENE:
		{
			LoadScene();
		}break;
		case SECTION_MATERIAL:
		{
			bLoad = LoadMaterialList();
		}break;
		default:
		{
			break;
		}
		}
	}

	while (!feof(m_pStream))
	{
		m_iNumObject++;
		m_GeomObjectList.resize(m_iNumObject);
		m_GeomObjectList[m_iNumObject - 1].Init();
		if (!LoadObject())
		{
			while (!feof(m_pStream))
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s"), m_pString);
				if (!_tcsicmp(m_pString, L"*GEOMOBJECT"))
				{
					break;
				}
				if (!_tcsicmp(m_pString, L"*HELPEROBJECT"))
				{
					break;
				}
			}
		}
		// 지오메트리 찾으면 계속돌게끔하면되고 레퍼런스를 찾으면 끝내야한다.
		/*if (!_tcsicmp(m_pString, L"*MATERIAL_REF"))
		break;*/
	}
	fclose(m_pStream);

	TMatrix matWorld;
	TMatrix matInvParentWorld = TMatrix::Identity;
	TQuaternion qRotation;
	for (int iObj = 0; iObj < m_GeomObjectList.size(); iObj++)
	{
		matWorld = m_GeomObjectList[iObj].m_matWorld;
		if (!m_GeomObjectList[iObj].m_Object.m_szParentName.empty())
		{
			// 부모 검색
			m_GeomObjectList[iObj].m_iParentID = SearchToObject(m_GeomObjectList[iObj].m_Object.m_szParentName);
			m_GeomObjectList[iObj].m_pParentObj = &m_GeomObjectList[m_GeomObjectList[iObj].m_iParentID];

			// 부모 역행렬
			matInvParentWorld = m_GeomObjectList[m_GeomObjectList[iObj].m_iParentID].m_matWorld;
			D3DXMatrixInverse(&matInvParentWorld, NULL, &matInvParentWorld);
		}
		matWorld = matWorld * matInvParentWorld;

		TVector3 vScale, vPos;
		D3DXMatrixDecompose(&vScale, &qRotation, &vPos, &matWorld);
		D3DXMatrixScaling(&m_GeomObjectList[iObj].m_Object.m_matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixTranslation(&m_GeomObjectList[iObj].m_Object.m_matTrans, vPos.x, vPos.y, vPos.z);
		D3DXMatrixRotationQuaternion(&m_GeomObjectList[iObj].m_Object.m_matRotation, &qRotation);

		m_GeomObjectList[iObj].m_Object.m_matCaculation = matWorld;
	}

	return true;
}
//--------------------------------------------------------------------------------------
// 섹션에 따른 파일 라인 로드
//--------------------------------------------------------------------------------------
bool AseObject::Load3DMax()
{
	// 버전읽고싶다면
	// int iVersion;
	// _stscanf(m_pBuffer, _T("%s %d"), m_pString, iVersion);
	return true;
}

bool AseObject::LoadComment()
{
	return true;
}

bool AseObject::LoadScene()
{
	_fgetts(m_pBuffer, 256, m_pStream);

	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_Scene.iFristFrame);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_Scene.iLastFrame);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_Scene.iFrameSpeed);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_Scene.iTickPerFrame);

	return true;
}

bool AseObject::LoadMaterialList()
{
	bool bLoad = true;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_iNumMaterial);
	if (m_iNumMaterial != 0)
	{
		bLoad = LoadMaterial();
		return bLoad;
	}
	bLoad = false;
	return bLoad;
}

bool AseObject::LoadMaterial()
{
	GeometryMaterial* pCurrentMtrl = 0;
	int iSubIndex;
	int iNumSection = sizeof(aseMaterialTokens) / sizeof(aseMaterialTokens[0]);
	bool bMaterialRead = true;
	while (bMaterialRead)
	{
		int iSection = GetSection(aseMaterialTokens, iNumSection);
		switch (iSection)
		{
		case MATERIAL:
		{
			GeometryMaterial material;
			m_MaterialList.push_back(material);
		}break;
		case NUMSUBMTLS:
		{
			GeometryMaterial& material = m_MaterialList[m_MaterialList.size() - 1];
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_iNumSubMaterial);
			material.subMaterial.resize(m_iNumSubMaterial);
		}break;
		case SUBMATERIAL:
		{
			pCurrentMtrl = &m_MaterialList[m_MaterialList.size() - 1];
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &iSubIndex);
			pCurrentMtrl = &pCurrentMtrl->subMaterial[iSubIndex];
		}break;
		case MAP_SUBNO:
		{
			GeometryMaterial& material = m_MaterialList[m_MaterialList.size() - 1];
			TexMap map;
			//pCurrentMtrl = &m_MaterialList[m_MaterialList.size() - 1];
			if (pCurrentMtrl != NULL)
			{
				_stscanf(m_pBuffer, _T("%s%d"), m_pString, &map.iType);
				pCurrentMtrl->m_texMap.push_back(map);
			}
			else
			{
				// 루트
				map.iType = 1;

				material.m_texMap.push_back(map);
			}
		}break;
		case ASE_BITMAP:
		{
			TCHAR name[256] = { 0, };
			GeometryMaterial& material = m_MaterialList[m_MaterialList.size() - 1];
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, &name);

			TCHAR drive[256] = { 0, };
			TCHAR dir[256] = { 0, };
			TCHAR filename[256] = { 0, };
			TCHAR ext[256] = { 0, };

			TCHAR path[256] = L"../data/Texture/";
			_tsplitpath(name, drive, dir, filename, ext);

			if (pCurrentMtrl != NULL)
			{
				int iCnt = pCurrentMtrl->m_texMap.size() - 1;
				pCurrentMtrl->m_texMap[iCnt].name = path;
				pCurrentMtrl->m_texMap[iCnt].name += filename;
				ext[4] = 0;

				if (!_tcscmp(ext, L".tga"))
				{
					_tcscpy(ext, L".dds");
				}
				pCurrentMtrl->m_texMap[iCnt].name += ext;
			}
			else
			{
				TexMap map;
				map.name = path;
				map.name += filename;
				ext[4] = 0;
				if (!_tcscmp(ext, L".tga"))
				{
					_tcscpy(ext, L".dds");
				}
				map.name += ext;
				material.m_texMap.push_back(map);
			}
		}break;
		case GEOMOBJECT:
		{
			bMaterialRead = false;
		}break;
		case HELPEROBJECT:
		{
			bMaterialRead = false;
		}break;
		default:
		{
			break;
		}
		}
	}
	return bMaterialRead;
}

bool AseObject::LoadMesh()
{
	int iNumSection = sizeof(aseMeshTokens) / sizeof(aseMeshTokens[0]);
	int iVertexIndex;
	int iFaceIndex;
	bool bLoad = true;
	while (bLoad)
	{
		int iSection = GetSection(aseMeshTokens, iNumSection);
		switch (iSection)
		{
		case MESH_NUMVERTEX:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumVertex);
		}break;
		case MESH_VERTEX_LIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_VertexArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumVertex);
			for (int iVer = 0; iVer < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumVertex; iVer++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
					m_pString, &iVertexIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_VertexArray[iVer].x,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_VertexArray[iVer].z,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_VertexArray[iVer].y);
			}
		}break;
		case MESH_NUMFACES:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumFace);
		}break;
		case MESH_FACE_LIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumFace);
			for (int iFace = 0; iFace < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumFace; iFace++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%s %s%d %s%d %s%d %s%d %s%d %s%d"),
					m_pString, m_pString,
					m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iIndex[0], // A
					m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iIndex[2], // B
					m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iIndex[1], // C
					m_pString, &iFaceIndex,																  // AB
					m_pString, &iFaceIndex,																  // BC
					m_pString, &iFaceIndex);															  // CA
				T_STR pBuffer = _tcsstr(m_pBuffer, L"*MESH_MTLID");
				_stscanf(pBuffer.c_str(), _T("%s %d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iSubMtrl); // *MESH_MTLID 
				if (m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iSubMtrl >= 255)
				{
					m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceArray[iFace].iSubMtrl = 4;
				}
			}
		}break;
		case MESH_NUMTVERTEX:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexVertex);
		}break;
		case MESH_TVERTLIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexVertexArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexVertex);
			for (int iTex = 0; iTex < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexVertex; iTex++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f"),
					m_pString, &iVertexIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexVertexArray[iTex].x,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexVertexArray[iTex].y);
				m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexVertexArray[iTex].y = 1.0f - m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexVertexArray[iTex].y;
			}
		}break;
		case MESH_NUMTVFACES:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexFace);
		}break;
		case MESH_TFACELIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexFaceArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexFace);
			for (int iTFace = 0; iTFace < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumTexFace; iTFace++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%d%d%d"),
					m_pString, &iFaceIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexFaceArray[iTFace].iIndex[0],
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexFaceArray[iTFace].iIndex[2],
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_TexFaceArray[iTFace].iIndex[1]);
			}
		}break;
		case MESH_NUMCVERTEX:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorVertex);
		}break;
		case MESH_CVERTLIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorVertexArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorVertex);
			for (int iColor = 0; iColor < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorVertex; iColor++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
					m_pString, &iVertexIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorVertexArray[iColor].x,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorVertexArray[iColor].z,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorVertexArray[iColor].y);
			}
		}break;
		case MESH_NUMCVFACES:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorFace);
		}break;
		case MESH_CFACELIST:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorFaceArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorFace);
			for (int iColor = 0; iColor < m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumColorFace; iColor++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%d%d%d"),
					m_pString, &iVertexIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorFaceArray[iColor].iIndex[0],
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorFaceArray[iColor].iIndex[2],
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_ColorFaceArray[iColor].iIndex[1]);
			}
		}break;
		case MESH_NORMAL:
		{
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray.resize(m_GeomObjectList[m_iNumObject - 1].m_Object.m_iNumFace);
			for (int iFace = 0; iFace < m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray.size(); iFace++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
					m_pString, &iFaceIndex,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vFace.x,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vFace.z,
					&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vFace.y);
				int iIndexCount[3];
				iIndexCount[0] = 0; iIndexCount[1] = 2; iIndexCount[2] = 1;

				for (int iVer = 0; iVer < 3; iVer++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer, _T("%s%d%f%f%f"),
						m_pString, &m_iNumVertex,
						&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vVertex[iIndexCount[iVer]].x,
						&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vVertex[iIndexCount[iVer]].z,
						&m_GeomObjectList[m_iNumObject - 1].m_Object.m_FaceNormalArray[iFace].vVertex[iIndexCount[iVer]].y);
				}
			}
		}break;
		case TM_ANIMATION:
		{
			bLoad = LoadAnimation();
		}break;
		case MESH_MATERIAL_REF:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iMaterialRef);
			bLoad = false;
		}break;
		case MESH_WIREFRAME_COLOR:
		{
			bLoad = false;
		}break;
		default:
			break;
		}
	}
	return bLoad;
}
bool AseObject::LoadAnimation()
{
	int iNumSection = sizeof(aseAnimationTokens) / sizeof(aseAnimationTokens[0]);
	m_GeomObjectList[m_iNumObject - 1].m_Object.m_Scene = m_Scene;
	bool bLoad = true;
	while (bLoad)
	{
		fgetpos(m_pStream, &m_FilePosition);
		int iSection = GetSection(aseAnimationTokens, iNumSection);
		switch (iSection)
		{
		case CONTROL_POS_SAMPLE:
		{
			Animtrack vPos;
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%d%f%f%f"), m_pString, &vPos.iTick, &vPos.vPos.x, &vPos.vPos.z, &vPos.vPos.y);
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_PosTrack.push_back(vPos);
			while (1)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f"), m_pString, &vPos.iTick, &vPos.vPos.x, &vPos.vPos.z, &vPos.vPos.y);
				if (_tcscmp(m_pString, L"*CONTROL_POS_SAMPLE"))
				{
					break;
				}
				m_GeomObjectList[m_iNumObject - 1].m_Object.m_PosTrack.push_back(vPos);
			}
		}break;
		case CONTROL_ROT_SAMPLE:
		{
			Animtrack vRot;
			Animtrack accumulateTrack;

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%d%f%f%f%f"), m_pString, &vRot.iTick, &vRot.vRot.x, &vRot.vRot.z, &vRot.vRot.y, &vRot.vRot.w);
			D3DXQuaternionRotationAxis(&vRot.vRot, &TVector3(vRot.vRot.x, vRot.vRot.y, vRot.vRot.z), vRot.vRot.w);
			accumulateTrack = vRot;
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_RotTrack.push_back(vRot);

			while (1)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f%f"), m_pString, &vRot.iTick, &vRot.vRot.x, &vRot.vRot.z, &vRot.vRot.y, &vRot.vRot.w);
				if (_tcscmp(m_pString, L"*CONTROL_ROT_SAMPLE"))
				{
					break;
				}
				D3DXQuaternionRotationAxis(&vRot.vRot, &TVector3(vRot.vRot.x, vRot.vRot.y, vRot.vRot.z), vRot.vRot.w);
				D3DXQuaternionMultiply(&vRot.vRot, &accumulateTrack.vRot, &vRot.vRot);
				accumulateTrack = vRot;
				m_GeomObjectList[m_iNumObject - 1].m_Object.m_RotTrack.push_back(vRot);
			}
		}break;
		case CONTROL_SRC_SAMPLE:
		{
			Animtrack vSrc;
			Animtrack accumulateTrack;

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%d%f%f%f%f%f%f%f"), m_pString, &vSrc.iTick, &vSrc.vPos.x, &vSrc.vPos.z, &vSrc.vPos.y, &vSrc.vRot.x, &vSrc.vRot.z, &vSrc.vRot.y, &vSrc.vRot.w);
			D3DXQuaternionRotationAxis(&vSrc.vRot, &TVector3(vSrc.vRot.x, vSrc.vRot.y, vSrc.vRot.z), vSrc.vRot.w);
			accumulateTrack = vSrc;
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_ScaleTrack.push_back(vSrc);

			while (1)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%f%f%f%f%f%f%f"), m_pString, &vSrc.iTick, &vSrc.vPos.x, &vSrc.vPos.z, &vSrc.vPos.y, &vSrc.vRot.x, &vSrc.vRot.z, &vSrc.vRot.y, &vSrc.vRot.w);
				if (_tcscmp(m_pString, L"*CONTROL_SCALE_SAMPLE"))
				{
					fgetpos(m_pStream, &m_FilePosition);
					break;
				}
				D3DXQuaternionRotationAxis(&vSrc.vRot, &TVector3(vSrc.vRot.x, vSrc.vRot.y, vSrc.vRot.z), vSrc.vRot.w);
				D3DXQuaternionMultiply(&vSrc.vRot, &accumulateTrack.vRot, &vSrc.vRot);
				accumulateTrack = vSrc;
				m_GeomObjectList[m_iNumObject - 1].m_Object.m_ScaleTrack.push_back(vSrc);
			}
		}break;
		case MATERIAL_REF:
		{
			_stscanf(m_pBuffer, _T("%s%d"), m_pString, &m_GeomObjectList[m_iNumObject - 1].m_Object.m_iMaterialRef);
			bLoad = false;
		}break;
		case WIREFRAME_COLOR:
		{
			//_fgetts(m_pBuffer, 256, m_pStream);
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_iMaterialRef = 0;
			bLoad = false;
		}break;
		case GEOM_OBJECT:
		{
			fsetpos(m_pStream, &m_FilePosition);
			bLoad = false;
		}break;
		case HELPER_OBJECT:
		{
			fsetpos(m_pStream, &m_FilePosition);
			bLoad = false;
		}break;
		default:
		{
			break;
		}
		}
	}
	return bLoad;
}

// 오브젝트도 메터리얼처럼 리스트를 만들어서 한개 더 찾으면 리스트에 추가해야한다. (멀티오브젝트일 경우)
bool AseObject::LoadObject()
{
	int iNumSection = sizeof(aseGameObjectTokens) / sizeof(aseGameObjectTokens[0]);
	bool bLoad = true;

	int len = 0;
	while (bLoad)
	{
		TCHAR name[256] = { 0, };
		TCHAR splitName[256] = { 0, };
		int iSection = GetSection(aseGameObjectTokens, iNumSection);
		switch (iSection)
		{
		case GEOM_NAME:
		{
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, &name);
			len = _tcslen(name);
			wcsncpy(splitName, &name[1], len - 2);
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_szNodeName = splitName;
		}break;
		case GEOM_PARENT:
		{
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, &name);
			len = _tcslen(name);
			wcsncpy(splitName, &name[1], len - 2);
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_szParentName = splitName;
		}break;
		case HELPER_CLASS:
		{
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, &name);
			len = _tcslen(name);
			wcsncpy(splitName, &name[1], len - 2);
			m_GeomObjectList[m_iNumObject - 1].m_Object.m_szClassName = splitName;
		}break;
		case GEOM_TM:
		{
			TQuaternion qRotaion;
			D3DXQuaternionIdentity(&qRotaion);
			TVector3	   vAxis, vVector;
			float		   fAngle;

			_fgetts(m_pBuffer, 256, m_pStream);
			_fgetts(m_pBuffer, 256, m_pStream);
			_fgetts(m_pBuffer, 256, m_pStream);
			_fgetts(m_pBuffer, 256, m_pStream);

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"),
				m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._11,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._13,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._12);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"),
				m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._31,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._33,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._32);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"),
				m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._21,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._23,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._22);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"),
				m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._41,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._43,
				&m_GeomObjectList[m_iNumObject - 1].m_matWorld._42);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString, &vVector.x, &vVector.z, &vVector.y);
			vVector = (vVector == TVector3(0.0f, 0.0f, 0.0f)) ? TVector3(1.0f, 1.0f, 1.0f) : vVector;

			D3DXMatrixTranslation(&m_GeomObjectList[m_iNumObject - 1].m_Object.m_matTrans, vVector.x, vVector.y, vVector.z);

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString, &vAxis.x, &vAxis.z, &vAxis.y);
			vAxis = (vAxis == TVector3(0.0f, 0.0f, 0.0f)) ? TVector3(1.0f, 1.0f, 1.0f) : vAxis;

			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f"), m_pString, &fAngle);
			// 임의의 축과 각을 쿼터니언으로 변환
			D3DXQuaternionRotationAxis(&qRotaion, &vAxis, fAngle);
			D3DXMatrixRotationQuaternion(&m_GeomObjectList[m_iNumObject - 1].m_Object.m_matRotation, &qRotaion);

			//스케일축을 중심으로 회전하는 값이 스테일 값이므로 
			//스케일축의 회전만큼을 먼저 반대로 회전한 이후
			//스케일값을 적용시키고 그 다음에 
			//다시 스케일축만큼을 회전시켜 원상복귀 시킨다.
			TMatrix matScl;
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString, &vVector.x, &vVector.z, &vVector.y);
			vVector = (vVector == TVector3(0.0f, 0.0f, 0.0f)) ? TVector3(1.0f, 1.0f, 1.0f) : vVector;
			D3DXMatrixScaling(&matScl, vVector.x, vVector.y, vVector.z);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString, &vAxis.x, &vAxis.z, &vAxis.y);
			vAxis = (vAxis == TVector3(0.0f, 0.0f, 0.0f)) ? TVector3(1.0f, 1.0f, 1.0f) : vAxis;
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pString, _T("%s%f"), m_pString, &fAngle);

			// 스케일축의 행렬과 그 역행렬을 구한다.
			TMatrix matRotation, matRotationInv;
			D3DXMatrixRotationAxis(&matRotation, &vAxis, fAngle);
			D3DXMatrixInverse(&matRotationInv, NULL, &matRotation);

			m_GeomObjectList[m_iNumObject - 1].m_Object.m_matScale = matRotationInv * matScl * matRotation;
		}break;
		case GEOM_MESH:
		{
			bLoad = LoadMesh();
		}break;
		case BOUNDINGBOX_MIN:
		{
			TCHAR szCompare[256];
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMin.x,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMin.z,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMin.y);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%f%f%f"), m_pString,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMax.x,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMax.z,
				&m_GeomObjectList[m_iNumObject - 1].m_Object.m_vMax.y);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s%s"), m_pString, &szCompare);
			if (!_tcsicmp(m_pString, L"*TM_ANIMATION"))
			{
				bLoad = LoadAnimation();
			}
			else
			{
				bLoad = false;
			}
		}break;
		default:
		{
			break;
		}
		}
	}
	return bLoad;
}

//--------------------------------------------------------------------------------------
// 섹션에 따른 파일 내용 로드
//--------------------------------------------------------------------------------------
int AseObject::GetSection(T_STR* ppObjectType, int iNum)
{
	while (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		for (int iSection = 0; iSection < iNum; iSection++)
		{
			if (!_tcsicmp(m_pString, ppObjectType[iSection].c_str()))
			{
				return iSection;
			}
		}
		return -1;
	}
}

HRESULT	AseObject::CreateLoad(T_STR TextureName, T_STR vertexShaderName, T_STR PixelShaderName, T_STR geometryShaderName)
{
	// 매터리얼이 존재할 경우
	if (m_MaterialList.size() != 0)
	{
		for (int iMat = 0; iMat < m_MaterialList.size(); iMat++)
		{
			for (int iObj = 0; iObj < m_iNumObject; iObj++)
			{
				// 서브매터리얼이 존재할 경우
				if (m_MaterialList[iMat].subMaterial.size() != 0)
				{
					if (m_GeomObjectList[iObj].m_Object.m_VertexArray.size() <= 0)
						continue;

					if (m_GeomObjectList[iObj].m_pTexture == nullptr)
					{
						m_GeomObjectList[iObj].m_pTexture = new Texture();
					}
					// 서브매터리얼 개수만큼 버텍스 / 인덱스버퍼 할당
					m_GeomObjectList[iObj].m_VBArray.resize(m_MaterialList[iMat].subMaterial.size());
					m_GeomObjectList[iObj].m_IBArray.resize(m_MaterialList[iMat].subMaterial.size());
					// 서브매터리얼 리소스뷰 할당
					m_GeomObjectList[iObj].m_pTexture->m_pTextureSRV.reserve(m_MaterialList[iMat].subMaterial.size());
					m_GeomObjectList[iObj].Create(_T(""), vertexShaderName, PixelShaderName, geometryShaderName);
					for (int iSub = 0; iSub < m_MaterialList[iMat].subMaterial.size(); iSub++)
					{
						if (m_MaterialList[iMat].subMaterial[iSub].m_texMap.size() <= 0)
							continue;
						m_GeomObjectList[iObj].LoadTexture(m_MaterialList[iMat].subMaterial[iSub].m_texMap[0].name);
					}
				}
				else // 서브매터리얼이 존재하지 않을 경우
				{
					m_GeomObjectList[iObj].Create(m_MaterialList[iObj].m_texMap[iObj].name, vertexShaderName, PixelShaderName, geometryShaderName);
				}
			}
		}
	}
	else // 매터리얼이 존재하지 않을 경우
	{
		for (int iObj = 0; iObj < m_iNumObject; iObj++)
		{
			m_GeomObjectList[iObj].Create(_T(""), vertexShaderName, PixelShaderName, geometryShaderName);
		}
	}
	return S_OK;
}

HRESULT AseObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	for (int iObj = 0; iObj < m_iNumObject; iObj++)
	{
		for (int iMat = 0; iMat < m_MaterialList.size(); iMat++)
		{
			if (m_MaterialList[iMat].subMaterial.size() > 0)
			{
				m_GeomObjectList[iObj].m_VBArray.resize(m_MaterialList[iMat].subMaterial.size());
			}
			m_GeomObjectList[iObj].CreateVertexBuffer();
		}
	}
	return hr;
}

HRESULT AseObject::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	for (int iObj = 0; iObj < m_iNumObject; iObj++)
	{
		m_GeomObjectList[iObj].CreateConstantBuffer();
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// 보간 함수
//--------------------------------------------------------------------------------------
void AseObject::Interpolate(GeomObj* pGeom, TMatrix* matParents, float fFrameTick)
{
	Animtrack* pStartTrack = NULL;
	Animtrack* pEndTrack = NULL;
	TQuaternion qRotation, qScale;
	TMatrix	matScaleRot;
	TMatrix	matRotationInv;
	float time = 0.0f;

	TMatrix matAnim, matPos, matRotate, matScale;
	matAnim = pGeom->m_Object.m_matCaculation;
	matRotate = pGeom->m_Object.m_matRotation;
	matPos = pGeom->m_Object.m_matTrans;
	matScale = pGeom->m_Object.m_matScale;


	D3DXQuaternionRotationMatrix(&qRotation, &matRotate);
	D3DXQuaternionRotationMatrix(&qScale, &matScale);

	float fStartTick = m_Scene.iFristFrame * m_Scene.iTickPerFrame;
	float fEndTick = 0.0f;

	TVector3 vTrans(matPos._41, matPos._42, matPos._43);
	if (pGeom->m_Object.m_PosTrack.size())
	{
		if (GetAnimationTrack(fFrameTick, &pGeom->m_Object.m_PosTrack, &pStartTrack, &pEndTrack))
		{
			vTrans = pStartTrack->vPos;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			time = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
			D3DXVec3Lerp(&vTrans, &vTrans, &pEndTrack->vPos, time);
		}
		D3DXMatrixTranslation(&matPos, vTrans.x, vTrans.y, vTrans.z);
	}

	pStartTrack = NULL;
	pEndTrack = NULL;
	fStartTick = m_Scene.iFristFrame * m_Scene.iTickPerFrame;
	fEndTick = 0.0f;

	if (pGeom->m_Object.m_RotTrack.size())
	{
		if (GetAnimationTrack(fFrameTick, &pGeom->m_Object.m_RotTrack, &pStartTrack, &pEndTrack))
		{
			qRotation = pStartTrack->vRot;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			time = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
			D3DXQuaternionSlerp(&qRotation, &qRotation, &pEndTrack->vRot, time);
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qRotation);
	}

	pStartTrack = NULL;
	pEndTrack = NULL;
	fStartTick = m_Scene.iFristFrame * m_Scene.iTickPerFrame;
	fEndTick = 0.0f;

	TVector3 vScale(matScale._11, matScale._22, matScale._33);
	if (pGeom->m_Object.m_ScaleTrack.size())
	{
		if (GetAnimationTrack(fFrameTick, &pGeom->m_Object.m_ScaleTrack, &pStartTrack, &pEndTrack))
		{
			qScale = pStartTrack->vRot;
			vScale = pStartTrack->vPos;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			time = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vPos, time);
			D3DXQuaternionSlerp(&qScale, &qScale, &pEndTrack->vRot, time);
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qScale);
		D3DXMatrixInverse(&matRotationInv, NULL, &matScaleRot);
		matScale = matRotationInv * matScale * matScaleRot;
	}

	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// 최종 애니메이션 행렬을 완성한다.	
	D3DXMatrixMultiply(&pGeom->m_Object.m_matCaculation, &matAnim, matParents);
}

//--------------------------------------------------------------------------------------
// 프레임에 따른 트랙정보 반환
//--------------------------------------------------------------------------------------
bool AseObject::GetAnimationTrack(float fFrame,
	vector<Animtrack>* pTrackList,
	Animtrack** pStartTrack,
	Animtrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList->size(); dwTrack++)
	{
		Animtrack* pTrack = &pTrackList->at(dwTrack);
		_ASSERT(pTrack);
		// fFrame 보다 큰 Tick 트랙이 있다면 이전 트랙을 넘겨 주어야 하기 때문에 break한다.
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// 현프레임보다 큰값이 없다면. 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}

//--------------------------------------------------------------------------------------
// 부모 오브젝트 검색
//--------------------------------------------------------------------------------------
int AseObject::SearchToObject(wstring szParent)
{
	for (int iObj = 0; iObj < m_GeomObjectList.size(); iObj++)
	{
		if (m_GeomObjectList[iObj].m_Object.m_szNodeName == szParent)
		{
			return iObj;
		}
	}
	return -1;
}

//--------------------------------------------------------------------------------------
// 재정의 함수
//--------------------------------------------------------------------------------------
bool AseObject::Frame()
{
	TMatrix mat = TMatrix::Identity;

	m_fTickFrame += g_fSecPerFrame * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame * 0.5f;
	if (m_fTickFrame >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
	{
		m_fTickFrame = m_fTickFrame - m_Scene.iLastFrame * m_Scene.iTickPerFrame;
	}

	for (int iObj = 0; iObj < m_iNumObject; iObj++)
	{
		if (m_GeomObjectList[iObj].m_pParentObj)
		{
			Interpolate(&m_GeomObjectList[iObj], &m_GeomObjectList[iObj].m_pParentObj->m_Object.m_matCaculation, m_fTickFrame);
		}
		else
		{
			Interpolate(&m_GeomObjectList[iObj], &mat, m_fTickFrame);
		}
	}

	return true;
}

bool AseObject::Render(ID3D11DeviceContext* pContext)
{
	for (int iObj = 0; iObj < m_iNumObject; iObj++)
	{
		if (m_GeomObjectList[iObj].m_Object.m_iMaterialRef < 0)
			continue;
		pContext->VSSetShader(m_GeomObjectList[iObj].m_pVS, NULL, 0);
		pContext->PSSetShader(m_GeomObjectList[iObj].m_pPS, NULL, 0);
		//m_pContext->GSSetShader(m_GeomObjectList[iObj].m_pGS, NULL, 0);
		pContext->IASetInputLayout(m_GeomObjectList[iObj].m_pVertexLayout);
		m_GeomObjectList[iObj].Render(pContext);
	}

	return true;
}

AseObject::AseObject() : m_iNumObject(0), m_fTickFrame(0.0f)
{
}


AseObject::~AseObject()
{
}
