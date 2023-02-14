#pragma once
#include "Input.h"
#include "Object.h"

struct Ray
{
	TVector3	vOrigin;
	TVector3	vPrevDirection;
	TVector3	vDirection;
	float		fExtent;
	Ray()
	{
		fExtent = -1;
	}
};

class Select
{
public:
	Ray			m_Ray;
	TVector3	m_vIntersection;
	TVector3	m_vCenter;
	TVector3	m_vMousePos;
	TVector3	m_vPrevMousePos;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
public:
	TVector3	m_vDxR;
	TVector3	m_vSrcVector[3];
	float		m_fPickDistance;
public:
	TVector3	m_pvec;
	TVector3	m_tvec;
	TVector3	m_qvec;
public:
	Object*		m_pSelectedObject;
	bool		m_bSelect;
public:
	bool		PointInPolygon(TVector3 vert, TVector3 faceNormal,TVector3 v0,TVector3 v1,TVector3 v2);
	bool		CheckPick(TVector3& v0, TVector3& v1, TVector3& v2);
	bool		CheckOBBToRay(CollisionBox* pBox, Ray* pRay = nullptr);
public:
	bool		GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2);
	bool		IntersectTriangle(const TVector3& origin, const TVector3& dir, TVector3& v0, TVector3& v1, TVector3& v2, float* t, float* u, float* v);
	bool		IntersectRayToSphere(CollisionSphere* pSphere, Ray* pRay = nullptr);
	bool		IntersectBox(CollisionBox* pBox, Ray* pRay = nullptr);
public:
	void		Update();
	void		SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	bool		CheckObjectSelection(Object* pObject);
	bool		MoveObject(TMatrix& matWorld);
public:
	Select();
	~Select();
};

