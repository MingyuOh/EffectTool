#include "Particle.h"



Particle::Particle()
{
	// ��ġ
	m_vPos				= TVector3(0.0f, 0.0f, 0.0f);
	m_vFirstPos			= TVector3(0.0f, 0.0f, 0.0f);
	
	// ����
	m_vColor.x			= 1.0f;
	m_vColor.y			= 1.0f;
	m_vColor.z			= 1.0f;
	m_vColor.w			= 1.0f;

	// �ӵ�
	m_vSpeed.x			= randstep(3.0f, 5.0f);
	m_vSpeed.y			= randstep(3.0f, 5.0f);
	m_vSpeed.z			= randstep(3.0f, 5.0f);

	// ����
	m_vDirection.x		=  0.0f;
	m_vDirection.y		= -1.0f;
	m_vDirection.z		=  0.0f;

	// ũ��
	m_vScale			= TVector3(1.0f, 1.0f, 1.0f);

	// �߽ɰ�
	m_vCenter			= TVector3(1.0f, 1.0f, 1.0f);

	// ������
	m_fRadian			= 1.0f;
}


Particle::~Particle()
{
}
