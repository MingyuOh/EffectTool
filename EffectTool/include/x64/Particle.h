#pragma once
#include "CoreSTD.h"

class Particle
{
public:
	TVector3		m_vPos;
	TVector4		m_vColor;
	TVector3		m_vSpeed;
	TVector3		m_vDirection;
	TVector3		m_vScale;
	TVector3		m_vCenter;
	TVector3		m_vFirstPos;
	float			m_fRadian;
public:
	Particle();
	~Particle();
};

