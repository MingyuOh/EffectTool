#pragma once
#include "Core.h"
#include "Plane.h"
#include "BoundingBoxShape.h"
#include "ParticleSystemManager.h"

#define EFFECT_EXTINCTION_DISTANCE 10.0f 

class Main :public Core
{
public:
	// ¿Ã∆Â∆Æ
	vector<ParticleSystem>	m_ParticleSystem;
	int						m_iParticleSystemCount;
public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();
public:
	Main();
	virtual ~Main();
};

