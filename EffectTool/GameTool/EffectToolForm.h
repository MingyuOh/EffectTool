#pragma once
#pragma warning(disable:4996)

#include "afxwin.h"
#include "CoreSTD.h"
#include "EffectLoadShader.h"
#include "EffectLoadTexture.h"

#define VERSION 1.0f

// EffectToolForm 폼 뷰입니다.

class EffectToolForm : public CFormView
{
	DECLARE_DYNCREATE(EffectToolForm)
	
protected:
	EffectToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	EffectLoadShader	m_LoadShader;	// 이펙트 셰이더로드 객체
	EffectLoadTexture	m_LoadTexture;	// 이펙트 텍스처로드 객체
public:
	CButton m_EffectCreateBtn;
	// 바운딩박스 최대값 좌표
	float m_fBoxMaxX;
	float m_fBoxMaxY;
	float m_fBoxMaxZ;
	// 바운딩 박스 최소값 좌표
	float m_fBoxMinX;
	float m_fBoxMinY;
	float m_fBoxMinZ;
	// 이펙트 개수
	int m_iEffectCnt;
	// 방향벡터 좌표
	float m_fDirX;
	float m_fDirY;
	float m_fDirZ;
	// 이펙트 속도
	float m_fEffectSpeed;
	BOOL  m_bRandomSpeed;
	CButton m_RandomSpeed;
	// 이펙트 반지름
	float m_fRadian;
	BOOL m_bRandomRadian;
	CButton m_RandomRadian;
	// 이펙트 생성 시간
	float m_fLifeTime;
	BOOL m_bRandomLifeTime;
	CButton m_RandomLifeTime;
	// 이펙트 크기
	int	 m_iScale;
	BOOL m_bRandomScale;
	CButton m_RandomScale;
	// 로드여부
	BOOL m_bLoad;
	// 최대 파티클 개수
	int m_iMaxParticleCnt;
	// 최대 파티클 개수만큼 생성 여부
	CButton m_CreateMaxParticle;
	BOOL m_bCreateMaxParticle;
	// 삭제할 이펙트 인덱스
	int m_iDeleteIndex;
	// 현제 이펙트 인덱스
	int m_iCurrentIndex;
	// 이펙트 위치
	BOOL m_bRandomPos;
	CButton m_RandomPos;
	// 세이브 파일 이름
	CString m_szSaveFileName;
	CString m_szLoadFileName;
	// 파일 스트림
	FILE* m_pStream;
	// 빌보드행렬적용
	CButton m_BillboardApply;
	BOOL m_bBillboardApply;
	CButton m_BillboardNotApply;
	BOOL m_bBillboardNotApply;
	// 원근투영 적용
	CButton m_Perspective;
	BOOL m_bPerspective;
	// 직교투영 적용
	CButton m_Orthogonal;
	BOOL m_bOrthogonal;
	// 평면 모양(정면 or 바닥)
	int m_iPlaneShape;
	// 포지션값
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	// 컬러값
	float m_fColorRed;
	float m_fColorGreen;
	float m_fColorBlue;
	float m_fColorAlpha;
	CButton m_ColorRandom;
	BOOL m_bColorRandom;
	// 이펙트 반복 여부
	CButton m_EffectRepeat;
	BOOL m_bEffectRepeat;
	// 로테이션 타입
	int m_iRotationType;
	COORDINATE_TYPE m_eCoordRotationType;
	// 중력 적용
	CButton m_Gravity;
	BOOL m_bGravity;
	// 바운드 적용
	CButton m_Bound;
	BOOL m_bBound;
	// 디렉션 랜덤
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


