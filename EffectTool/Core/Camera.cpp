#include "Camera.h"
#include "Input.h"

bool Camera::Init()
{
	m_vPos = TVector3(10.0f, 10.0f, -50.0f);
	m_vTarget = TVector3(0.0f, 0.0f, 0.0f);
	m_vUp = TVector3(0.0f, 1.0f, 0.0f);

	SetViewTMatrix(m_vPos, m_vTarget, m_vUp);
	SetProjTMatrix(g_iWindowWidth, g_iWindowHeight);
	return true;
}

bool Camera::Frame()
{
	// 가속도
	if (I_Input.KeyCheck(DIK_SPACE) == KEY_HOLD)
	{
		m_fSpeed += 10.0f * g_fSecPerFrame;
	}
	else
	{
		m_fSpeed -= 10.0f * g_fSecPerFrame;
		if (m_fSpeed < 1.0f)
			m_fSpeed = 1.0f;
	}

	// 위
	if (I_Input.KeyCheck(DIK_W) == KEY_HOLD)
	{
		m_vPos += m_vLook * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 아래
	if (I_Input.KeyCheck(DIK_S) == KEY_HOLD)
	{
		m_vPos -= m_vLook * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 왼쪽
	if (I_Input.KeyCheck(DIK_A) == KEY_HOLD)
	{
		m_vPos -= m_vSide * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	// 오른쪽
	if (I_Input.KeyCheck(DIK_D) == KEY_HOLD)
	{
		m_vPos += m_vSide * m_fSpeed * 10.0f * g_fSecPerFrame;
	}

	if (I_Input.m_MouseState[1])
	{
		m_fPitch += XMConvertToRadians(I_Input.m_DIMouseState.lY) * 0.1f;
		m_fYaw   += XMConvertToRadians(I_Input.m_DIMouseState.lX) * 0.1f;
	}

	UpdateQuaternion();
	
	return true;
}

bool Camera::Render()
{
	return true;
}

bool Camera::Release()
{
	Frustum::Release();
	return true;
}

bool Camera::UpdateVector()
{
	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	D3DXVec3Normalize(&m_vLook, &m_vLook);

	return true;
}

bool Camera::UpdateQuaternion()
{

	TQuaternion qRotation;
	// x축행렬과 y축행렬이 결합되어서 새로운 행렬이 만들어진다.
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);	// 단발성이 아니라면 회전이 들어간다 그러면 이 함수를 사용해야한다.
	TMatrix matRotation;
	ZeroMemory(&matRotation, sizeof(matRotation));

	D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &qRotation, &m_vPos);	// 사원수(회전 행렬에만 적용됨)를 행렬로 변환해주는 함수, 서로 상호변환 가능

															// 여기까지 만들어진 것은 오브젝트의 행렬이므로 카메라 행렬로 변환(역행렬)해주어야 한다.
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	// 이걸 뷰행렬로 사용하겠다!
	// 그러므로 SetViewTMatrix에 LookAtLH 함수가 필요가 없어진다.
	// 회전행렬을 만들었으므로 이동행렬을 만들면
	UpdateVector();

	/*m_matView._41 = -m_vPos.Dot(m_vSide);
	m_matView._42 = -m_vPos.Dot(m_vUp);
	m_matView._43 = -m_vPos.Dot(m_vLook);*/

	return true;
}

bool Camera::SetViewTMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
	m_vPos = vPos;
	m_vTarget = vTarget;
	m_vUp = vUp;

	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &m_vUp);

	TQuaternion qRotation;
	TVector3 vTrans, vScale, vAxis;
	TMatrix mInvView;
	TQuaternion q;
	float fAngle;

	if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotation, &vTrans, &m_matView)))
	{
		D3DXQuaternionNormalize(&qRotation, &qRotation);
		D3DXQuaternionToAxisAngle(&qRotation, &vAxis, &fAngle);
		// 반드시 정규화 해야 한다.
		D3DXVec3Normalize(&vAxis, &vAxis);

		q.x = sin(fAngle / 2) * vAxis.x;
		q.y = sin(fAngle / 2) * vAxis.y;
		q.z = sin(fAngle / 2) * vAxis.z;
		q.w = cos(fAngle / 2);
	}
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	TVector3* pZBasis = (TVector3*)&mInvView._31;

	m_fYaw = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fPitch = -atan2f(pZBasis->y, fLen);

	UpdateVector();
	return true;
}

bool Camera::SetProjTMatrix(UINT iWidth, UINT iHeight, float fFov, float fNear, float fFar)
{
	float fAspect = (float)iWidth / (float)iHeight;

	m_fFOV = fFov;
	m_fAspect = fAspect;
	m_fNearPlane = fNear;
	m_fFarPlane = fFar;

	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
	return true;
}

bool Camera::Update()
{
	/*TVector3 vPos = m_vObjectPos;
	vPos -= m_vObjectDir * 10.0f;
	m_vPos.y += 15.0f;
	m_vPos = vPos;
	TMatrixLookAtLH(&m_matView, &m_vPos, &m_vObjectPos, &TVector3(0, 1, 0));*/
	return true;
}

Camera::Camera()
{
	m_fFOV			= 0.0f;
	m_fAspect		= 0.0f;
	m_fNearPlane	= 0.0f;
	m_fFarPlane		= 0.0f;
	m_fPitch		= 0.0f;
	m_fYaw			= 0.0f;
	m_fRoll			= 0.0f;
	m_fSpeed		= 1.0f;
}

Camera::~Camera()
{
}