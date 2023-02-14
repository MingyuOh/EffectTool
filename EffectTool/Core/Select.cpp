#include "Select.h"

Select::Select()
{
	m_bSelect = false;
	m_fPickDistance = 0.0f;
	m_pSelectedObject = nullptr;
}

Select::~Select()
{

}

bool Select::PointInPolygon(TVector3 vert, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	// v0    v1
	//       
	// v2    v3
	//
	//     v
	TVector3 e0, e1, iInter, vNormal;
	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = vert - v1;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f)
	{
		return false;
	}

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f)
	{
		return false;
	}

	//     v1
	//     v
	// v0      v2
	//
	//     v
	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = vert - v2;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f)
	{
		return false;
	}

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f)
	{
		return false;
	}
	
	return true;

}

bool Select::CheckPick(TVector3& v0, TVector3& v1, TVector3& v2)
{
	float fBary1, fBary2;
	float fDistance;

	// Check if the pick ray passes through this point
	if (IntersectTriangle(m_Ray.vOrigin, m_Ray.vDirection, v0, v1, v2, &fDistance, &fBary1, &fBary2))
	{
		m_vSrcVector[0] = v0;
		m_vSrcVector[1] = v1;
		m_vSrcVector[2] = v2;
		return true;
	}

	return false;
}

bool Select::CheckOBBToRay(CollisionBox* pBox, Ray* pRay)
{
	if (pRay == nullptr)
	{
		pRay = &m_Ray;
	}

	float t_min = -999999.0f;
	float t_max = 999999.0f;

	float f[3], s[3], fa[3], sa[3];

	TVector3 vR = pRay->vOrigin - pBox->vCenter;

	// 반직선의 위치와 방향성 검사
	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection);		// 분리축으로부터 방향 체크
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR);					// 분리축 내에 위치한지 체크
		fa[v] = fabs(f[v]);											// 방향 벡터 크기
		sa[v] = fabs(s[v]);											// 위치 벡터 크기

		if (sa[v] > pBox->fLength[v] && f[v] * s[v] >= 0.0f)
		{
			return false;
		}

		float t1 = (-s[v] - pBox->fLength[v]) / f[v];
		float t2 = (-s[v] + pBox->fLength[v]) / f[v];

		if (t1 > t2)
		{
			swap(t1, t2);
		}

		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
		{
			return false;
		}
	}

	// OBB 박스와 반직선 충돌 판정 ( f > s => 충돌 )
	float fCloss[3], fRhs;
	TVector3 vDxR;
	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);

	// f(x) - x분리축(x (cross) direction)에 OBB박스 투영
	// f(x) = (OBB 박스의 y축 길이 * ( OBB 박스의 z축 (dot) 반직선 방향 벡터 )) 
	//          + (OBB 박스의 z축 길이 * ( OBB 박스의 y축 (dot) 반직선 방향 벡터 )) 
	fRhs = (pBox->fLength[1] * fa[2]) + (pBox->fLength[2] * fa[1]);
	// s(x) - 반직선 시점과 OBB 중점을 잇는 벡터의 x축 vR 투영값
	// s(x) = x축 (dot) 반직선의 방향 벡터 (cross) vR(반직선의 시점 - OBB 박스의 중점) 
	fCloss[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	if (fRhs < fCloss[0])
	{
		m_vDxR = vDxR;
		return false;
	}

	// f(y) - y분리축(y (cross) direction)에 OBB박스 투영
	// f(y) = (OBB 박스의 x축 길이 * ( OBB 박스의 z축 (dot) 반직선 방향 벡터 )) 
	//          + (OBB 박스의 z축 길이 * ( OBB 박스의 x축 (dot) 반직선 방향 벡터 )) 
	fRhs = (pBox->fLength[0] * fa[2]) + (pBox->fLength[2] * fa[0]);
	// s(y) - 반직선 시점과 OBB 중점을 잇는 벡터의 y축 vR 투영값
	// s(y) = y축 (dot) 반직선의 방향 벡터 (cross) vR(반직선의 시점 - OBB 박스의 중점) 
	fCloss[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	if (fRhs < fCloss[1])
	{
		m_vDxR = vDxR;
		return false;
	}

	// f(z) - z분리축(z (cross) direction)에 OBB박스 투영
	// f(z) = (OBB 박스의 x축 길이 * ( OBB 박스의 y축 (dot) 반직선 방향 벡터 )) 
	//          + (OBB 박스의 y축 길이 * ( OBB 박스의 x축 (dot) 반직선 방향 벡터 )) 
	fRhs = (pBox->fLength[0] * fa[1]) + (pBox->fLength[1] * fa[0]);
	// s(z) - 반직선 시점과 OBB 중점을 잇는 벡터의 z축 vR 투영값
	// s(z) = z축 (dot) 반직선의 방향 벡터 (cross) vR(반직선의 시점 - OBB 박스의 중점) 
	fCloss[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	if (fRhs < fCloss[2])
	{
		m_vDxR = vDxR;
		return false;
	}

	m_fPickDistance = t_min;
	m_vIntersection = pRay->vOrigin + pRay->vDirection * m_fPickDistance;
	return true;
}

bool Select::GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 vDirection = vEnd - vStart;
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	float a0 = D3DXVec3Dot(&vNormal, &(v0 - vStart));
	float fT = a0 / D;

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}
	m_vIntersection = vStart + (vDirection * fT);

	return true;
}

