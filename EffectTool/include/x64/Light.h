#pragma once
#include "CoreSTD.h"

class Light
{
private:
	TVector3	m_vPos;
	TVector3	m_vInitPos;
	TVector3	m_vTarget;
	TVector3	m_vDir;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
public:
	void		Set(TVector3 vPos, TVector3 vTarget);
	bool		Frame();
public:
	TMatrix		GetWorldMatrix();
	TMatrix		GetViewMatrix();
	TMatrix		GetProjMatrix();

public:
	Light();
	~Light();
};

