#pragma once
#include "CoreSTD.h"
#include "Frustum.h"

class Camera :public Frustum
{
public:
	//--------------------------------------------------------------------------------------
	// ī�޶�� ��ü�� ��ġ ����
	//--------------------------------------------------------------------------------------
	TVector3			m_vPos;
	TVector3			m_vTarget;
	TVector3			m_vUp;
	//--------------------------------------------------------------------------------------
	// ��ġ�� ��Ÿ���� ���⺤��
	//--------------------------------------------------------------------------------------
	TVector3			m_vLook;
	TVector3			m_vSide;
	//--------------------------------------------------------------------------------------
	// ī�޶� ȸ�� ����
	//--------------------------------------------------------------------------------------
	float				m_fPitch;
	float				m_fYaw;
	float				m_fRoll;
	//--------------------------------------------------------------------------------------
	// ���� ����� �Ӽ�����
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
	float				m_fAspect;              // Aspect ratio
	float				m_fNearPlane;           // Near plane
	float				m_fFarPlane;            // Far plane
	//--------------------------------------------------------------------------------------
	// ī�޶� �ӵ�
	//--------------------------------------------------------------------------------------
	float				m_fSpeed;
	//--------------------------------------------------------------------------------------
	// Hero ��ġ ���� ����
	//--------------------------------------------------------------------------------------
	TVector3			m_vObjectPos;
	TVector3			m_vObjectDir;
public:
	bool						Init();
	virtual		bool			Frame();
	bool						Render();
	bool						Release();
	bool						SetProjTMatrix(UINT iWidth, UINT iHeight, float fFov = PI * 0.25, float fNear = NEAR_DISTANCE, float fFar = FAR_DISTANCE);
	bool						SetViewTMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp);
	bool						UpdateVector();
	virtual		bool			UpdateQuaternion();
	virtual		bool			Update();
public:
	Camera();
	virtual ~Camera();
};

