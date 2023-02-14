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
	// x����İ� y������� ���յǾ ���ο� ����� ���������.
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);	// �ܹ߼��� �ƴ϶�� ȸ���� ���� �׷��� �� �Լ��� ����ؾ��Ѵ�.
	D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &qRotation, &m_vPos);	// �����(ȸ�� ��Ŀ��� �����)�� ��ķ� ��ȯ���ִ� �Լ�, ���� ��ȣ��ȯ ����
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);// ������� ������� ���� ������Ʈ�� ����̹Ƿ� ī�޶� ��ķ� ��ȯ(�����)���־�� �Ѵ�.
	// �̰� ����ķ� ����ϰڴ�!
	// �׷��Ƿ� SetViewTMatrix�� LookAtLH �Լ��� �ʿ䰡 ��������.

	// ȸ������� ��������Ƿ� �̵������ �����
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
