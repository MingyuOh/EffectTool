#include "Frustum.h"

//--------------------------------------------------------------------------------------
// 평면으로부터 떨어진 정점의 거리를 구하는 함수
// 점 제외 처리 ( 주의 : 평면노말은 내부을 바라보고 있다. )
//--------------------------------------------------------------------------------------
bool Frustum::ClassifyPoint(TVector3* pv)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = (m_Plane[iPlane].fA * pv->x) +
			(m_Plane[iPlane].fB * pv->y) +
			(m_Plane[iPlane].fC * pv->z) +
			m_Plane[iPlane].fD;
		if (fPlaneToPoint < 0.0f)
			return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// 평면으로부터 떨어진 원의 거리를 구하는 함수
// 구 제외 처리 ( 구의 원점과 평면과의 거리보다 반지름이 클 경우에 제외)
//--------------------------------------------------------------------------------------
bool Frustum::ClassifySphere(CollisionSphere* pSphere)
{
	float fPlaneToCenter;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToCenter = (m_Plane[iPlane].fA * pSphere->vCenter.x) +
			(m_Plane[iPlane].fB * pSphere->vCenter.y) +
			(m_Plane[iPlane].fC * pSphere->vCenter.z) +
			m_Plane[iPlane].fD;

		if (fPlaneToCenter <= -pSphere->fRadius)
			return false;
	}
	return true;
}

void Frustum::SeTMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
	}
	else
	{
		m_matWorld = TMatrix::Identity;
	}

	if (pView != NULL)
	{
		m_matView = *pView;
	}

	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
}

void Frustum::ExtractPlanesD3D(CollisionPlane* pPlanes, const TMatrix & comboTMatrix)
{
	if (pPlanes == NULL)
		return;

	// Left clipping plane
	pPlanes[0].fA = comboTMatrix._14 + comboTMatrix._11;
	pPlanes[0].fB = comboTMatrix._24 + comboTMatrix._21;
	pPlanes[0].fC = comboTMatrix._34 + comboTMatrix._31;
	pPlanes[0].fD = comboTMatrix._44 + comboTMatrix._41;
	// Right clipping plane
	pPlanes[1].fA = comboTMatrix._14 - comboTMatrix._11;
	pPlanes[1].fB = comboTMatrix._24 - comboTMatrix._21;
	pPlanes[1].fC = comboTMatrix._34 - comboTMatrix._31;
	pPlanes[1].fD = comboTMatrix._44 - comboTMatrix._41;
	// Top clipping plane
	pPlanes[2].fA = comboTMatrix._14 - comboTMatrix._12;
	pPlanes[2].fB = comboTMatrix._24 - comboTMatrix._22;
	pPlanes[2].fC = comboTMatrix._34 - comboTMatrix._32;
	pPlanes[2].fD = comboTMatrix._44 - comboTMatrix._42;
	// Bottom clipping plane
	pPlanes[3].fA = comboTMatrix._14 + comboTMatrix._11;
	pPlanes[3].fB = comboTMatrix._24 + comboTMatrix._21;
	pPlanes[3].fC = comboTMatrix._34 + comboTMatrix._31;
	pPlanes[3].fD = comboTMatrix._44 + comboTMatrix._41;
	// Near clipping plane
	pPlanes[4].fA = comboTMatrix._13;
	pPlanes[4].fB = comboTMatrix._23;
	pPlanes[4].fC = comboTMatrix._33;
	pPlanes[4].fD = comboTMatrix._43;
	// Far clipping plane
	pPlanes[5].fA = comboTMatrix._14 - comboTMatrix._13;
	pPlanes[5].fB = comboTMatrix._24 - comboTMatrix._23;
	pPlanes[5].fC = comboTMatrix._34 - comboTMatrix._33;
	pPlanes[5].fD = comboTMatrix._44 - comboTMatrix._43;

	pPlanes[0].Normalize();
	pPlanes[1].Normalize();
	pPlanes[2].Normalize();
	pPlanes[3].Normalize();
	pPlanes[4].Normalize();
	pPlanes[5].Normalize();
}

