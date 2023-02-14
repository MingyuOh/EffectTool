#include "Shape.h"

void Shape::CreateOBBBox(float fLengthX, float fLengthY, float fLengthZ, 
	TVector3 vCenter, TVector3 vDirX,TVector3 vDirY,TVector3 vDirZ)
{
	m_BoundingBox.vColor = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BoundingBox.fLength[0] = fLengthX;
	m_BoundingBox.fLength[1] = fLengthY;
	m_BoundingBox.fLength[2] = fLengthZ;
	m_BoundingBox.vCenter = vCenter;

	m_BoundingBox.vAxis[0] = m_BoundingBox.fLength[0] * vDirX;
	m_BoundingBox.vAxis[1] = m_BoundingBox.fLength[1] * vDirY;
	m_BoundingBox.vAxis[2] = m_BoundingBox.fLength[2] * vDirZ;

	m_BoundingBox.vPos[0] = vCenter - m_BoundingBox.vAxis[0] - m_BoundingBox.vAxis[1] - m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[1] = vCenter - m_BoundingBox.vAxis[0] + m_BoundingBox.vAxis[1] - m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[2] = vCenter + m_BoundingBox.vAxis[0] + m_BoundingBox.vAxis[1] - m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[3] = vCenter + m_BoundingBox.vAxis[0] - m_BoundingBox.vAxis[1] - m_BoundingBox.vAxis[2];

	m_BoundingBox.vPos[4] = vCenter - m_BoundingBox.vAxis[0] - m_BoundingBox.vAxis[1] + m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[5] = vCenter - m_BoundingBox.vAxis[0] + m_BoundingBox.vAxis[1] + m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[6] = vCenter + m_BoundingBox.vAxis[0] + m_BoundingBox.vAxis[1] + m_BoundingBox.vAxis[2];
	m_BoundingBox.vPos[7] = vCenter + m_BoundingBox.vAxis[0] - m_BoundingBox.vAxis[1] + m_BoundingBox.vAxis[2];

	m_BoundingBox.vMax = m_BoundingBox.vPos[0];
	m_BoundingBox.vMin = m_BoundingBox.vPos[0];

	for (int iPoint = 1; iPoint < 8; iPoint++)
	{
		if (m_BoundingBox.vMax.x < m_BoundingBox.vPos[iPoint].x)
		{
			m_BoundingBox.vMax.x = m_BoundingBox.vPos[iPoint].x;
		}
		if (m_BoundingBox.vMax.y < m_BoundingBox.vPos[iPoint].y)
		{
			m_BoundingBox.vMax.y = m_BoundingBox.vPos[iPoint].y;
		}
		if (m_BoundingBox.vMax.z < m_BoundingBox.vPos[iPoint].z)
		{
			m_BoundingBox.vMax.z = m_BoundingBox.vPos[iPoint].z;
		}

		if (m_BoundingBox.vMin.x > m_BoundingBox.vPos[iPoint].x)
		{
			m_BoundingBox.vMin.x = m_BoundingBox.vPos[iPoint].x;
		}
		if (m_BoundingBox.vMin.y > m_BoundingBox.vPos[iPoint].y)
		{
			m_BoundingBox.vMin.y = m_BoundingBox.vPos[iPoint].y;
		}
		if (m_BoundingBox.vMin.z > m_BoundingBox.vPos[iPoint].z)
		{
			m_BoundingBox.vMin.z = m_BoundingBox.vPos[iPoint].z;
		}
	}
	D3DXVec3Normalize(&m_BoundingBox.vAxis[0], &m_BoundingBox.vAxis[0]);
	D3DXVec3Normalize(&m_BoundingBox.vAxis[1], &m_BoundingBox.vAxis[1]);
	D3DXVec3Normalize(&m_BoundingBox.vAxis[2], &m_BoundingBox.vAxis[2]);
}

