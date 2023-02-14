#pragma once
#pragma warning(disable:4996)

#include "afxwin.h"
#include "CoreSTD.h"
#include "EffectLoadShader.h"
#include "EffectLoadTexture.h"

#define VERSION 1.0f

// EffectToolForm �� ���Դϴ�.

class EffectToolForm : public CFormView
{
	DECLARE_DYNCREATE(EffectToolForm)
	
protected:
	EffectToolForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~EffectToolForm();
public:
	static EffectToolForm* CreateOne(CWnd* pParent);
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTOOLFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	EffectLoadShader	m_LoadShader;	// ����Ʈ ���̴��ε� ��ü
	EffectLoadTexture	m_LoadTexture;	// ����Ʈ �ؽ�ó�ε� ��ü
public:
	CButton m_EffectCreateBtn;
	// �ٿ���ڽ� �ִ밪 ��ǥ
	float m_fBoxMaxX;
	float m_fBoxMaxY;
	float m_fBoxMaxZ;
	// �ٿ�� �ڽ� �ּҰ� ��ǥ
	float m_fBoxMinX;
	float m_fBoxMinY;
	float m_fBoxMinZ;
	// ����Ʈ ����
	int m_iEffectCnt;
	// ���⺤�� ��ǥ
	float m_fDirX;
	float m_fDirY;
	float m_fDirZ;
	// ����Ʈ �ӵ�
	float m_fEffectSpeed;
	BOOL  m_bRandomSpeed;
	CButton m_RandomSpeed;
	// ����Ʈ ������
	float m_fRadian;
	BOOL m_bRandomRadian;
	CButton m_RandomRadian;
	// ����Ʈ ���� �ð�
	float m_fLifeTime;
	BOOL m_bRandomLifeTime;
	CButton m_RandomLifeTime;
	// ����Ʈ ũ��
	int	 m_iScale;
	BOOL m_bRandomScale;
	CButton m_RandomScale;
	// �ε忩��
	BOOL m_bLoad;
	// �ִ� ��ƼŬ ����
	int m_iMaxParticleCnt;
	// �ִ� ��ƼŬ ������ŭ ���� ����
	CButton m_CreateMaxParticle;
	BOOL m_bCreateMaxParticle;
	// ������ ����Ʈ �ε���
	int m_iDeleteIndex;
	// ���� ����Ʈ �ε���
	int m_iCurrentIndex;
	// ����Ʈ ��ġ
	BOOL m_bRandomPos;
	CButton m_RandomPos;
	// ���̺� ���� �̸�
	CString m_szSaveFileName;
	CString m_szLoadFileName;
	// ���� ��Ʈ��
	FILE* m_pStream;
	// �������������
	CButton m_BillboardApply;
	BOOL m_bBillboardApply;
	CButton m_BillboardNotApply;
	BOOL m_bBillboardNotApply;
	// �������� ����
	CButton m_Perspective;
	BOOL m_bPerspective;
	// �������� ����
	CButton m_Orthogonal;
	BOOL m_bOrthogonal;
	// ��� ���(���� or �ٴ�)
	int m_iPlaneShape;
	// �����ǰ�
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	// �÷���
	float m_fColorRed;
	float m_fColorGreen;
	float m_fColorBlue;
	float m_fColorAlpha;
	CButton m_ColorRandom;
	BOOL m_bColorRandom;
	// ����Ʈ �ݺ� ����
	CButton m_EffectRepeat;
	BOOL m_bEffectRepeat;
	// �����̼� Ÿ��
	int m_iRotationType;
	COORDINATE_TYPE m_eCoordRotationType;
	// �߷� ����
	CButton m_Gravity;
	BOOL m_bGravity;
	// �ٿ�� ����
	CButton m_Bound;
	BOOL m_bBound;
	// �𷺼� ����
	CButton m_RandomDir;
	BOOL m_bRandomDir;

	afx_msg void OnBnClickedCreateEffect();
	afx_msg void OnBnClickedCreateTexture();
	afx_msg void OnBnClickedCreateShader();
	afx_msg void OnBnClickedSaveEffect();
	afx_msg void OnBnClickedLoadEffect();
	afx_msg void OnBnClickedEffectDelete();
	afx_msg void OnBnClickedCreateParticle();
	afx_msg void OnBnClickedEffectDeleteAll();
	afx_msg void OnBnClickedEffectShapeCtrl(UINT msg);
	void SetCircleRotationType();
	void CreateParticle();
	void CreateEffect(ParticleSystem& newParticle, TVector3 vBoundingBoxMin, TVector3 vBoundingBoxMax);
	void ChangeEffectEngineVersion();
	void ResetEffect();
};


