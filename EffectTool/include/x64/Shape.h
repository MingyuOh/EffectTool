#pragma once
#include "Object.h"

class Shape : public Object
{
public:
	void				CreateOBBBox(float fLengthX = 1.0f, float fLengthY = 1.0f, float fLengthZ = 1.0f,
								TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
								TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
								TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
								TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	void				CreateAABBBox(TVector3 vMax = TVector3(1.0f, 1.0f, 1.0f),
								TVector3 vMin = TVector3(-1.0f, -1.0f, -1.0f));
	bool				UpdateBoundingBox(CollisionBox& box, TMatrix& matrix) override;
	static bool			CheckOBBToOBB(const CollisionBox& box1, const CollisionBox& box2);
public:
	Shape();
	virtual ~Shape();
};