void Shape::CreateAABBBox(TVector3 vMax, TVector3 vMin)
{
	m_BoundingBox.vMax = vMax;
	m_BoundingBox.vMin = vMin;
	m_BoundingBox.vPos[0] = TVector3(vMin.x, vMin.y, vMin.z);
	m_BoundingBox.vPos[1] = TVector3(vMin.x, vMax.y, vMin.z);
	m_BoundingBox.vPos[2] = TVector3(vMax.x, vMax.y, vMin.z);
	m_BoundingBox.vPos[3] = TVector3(vMax.x, vMin.y, vMin.z);

	m_BoundingBox.vPos[4] = TVector3(vMin.x, vMin.y, vMax.z);
	m_BoundingBox.vPos[5] = TVector3(vMin.x, vMax.y, vMax.z);
	m_BoundingBox.vPos[6] = TVector3(vMax.x, vMax.y, vMax.z);
	m_BoundingBox.vPos[7] = TVector3(vMax.x, vMin.y, vMax.z);

	m_BoundingBox.vCenter = (vMax + vMin);
	m_BoundingBox.vCenter *= 0.5f;
	m_BoundingBox.vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	m_BoundingBox.vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	m_BoundingBox.vAxis[2] = TVector3(0.0f, 0.0f, 1.0f);
}

bool Shape::UpdateBoundingBox(CollisionBox& box, TMatrix& matrix)
{
	// �������� ȸ��
	D3DXVec3TransformNormal(&box.vAxis[0], &m_InitBox.vAxis[0], &matrix);
	D3DXVec3TransformNormal(&box.vAxis[1], &m_InitBox.vAxis[1], &matrix);
	D3DXVec3TransformNormal(&box.vAxis[2], &m_InitBox.vAxis[2], &matrix);

	D3DXVec3Normalize(&box.vAxis[0], &box.vAxis[0]);
	D3DXVec3Normalize(&box.vAxis[1], &box.vAxis[1]);
	D3DXVec3Normalize(&box.vAxis[2], &box.vAxis[2]);

	TVector3 vMax, vMin;
	vMax = this->m_InitBox.vMax;
	vMin = this->m_InitBox.vMin;
	D3DXVec3TransformCoord(&box.vMax, &vMax, &matrix);
	D3DXVec3TransformCoord(&box.vMin, &vMin, &matrix);

	box.vCenter.x = matrix._41;
	box.vCenter.y = matrix._42;
	box.vCenter.z = matrix._43;

	float test[3];
	test[0] = box.fLength[0];
	test[1] = box.fLength[1];
	test[2] = box.fLength[2];

	box.fLength[0] = (box.vMax.x - box.vMin.x) * 0.5f;
	box.fLength[1] = (box.vMax.y - box.vMin.y) * 0.5f;
	box.fLength[2] = (box.vMax.z - box.vMin.z) * 0.5f;

	return true;
}