bool Select::IntersectTriangle(const TVector3& origin, const TVector3& dir, TVector3& v0, TVector3& v1, TVector3& v2, float* t, float* u, float* v)
{
	TVector3 edge1 = v1 - v0;
	TVector3 edge2 = v2 - v0;

	D3DXVec3Cross(&m_pvec, &dir, &edge2);

	float det = D3DXVec3Dot(&edge1, &m_pvec);
	float fInvDet = 1.0f / det;

	if (det > 0)
	{
		m_tvec = origin - v0;
	}
	else
	{
		// 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
		m_tvec = v0 - origin;
		det = -det;
	}

	if (det < 0.0001f)
	{
		return false;
	}

	// u값 계산
	*u = D3DXVec3Dot(&m_tvec, &m_pvec);
	if (*u < 0.0f || *u > det)
	{
		return false;
	}

	// v값 계산
	D3DXVec3Cross(&m_qvec, &m_tvec, &edge1);
	*v = D3DXVec3Dot(&dir, &m_qvec);
	if (*v < 0.0f || *u + *v > det)
	{
		return false;
	}

	// t값 계산
	*t = D3DXVec3Dot(&edge2, &m_qvec);
	
	*u *= fInvDet;
	*v *= fInvDet;
	*t *= fInvDet;

	return true;
}

bool Select::IntersectRayToSphere(CollisionSphere* pSphere, Ray* pRay)
{
	if (pRay == nullptr)
	{
		pRay = &m_Ray;
	}

	// o(origin) - c(center)
	TVector3 vDir = pRay->vOrigin - pSphere->vCenter;

	// B = 2(u dot ( o - c ))
	float fProj = D3DXVec3Dot(&pRay->vDirection, &vDir);

	// 제한된 길이(세그먼트) 제외 처리
	if (pRay->fExtent > -1.0f && pRay->fExtent < fabs(fProj))
	{
		return false;
	}

	float b = 2.0f * fProj;

	// C = (( o - c ) dot ( o - c )) - (r * r)
	float c = D3DXVec3Dot(&vDir, &vDir) - (pSphere->fRadius * pSphere->fRadius);

	// D = B * B - 4 * AC
	float fDiscriminant = (b * b) - (4.0f * c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);

	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t1;
		return true;
	}

	return false;
}

bool Select::IntersectBox(CollisionBox* pBox, Ray* pRay)
{
	if (pRay == nullptr)
	{
		pRay = &m_Ray;
	}

	TVector3 tmin;
	tmin.x = (pBox->vMin.x - pRay->vOrigin.x) / (pRay->vDirection.x + 0.001f);
	tmin.y = (pBox->vMin.y - pRay->vOrigin.y) / (pRay->vDirection.y + 0.001f);
	tmin.z = (pBox->vMin.z - pRay->vOrigin.z) / (pRay->vDirection.z + 0.001f);

	TVector3 tmax;
	tmax.x = (pBox->vMax.x - pRay->vOrigin.x) / (pRay->vDirection.x + 0.001f);
	tmax.y = (pBox->vMax.y - pRay->vOrigin.y) / (pRay->vDirection.y + 0.001f);
	tmax.z = (pBox->vMax.z - pRay->vOrigin.z) / (pRay->vDirection.z + 0.001f);

	TVector3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);

	TVector3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * maxmin;
		return true;
	}

	return false;
}

void Select::Update()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	m_Ray.vPrevDirection = m_Ray.vDirection;
	m_vPrevMousePos = m_vMousePos;

	TVector3 v;
	v.x = (((2.0f * ptCursor.x) / g_iWindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / g_iWindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	TMatrix matWorldView = m_matWorld * m_matView;
	TMatrix mat;
	D3DXMatrixInverse(&mat, nullptr, &matWorldView);

	m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_vMousePos, &v, &mat);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &mat);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &mat);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
}

void Select::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}

	if (pView != nullptr)
	{
		m_matView = *pView;
	}

	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}

	Update();
}

bool Select::CheckObjectSelection(Object* pObject)
{
	assert(pObject);

	if (I_Input.m_MouseState[0] == KEY_PUSH)
	{
		if (CheckOBBToRay(&pObject->m_BoundingBox))
		{
			m_bSelect = true;
			m_pSelectedObject = pObject;
			return true;
		}
		else
		{
			m_bSelect = false;
			m_pSelectedObject = nullptr;
		}
	}

	return false;
}

bool Select::MoveObject(TMatrix& matWorld)
{
	if (m_pSelectedObject == nullptr)
	{
		return false;
	}

	if (m_bSelect == true)
	{
		if (I_Input.m_MouseState[0] == KEY_HOLD)
		{
			TVector3 vMoveDirection = m_Ray.vDirection - m_Ray.vPrevDirection;
			D3DXVec3Normalize(&vMoveDirection, &vMoveDirection);
			
			float fDistance = fabs(D3DXVec3Length(&(m_vMousePos - m_vPrevMousePos)));
			matWorld._41 += (vMoveDirection.x * fDistance * 50.0f);
			matWorld._42 += (vMoveDirection.y * fDistance * 50.0f);
			matWorld._43 += (vMoveDirection.z * fDistance * 50.0f);
			m_pSelectedObject->UpdateBoundingBox(m_pSelectedObject->m_BoundingBox, matWorld);
		}
	}

	return true;
}