bool Frustum::CreateRenderBox(ID3D11Device*  pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	if (m_pBoxShape != NULL)
		return true;
	SAFE_NEW(m_pBoxShape, Box);
	Init();
	m_pBoxShape->Init();
	if (FAILED(m_pBoxShape->Create(_T(""), L"../data/Shader/VertexShader.vsh", L"../data/Shader/PixelShader.psh", L"../data/Shader/GeometryShader.gsh")))
	{
		MessageBox(0, _T("m_LineShape 실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	return true;
}

bool Frustum::CreateFrustum()
{
	m_matViewProj = m_matView * m_matProj;
	ExtractPlanesD3D(m_Plane, m_matViewProj);

	// view * proj의 역행렬을 구한다.(프러스텀 박스생성)
	m_matViewProj.Invert(m_matViewProj);
	//TMatrixInverse(&m_matViewProj, NULL, &m_matViewProj);
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3
	m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TVector3(-1.0f,  1.0f, 0.0f);
	m_vFrustum[2] = TVector3( 1.0f,  1.0f, 0.0f);
	m_vFrustum[3] = TVector3( 1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TVector3(-1.0f,  1.0f, 1.0f);
	m_vFrustum[6] = TVector3( 1.0f,  1.0f, 1.0f);
	m_vFrustum[7] = TVector3( 1.0f, -1.0f, 1.0f);

	// Vertex_최종 = Vertex_local * TMatrix_world * TMatrix_view * TMatrix_Proj 인데,
	// Vertex_world = Vertex_local * TMatrix_world이므로,
	// Vertex_최종 = Vertex_world * TMatrix_view * TMatrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( TMatrix_view * TMatrix_Proj ) 에서
	// 역행렬( TMatrix_view * TMatrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( TMatrix_view * TMatrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_vFrustum * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		m_vFrustum[iVertex].Transform(m_vFrustum[iVertex], m_matProj);
	}

	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
	return true;
}

BOOL Frustum::CheckOBBInPlane(CollisionBox*  pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	TVector3 vDir;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fLength[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fLength[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fLength[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y +
			m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= -fDistance)
			return FALSE;
	}
	return TRUE;
}

T_POSITION Frustum::CheckPoitionOBBInPlane(CollisionBox*  pBox)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	TVector3 vDir;
	T_POSITION  t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fLength[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fLength[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fLength[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y +
			m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= fDistance)
		{
			t_Position = P_SPANNING;
		}

		if (fPlaneToCenter + 1.0f < -fDistance)
		{
			return P_BACK;
		}
	}
	return t_Position;
}

bool Frustum::Init()
{
	m_vNearCenter = TVector3(0.0f, 0.0f, 0.0f);
	ZeroMemory(m_vFrustum, sizeof(m_vFrustum[0]) * 8);
	ZeroMemory(m_Plane, sizeof(m_Plane[0]) * 6);
	m_VertexList.resize(36);
	return true;
}

bool Frustum::Frame()
{
	return true;
}

bool Frustum::PreRender(ID3D11DeviceContext* pContext)
{
	if (!m_pBoxShape)
		return true;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_pBoxShape->m_VertexList[0] = PNCT_VERTEX(m_vFrustum[1], TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[1] = PNCT_VERTEX(m_vFrustum[2], TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[2] = PNCT_VERTEX(m_vFrustum[3], TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[3] = PNCT_VERTEX(m_vFrustum[0], TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 0.5f), TVector2(0.0f, 1.0f));
	// 뒷면
	m_pBoxShape->m_VertexList[4] = PNCT_VERTEX(m_vFrustum[6], TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[5] = PNCT_VERTEX(m_vFrustum[5], TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[6] = PNCT_VERTEX(m_vFrustum[4], TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[7] = PNCT_VERTEX(m_vFrustum[7], TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 0.5f), TVector2(0.0f, 1.0f));

	// 왼쪽
	m_pBoxShape->m_VertexList[8] = PNCT_VERTEX(m_vFrustum[5], TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[9] = PNCT_VERTEX(m_vFrustum[1], TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[10] = PNCT_VERTEX(m_vFrustum[0], TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[11] = PNCT_VERTEX(m_vFrustum[4], TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 0.5f), TVector2(0.0f, 1.0f));

	// 오늘쪽면
	m_pBoxShape->m_VertexList[12] = PNCT_VERTEX(m_vFrustum[2], TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[13] = PNCT_VERTEX(m_vFrustum[6], TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[14] = PNCT_VERTEX(m_vFrustum[7], TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[15] = PNCT_VERTEX(m_vFrustum[3], TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 0.5f), TVector2(0.0f, 1.0f));

	// 윗면
	m_pBoxShape->m_VertexList[16] = PNCT_VERTEX(m_vFrustum[5], TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[17] = PNCT_VERTEX(m_vFrustum[6], TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[18] = PNCT_VERTEX(m_vFrustum[2], TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[19] = PNCT_VERTEX(m_vFrustum[1], TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 0.5f), TVector2(0.0f, 1.0f));

	// 아랫면
	m_pBoxShape->m_VertexList[20] = PNCT_VERTEX(m_vFrustum[0], TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 0.5f), TVector2(0.0f, 0.0f));
	m_pBoxShape->m_VertexList[21] = PNCT_VERTEX(m_vFrustum[3], TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 0.5f), TVector2(1.0f, 0.0f));
	m_pBoxShape->m_VertexList[22] = PNCT_VERTEX(m_vFrustum[7], TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 0.5f), TVector2(1.0f, 1.0f));
	m_pBoxShape->m_VertexList[23] = PNCT_VERTEX(m_vFrustum[4], TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 0.5f), TVector2(0.0f, 1.0f));
	
	m_pBoxShape->Render(pContext);
	return true;
}


bool Frustum::PostRender(ID3D11DeviceContext* pContext)
{
	return true;
}

bool Frustum::Render(ID3D11DeviceContext* pContext)
{
	m_pBoxShape->Render(pContext);
	return true;
}

bool Frustum::Release()
{
	SAFE_DEL(m_pBoxShape);
	return true;
}

bool Frustum::ResetDevice()
{
	return true;
}

bool Frustum::ResetResource()
{ 
	return true;
}

Frustum::Frustum()
{
	m_pBoxShape = NULL;
}

Frustum::~Frustum()
{

}

bool CollisionPlane::CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 vEdge0 = v1 - v0;
	TVector3 vEdge1 = v2 - v0;
	TVector3 vNormal;
	vNormal = vEdge0.Cross(vEdge1);
	vNormal.Normalize();
	//D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
	//D3DXVec3Normalize(&vNormal, &vNormal);

	fA = vNormal.x; 
	fB = vNormal.y; 
	fC = vNormal.z;
	fD = -(fA*v0.x + fB*v0.y + fC*v0.z);
	return true;
}
bool CollisionPlane::CreatePlane(TVector3 vNormal, TVector3 v0)
{
	vNormal.Normalize();
	//D3DXVec3Normalize(&vNormal, &vNormal);

	fA = vNormal.x; fB = vNormal.y; fC = vNormal.z;
	fD = -(fA*v0.x + fB*v0.y + fC*v0.z);
	return true;
}