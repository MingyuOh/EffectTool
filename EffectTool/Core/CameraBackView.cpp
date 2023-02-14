#include "CameraBackView.h"
#include "Input.h"

bool CameraBackView::Frame()
{
	if (I_Input.m_MouseState[1])
	{
		m_fPitch += XMConvertToRadians(I_Input.m_DIMouseState.lY) * 0.1f;
		m_fYaw += XMConvertToRadians(I_Input.m_DIMouseState.lX) * 0.1f;
	}
	Update();
	return true;
}

bool CameraBackView::Update()
{
	TVector3 vPos = m_vObjectPos;
	vPos = vPos - (m_vObjectDir * 20.0f);
	vPos.y += 30.0f;
	m_vPos = vPos;

	if (m_bStartCameraPocus)
	{
		m_fPitch += XMConvertToRadians(vPos.y);
		m_bStartCameraPocus = false;
	}

	UpdateQuaternion();
	//TMatrixLookAtLH(&m_matView, &m_vPos, &m_vHeroPos, &TVector3(0, 1, 0));
	return true;
}

bool CameraBackView::UpdateQuaternion()
{
	TQuaternion qRotation;
	TMatrix matRotation;
	// x축행렬과 y축행렬이 결합되어서 새로운 행렬이 만들어진다.
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);	// 단발성이 아니라면 회전이 들어간다 그러면 이 함수를 사용해야한다.
	D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &qRotation, &m_vPos);	// 사원수(회전 행렬에만 적용됨)를 행렬로 변환해주는 함수, 서로 상호변환 가능
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);// 여기까지 만들어진 것은 오브젝트의 행렬이므로 카메라 행렬로 변환(역행렬)해주어야 한다.
	// 이걸 뷰행렬로 사용하겠다!
	// 그러므로 SetViewTMatrix에 LookAtLH 함수가 필요가 없어진다.

	// 회전행렬을 만들었으므로 이동행렬을 만들면
	UpdateVector();

	m_matView._41 = -m_vPos.Dot(m_vSide);
	m_matView._42 = -m_vPos.Dot(m_vUp);
	m_matView._43 = -m_vPos.Dot(m_vLook);

	return true;
}

CameraBackView::CameraBackView()
{
	m_bStartCameraPocus = true;
}

CameraBackView::~CameraBackView()
{
}