bool Shape::CheckOBBToOBB(const CollisionBox& box1, const CollisionBox& box2)
{
	// "D > ��1 + ��2" ��쿡�� �и� ����� 
	// �� OBB���̿� ������ �� �ִٴ� ���� �Ǹ�
	// ���� ���ɼ����� ����
	
	// D: �� �ڽ� ������ �Ÿ� ����
	TVector3 D = box2.vCenter - box1.vCenter;

	// C[n][n]: �� ��° OBB n �࿡ ù ��° OBB�� n ���� ����
	// absC[n][n]: �� ��° OBB n �࿡ ù ��° OBB�� n ���� ������ ũ��
	// AD[n]:�� OBB ������ �Ÿ� ���Ϳ� ù��° OBB�� n ���� ����
	// absD: �� OBB ������ �Ÿ� ���Ϳ� ù ��° OBB�� n ���� ���� ũ��
	// p1: ù ��° OBB ����, p2: �� ��° OBB ����

	float C[3][3];
	float absC[3][3];
	float AD[3];
	float p1, p2, absD;

	// �и���( w ) = ù ��° OBB x��( A0 )
	C[0][0] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[0]);
	C[0][1] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[1]);
	C[0][2] = D3DXVec3Dot(&box1.vAxis[0], &box2.vAxis[2]);
	absC[0][0] = fabs(C[0][0]);
	absC[0][1] = fabs(C[0][1]);
	absC[0][2] = fabs(C[0][2]);
	AD[0] = D3DXVec3Dot(&box1.vAxis[0], &D);
	absD = fabs(AD[0]);
	p1 = box1.fLength[0];
	p2 = box1.fLength[0] * absC[0][0] + box1.fLength[1] * absC[0][1] + box1.fLength[2] * absC[0][2];
	if (absD > p1 + p2)
	{
		return false;
	}

	// �и���( w ) = ù ��° OBB y��( A1 )
	C[1][0] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[0]);
	C[1][1] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[1]);
	C[1][2] = D3DXVec3Dot(&box1.vAxis[1], &box2.vAxis[2]);
	absC[1][0] = fabs(C[1][0]);
	absC[1][1] = fabs(C[1][1]);
	absC[1][2] = fabs(C[1][2]);
	AD[1] = D3DXVec3Dot(&box1.vAxis[1], &D);
	absD = fabs(AD[1]);
	p1 = box1.fLength[1];
	p2 = box1.fLength[0] * absC[1][0] + box1.fLength[1] * absC[1][1] + box1.fLength[2] * absC[1][2];
	if (absD > p1 + p2)
	{
		return false;
	}
	
	// �и���( w ) = ù ��° OBB z��( A2 )
	C[2][0] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[0]);
	C[2][1] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[1]);
	C[2][2] = D3DXVec3Dot(&box1.vAxis[2], &box2.vAxis[2]);
	absC[2][0] = fabs(C[2][0]);
	absC[2][1] = fabs(C[2][1]);
	absC[2][2] = fabs(C[2][2]);
	AD[2] = D3DXVec3Dot(&box1.vAxis[2], &D);
	absD = fabs(AD[2]);
	p1 = box1.fLength[2];
	p2 = box1.fLength[0] * absC[2][0] + box1.fLength[1] * absC[2][1] + box1.fLength[2] * absC[2][2];
	if (absD > p1 + p2)
	{
		return false;
	}

	// �и���( w ) = �� ��° OBB x��( B0 )
	absD = static_cast<float>(fabs(D3DXVec3Dot(&box2.vAxis[0], &D)));
	p1 = box1.fLength[0] * absC[0][0] + box1.fLength[1] * absC[0][1] + box1.fLength[2] * absC[0][2];
	p2 = box2.fLength[0];
	if (absD > p1 + p2)
	{
		return false;
	}

	// �и���( w ) = �� ��° OBB y��( B1 )
	absD = static_cast<float>(fabs(D3DXVec3Dot(&box2.vAxis[1], &D)));
	p1 = box1.fLength[0] * absC[1][0] + box1.fLength[1] * absC[1][1] + box1.fLength[2] * absC[1][2];
	p2 = box2.fLength[1];
	if (absD > p1 + p2)
	{
		return false;
	}

	// �и���( w ) = �� ��° OBB z��( B2 )
	absD = static_cast<float>(fabs(D3DXVec3Dot(&box2.vAxis[2], &D)));
	p1 = box1.fLength[0] * absC[2][0] + box1.fLength[1] * absC[2][1] + box1.fLength[2] * absC[2][2];
	p2 = box2.fLength[2];
	if (absD > p1 + p2)
	{
		return false;
	}

	// �и���( w ) = ù ��° OBB x�� x(����) �� ��° OBB x��( A0 x B0 )
	absD = static_cast<float>(fabs(AD[2] * C[1][0] - AD[1] * C[2][0]));
	p1 = box1.fLength[0] * absC[2][0] + box1.fLength[1] * absC[2][1] + box1.fLength[2] * absC[2][2];
	p2 = box2.fLength[2];
	if (absD > p1 + p2)
	{
		return false;
	}
}

Shape::Shape()
{
}


Shape::~Shape()
{
}
