#include "Light.h"

void Light::Set(TVector3 vPos, TVector3 vTarget)
{
	m_vInitPos = vPos;
	m_vPos = vPos;
	m_vTarget = vTarget;

	m_vDir = m_vTarget - m_vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	TVector3 vUp(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, NEAR_DISTANCE, FAR_DISTANCE);
}

bool Light::Frame()
{
	D3DXMatrixRotationY(&m_matWorld, XM_PI * g_fElapseTime * 0.1f);
	D3DXVec3TransformCoord(&m_vPos, &m_vInitPos, &m_matWorld);
	
	m_vDir = m_vTarget - m_vPos;
	TVector3 vUp(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, NEAR_DISTANCE, FAR_DISTANCE);

	return true;
}

TMatrix Light::GetWorldMatrix()
{
	return m_matWorld;
}

TMatrix	Light::GetViewMatrix()
{
	return m_matView;
}

TMatrix Light::GetProjMatrix()
{
	return m_matProj;
}

Light::Light()
{
	
}

Light::~Light()
{

}