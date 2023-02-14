#pragma once
#include "CoreSTD.h"
#include "Frustum.h"

class Camera :public Frustum
{
public:
	//--------------------------------------------------------------------------------------
	// 카메라와 물체의 위치 벡터
	//--------------------------------------------------------------------------------------
	TVector3			m_vPos;
	TVector3			m_vTarget;
	TVector3			m_vUp;
	//--------------------------------------------------------------------------------------
	// 위치를 나타내는 방향벡터
	//--------------------------------------------------------------------------------------
	TVector3			m_vLook;
	TVector3			m_vSide;
	//--------------------------------------------------------------------------------------
	// 카메라 회전 정보
	//--------------------------------------------------------------------------------------
	float				m_fPitch;
	float				m_fYaw;
	float				m_fRoll;
	//--------------------------------------------------------------------------------------
	// 투영 행렬의 속성값수
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
	float				m_fAspect;              // Aspect ratio
	float				m_fNearPlane;           // Near plane
	float				m_fFarPlane;            // Far plane
	//--------------------------------------------------------------------------------------
	// 카메라 속도
	//--------------------------------------------------------------------------------------
	float				m_fSpeed;
	//--------------------------------------------------------------------------------------
	// Hero 위치 방향 변수
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

