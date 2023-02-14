// EffectToolForm.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "GameTool.h"
#include "EffectToolForm.h"
#include "EffectLoadShader.h"

// EffectToolForm

IMPLEMENT_DYNCREATE(EffectToolForm, CFormView)

EffectToolForm::EffectToolForm()
	: CFormView(IDD_EFFECTTOOLFORM)
	, m_fBoxMaxX(100)
	, m_fBoxMaxY(100)
	, m_fBoxMaxZ(100)
	, m_fBoxMinX(-100)
	, m_fBoxMinY(-100)
	, m_fBoxMinZ(-100)
	, m_fDirX(0)
	, m_fDirY(1)
	, m_fDirZ(0)
	, m_fEffectSpeed(1)
	, m_fRadian(1)
	, m_fLifeTime(1)
	, m_iEffectCnt(1)
	, m_bRandomSpeed(FALSE)
	, m_iScale(1)
	, m_bRandomScale(FALSE)
	, m_bLoad(FALSE)
	, m_iMaxParticleCnt(1)
	, m_iDeleteIndex(0)
	, m_iCurrentIndex(0)
	, m_bRandomPos(FALSE)
	, m_bRandomRadian(FALSE)
	, m_bRandomLifeTime(FALSE)
	, m_bCreateMaxParticle(FALSE)
	, m_szSaveFileName(_T(""))
	, m_szLoadFileName(_T(""))
	, m_pStream(NULL)
	, m_bBillboardApply(FALSE)
	, m_bBillboardNotApply(FALSE)
	, m_bPerspective(TRUE)
	, m_bOrthogonal(FALSE)
	, m_eCoordRotationType(COORDINATE_TYPE::RotationZ)
	, m_iPlaneShape(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fColorRed(1.0f)
	, m_fColorGreen(1.0f)
	, m_fColorBlue(1.0f)
	, m_fColorAlpha(1.0f)
	, m_bColorRandom(FALSE)
	, m_bEffectRepeat(FALSE)
	, m_iRotationType(0)
	, m_bGravity(FALSE)
	, m_bBound(FALSE)
	, m_bRandomDir(FALSE)
{
}

EffectToolForm::~EffectToolForm()
{
	DeleteVector(m_LoadTexture.m_szTextureName);
}

EffectToolForm* EffectToolForm::CreateOne(CWnd* pParent)
{
	EffectToolForm* pForm = new EffectToolForm;
	if (!pForm->Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent, 0, NULL))
	{
		return NULL;
	}
	return pForm;
}

void EffectToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECTCREATE, m_EffectCreateBtn);
	DDX_Text(pDX, IDC_MAX_X, m_fBoxMaxX);
	DDX_Text(pDX, IDC_MAX_Y, m_fBoxMaxY);
	DDX_Text(pDX, IDC_MAX_Z, m_fBoxMaxZ);
	DDX_Text(pDX, IDC_MIN_X, m_fBoxMinX);
	DDX_Text(pDX, IDC_MIN_Y, m_fBoxMinY);
	DDX_Text(pDX, IDC_MIN_Z, m_fBoxMinZ);
	DDX_Text(pDX, IDC_DIR_X, m_fDirX);
	DDX_Text(pDX, IDC_DIR_Y, m_fDirY);
	DDX_Text(pDX, IDC_DIR_Z, m_fDirZ);
	DDX_Text(pDX, IDC_SPEED, m_fEffectSpeed);
	DDX_Text(pDX, IDC_RADIAN, m_fRadian);
	DDX_Text(pDX, IDC_LIFETIME, m_fLifeTime);
	DDX_Text(pDX, IDC_EFFECTCOUNT, m_iEffectCnt);
	DDX_Check(pDX, IDC_RANDOMSPEED, m_bRandomSpeed);
	DDX_Text(pDX, IDC_SCALE, m_iScale);
	DDX_Check(pDX, IDC_RANDOMSCALE, m_bRandomScale);
	DDX_Text(pDX, IDC_MAXPARTICLECOUNT, m_iMaxParticleCnt);
	DDX_Text(pDX, IDC_DELETEINDEX, m_iDeleteIndex);
	DDX_Control(pDX, IDC_RANDOMSPEED, m_RandomSpeed);
	DDX_Control(pDX, IDC_RANDOMSCALE, m_RandomScale);
	DDX_Check(pDX, IDC_RANDOMPOS, m_bRandomPos);
	DDX_Control(pDX, IDC_RANDOMPOS, m_RandomPos);
	DDX_Check(pDX, IDC_RANDOMRADIAN, m_bRandomRadian);
	DDX_Control(pDX, IDC_RANDOMRADIAN, m_RandomRadian);
	DDX_Control(pDX, IDC_RANDOMLIFETIME, m_RandomLifeTime);
	DDX_Check(pDX, IDC_RANDOMLIFETIME, m_bRandomLifeTime);
	DDX_Control(pDX, IDC_RANDOMLIFETIME, m_RandomLifeTime);
	DDX_Check(pDX, IDC_RANDOMLIFETIME, m_bRandomLifeTime);
	DDX_Control(pDX, IDC_CREATEMAXPARTICLE, m_CreateMaxParticle);
	DDX_Check(pDX, IDC_CREATEMAXPARTICLE, m_bCreateMaxParticle);
	DDX_Text(pDX, IDC_SAVE_FILENAME, m_szSaveFileName);
	DDX_Check(pDX, IDC_CHECK_Billboard_Apply, m_bBillboardApply);
	DDX_Check(pDX, IDC_CHECK_BILLBOARD_NOT_APPLY, m_bBillboardNotApply);
	DDX_Check(pDX, IDC_CHECK_PERSPECTIVE, m_bPerspective);
	DDX_Check(pDX, IDC_CHECK_ORTHOGONAL, m_bOrthogonal);
	DDX_Control(pDX, IDC_CHECK_Billboard_Apply, m_BillboardApply);
	DDX_Control(pDX, IDC_CHECK_BILLBOARD_NOT_APPLY, m_BillboardNotApply);
	DDX_Control(pDX, IDC_CHECK_PERSPECTIVE, m_Perspective);
	DDX_Control(pDX, IDC_CHECK_ORTHOGONAL, m_Orthogonal);
	DDX_Radio(pDX, IDC_STANDARD_PLANE, m_iPlaneShape);
	DDX_Text(pDX, IDC_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_POS_Z, m_fPosZ);
	DDX_Text(pDX, IDC_CORLOR_R, m_fColorRed);
	DDX_Text(pDX, IDC_CORLOR_G, m_fColorGreen);
	DDX_Text(pDX, IDC_CORLOR_B, m_fColorBlue);
	DDX_Text(pDX, IDC_CORLOR_A, m_fColorAlpha);
	DDX_Control(pDX, IDC_RANDOM_COLOR, m_ColorRandom);
	DDX_Check(pDX, IDC_RANDOM_COLOR, m_bColorRandom);
	DDX_Control(pDX, IDC_EFFECT_REPEAT, m_EffectRepeat);
	DDX_Check(pDX, IDC_EFFECT_REPEAT, m_bEffectRepeat);
	DDX_Radio(pDX, IDC_ROTATIONAXIS_NONE, m_iRotationType);
	DDX_Control(pDX, IDC_GRAVITY, m_Gravity);
	DDX_Check(pDX, IDC_GRAVITY, m_bGravity);
	DDX_Control(pDX, IDC_BOUND, m_Bound);
	DDX_Check(pDX, IDC_BOUND, m_bBound);
	DDX_Control(pDX, IDC_RANDOMDIR, m_RandomDir);
	DDX_Check(pDX, IDC_RANDOMDIR, m_bRandomDir);
}

BEGIN_MESSAGE_MAP(EffectToolForm, CFormView)
	ON_BN_CLICKED(IDC_EFFECTCREATE, &EffectToolForm::OnBnClickedCreateEffect)
	ON_BN_CLICKED(IDC_CREATETEXTURE, &EffectToolForm::OnBnClickedCreateTexture)
	ON_BN_CLICKED(IDC_CREATESHADER, &EffectToolForm::OnBnClickedCreateShader)
	ON_BN_CLICKED(IDC_SAVE_EFFECT, &EffectToolForm::OnBnClickedSaveEffect)
	ON_BN_CLICKED(IDC_LOAD_EFFECT, &EffectToolForm::OnBnClickedLoadEffect)
	ON_BN_CLICKED(IDC_EFFECTDELETE, &EffectToolForm::OnBnClickedEffectDelete)
	ON_BN_CLICKED(IDC_CREATEPARTICLE, &EffectToolForm::OnBnClickedCreateParticle)
	ON_BN_CLICKED(IDC_EFFECTDELETEALL, &EffectToolForm::OnBnClickedEffectDeleteAll)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_Billboard_Apply, IDC_FLOOR_PLANE, OnBnClickedEffectShapeCtrl)
END_MESSAGE_MAP()


// EffectToolForm 진단입니다.

#ifdef _DEBUG
void EffectToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void EffectToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// EffectToolForm 메시지 처리기입니다.

void EffectToolForm::OnBnClickedCreateEffect()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 예외처리
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 재생성
	if (m_iMaxParticleCnt <= ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size())
	{
		OnBnClickedEffectDeleteAll();
	}

	// 데이터 업데이트
	// 새로운 이펙트 생성시 시간 초기화
	if (m_bLoad == FALSE)
	{
		g_fElapseTime = 0.0f;
		UpdateData(TRUE);
	}

	// 이펙트 개수가 0개이하이면 생성하지 않는다.
	if (m_iEffectCnt <= 0)
		return;

	// 텍스처 이름이 없을 경우
	if (m_LoadTexture.m_szTextureName.size() == 0)
	{
		AfxMessageBox(L"텍스처가 존재하지 않습니다.\n텍스처를 생성해주세요.");
		return;
	}

	// 셰이더 이름이 없을 경우
	if (m_LoadShader.m_pszShaderName == NULL)
	{
		AfxMessageBox(L"셰이더가 존재하지 않습니다.\n셰이더를 생성해주세요.");
		return;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// 한번에 파티클을 생성
	if (m_bCreateMaxParticle == TRUE)
	{
		int iCreateParticle = 0;
		// 이미 생성된 파티클이 존재한다면, 유지하고  현재 파티클 개수부터 최대개수만큼만 생성
		if (((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount > 0)
		{
			iCreateParticle = m_iMaxParticleCnt - ((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount;
		}
		else
		{
			iCreateParticle = m_iMaxParticleCnt;
		}
		for (int iParticle = 0; iParticle < iCreateParticle; iParticle++)
		{
			CreateParticle();
		}
	}
	else
	{
		CreateParticle();
	}
}

void EffectToolForm::CreateEffect(ParticleSystem& newParticle, TVector3 vBoundingBoxMin, TVector3 vBoundingBoxMax)
{
	newParticle.m_iEffectCount = m_iEffectCnt;

	// 블랜딩 타입
	newParticle.m_bAlphaBlending = m_LoadTexture.m_bAlphaBlend;
	newParticle.m_bAlphaTesting = m_LoadTexture.m_bAlphaTest;

	// 이펙트 셋팅
	newParticle.m_Effect.Init();

	// 텍스처 정보 저장
	for (int iCnt = 0; iCnt < m_LoadTexture.m_szTextureName.size(); iCnt++)
	{
		newParticle.m_Effect.LoadTexture(m_LoadTexture.m_szTextureName[iCnt]);
	}
	newParticle.m_Effect.m_iVertexType = m_iPlaneShape;

	// apply to tool
	newParticle.m_Effect.Create(
		_T(""),
		m_LoadShader.m_pszShaderName,
		m_LoadShader.m_pszShaderName);

	// Create ParticleSystem 
	newParticle.Set(((CGameToolApp*)AfxGetApp())->m_Main.m_pMainCamera);
	newParticle.Create();
	newParticle.Init();

	// Save Matrix set
	newParticle.m_iRow = m_LoadTexture.m_iRow;
	newParticle.m_iCol = m_LoadTexture.m_iCol;

	// LifeTime
	newParticle.m_fLifeTime = m_fLifeTime;

	// Rotation Type
	if (m_iRotationType != 0)
	{
		newParticle.m_EffectType = EFFECT_TYPE::Rotate;
		// If Circle Exercise, Call the Function Determine Circle Rotation Type
		SetCircleRotationType();
		newParticle.m_CoordinateType = m_eCoordRotationType;
	}
	else
	{
		newParticle.m_EffectType = EFFECT_TYPE::Straight;
	}

	// Billboard Matrix
	if (m_bBillboardNotApply == TRUE)
	{
		m_bBillboardApply = FALSE;
		newParticle.m_bBillboard = FALSE;
	}
	else
	{
		m_bBillboardApply = TRUE;
		newParticle.m_bBillboard = TRUE;
	}

	// 이펙트 반복 여부
	newParticle.m_bRepeatEffect = m_bEffectRepeat;

	// 원근투영 / 직교투영
	if (m_bPerspective == TRUE)
	{
		newParticle.m_bPerspective = true;
	}
	else
	{
		newParticle.m_bPerspective = false;
	}

	if (m_bOrthogonal == TRUE)
	{
		newParticle.m_bOrthogonal = true;
	}
	else
	{
		newParticle.m_bOrthogonal = false;
	}

	newParticle.CreateBoundingBox(vBoundingBoxMin, vBoundingBoxMax);
}

void EffectToolForm::CreateParticle()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 이펙트 생성 시작
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ParticleSystem newParticle;
	CreateEffect(newParticle,
		TVector3(m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ),
		TVector3(m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ));

	// 이펙트 한개한개의 정보
	for (int iEffect = 0; iEffect < m_iEffectCnt; iEffect++)
	{
		// 이펙트 원점(바운딩박스 원점) 갱신
		if (m_bRandomPos == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vPos = TVector3(randstep(m_fBoxMinX + m_fPosX, m_fBoxMaxX), randstep(m_fBoxMinY + m_fPosY, m_fBoxMaxY), randstep(m_fBoxMinZ + m_fPosZ, m_fBoxMaxZ));
			// 처음 위치 저장
			newParticle.m_Particle[iEffect].m_vFirstPos = newParticle.m_Particle[iEffect].m_vPos;
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vPos = TVector3(m_fPosX, m_fPosY, m_fPosZ);
			// 처음 위치 저장
			newParticle.m_Particle[iEffect].m_vFirstPos = newParticle.m_Particle[iEffect].m_vPos;
		}
		newParticle.m_Particle[iEffect].m_vCenter = newParticle.m_BoundingBoxShape.m_BoundingBox.vCenter;

		// 이펙트 컬러값 설정
		if (m_bColorRandom == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vColor = TVector4(randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), randstep(0.0f, 1.0f));
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vColor = TVector4(m_fColorRed, m_fColorGreen, m_fColorBlue, m_fColorAlpha);
		}

		// 이펙트 방향벡터 설정
		newParticle.m_Particle[iEffect].m_vDirection = TVector3(m_fDirX, m_fDirY, m_fDirZ);

		// 반지름 설정
		if (m_bRandomRadian == TRUE)
			newParticle.m_Particle[iEffect].m_fRadian = randstep(1.0f, m_fRadian);
		else
			newParticle.m_Particle[iEffect].m_fRadian = m_fRadian;

		// 라이프타임 설정
		if (m_bRandomLifeTime == TRUE)
			newParticle.m_fLifeTime = randstep(1.0f, m_fLifeTime);
		else
			newParticle.m_fLifeTime = m_fLifeTime;

		// 이펙트 크기 설정
		if (m_bRandomScale == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vScale = TVector3(
				randstep(1.0f, m_iScale),
				randstep(1.0f, m_iScale),
				randstep(1.0f, m_iScale));
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vScale = TVector3((FLOAT)m_iScale, (FLOAT)m_iScale, (FLOAT)m_iScale);
		}

		// 이펙트 속도 랜덤 유무
		if (m_bRandomSpeed == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vSpeed = TVector3(
				randstep(1.0f, m_fEffectSpeed),
				randstep(1.0f, m_fEffectSpeed),
				randstep(1.0f, m_fEffectSpeed));
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vSpeed = TVector3(m_fEffectSpeed, m_fEffectSpeed, m_fEffectSpeed);
		}
	}
	// 파티클 생성
	((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.push_back(newParticle);
	((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount++;

	// 파티클 수 2개 이상이면 이전 생성된 이펙트 위치 초기화
	ResetEffect();
}

void EffectToolForm::ResetEffect()
{
	// 파티클 개수가 2개 이상이면 위치 초기화
	if (m_iMaxParticleCnt > 1)
	{
		int iEffectSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size() - 1;
		for (int iEffect = 0; iEffect < iEffectSize; iEffect++)
		{
			int iParticleSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iEffect].m_Particle.size();
			for (int iParticle = 0; iParticle < iParticleSize; iParticle++)
			{
				// 위치 초기화
				((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iEffect].m_Particle[iParticle].m_vPos
					= ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iEffect].m_Particle[iParticle].m_vFirstPos;
			}
		}
	}
}

void EffectToolForm::OnBnClickedEffectDelete()
{
	if(m_bLoad == FALSE)
		UpdateData(TRUE);

	if (m_iDeleteIndex >= ((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount)
	{
		AfxMessageBox(L"존재하지 않는 인덱스입니다.");
		return;
	}
	((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.erase(((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.begin() + m_iDeleteIndex);
	((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount--;

	CString msg;
	msg.Format(_T("%d번째 이펙트 삭제"), m_iDeleteIndex);
	AfxMessageBox(msg);
}


void EffectToolForm::OnBnClickedCreateTexture()
{
	m_LoadTexture.DoModal();
}


void EffectToolForm::OnBnClickedCreateShader()
{
	m_LoadShader.DoModal();
}

void EffectToolForm::OnBnClickedSaveEffect()
{
	UpdateData(TRUE);

	if (m_szSaveFileName.Right(4) != ".txt")
	{
		m_szSaveFileName += ".txt";
	}
	TCHAR szFileName[256];
	_tcscpy_s(szFileName, L"../data/Object/");
	TCHAR* pszFileName = (TCHAR*)(LPCTSTR)m_szSaveFileName;
	_tcscat_s(szFileName, pszFileName);
	_tfopen_s(&m_pStream, szFileName, _T("wt"));

	if (m_pStream == NULL)
	{
		return;
	}

	if (((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size() != 0)
	{
		TCHAR str[256];
		// 현재 버전 읽기
		_ftprintf_s(m_pStream, _T("*VERSION* %f\n"), VERSION);

		// 할당된 파티클 개수
		_ftprintf_s(m_pStream, _T("%d\n"), m_iMaxParticleCnt);

		// 최대 파티클 개수 생성 유무
		_ftprintf_s(m_pStream, _T("%d\n"), m_bCreateMaxParticle);

		// 바운딩박스
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ);// 최소값
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ);// 최대값

		for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
		{
			// 이펙트 정보
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_iEffectCount); // 이펙트 개수

			// 로테이션 타입
			if (m_iRotationType != 0)
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 1);
				_ftprintf_s(m_pStream, _T("%d\n"), m_eCoordRotationType);
			}
			else
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 0);
			}

			// 위치 랜덤 유무
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomPos);

			// 속도 랜덤 유무
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomSpeed);

			// 이펙트 속도
			_ftprintf_s(m_pStream, _T("%f\n"), m_fEffectSpeed);

			// 반지름 랜덤 유무
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomRadian);

			// 반지름
			_ftprintf_s(m_pStream, _T("%f\n"), m_fRadian);

			// LifeTime 랜덤 유무
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomLifeTime);

			// Life Time
			_ftprintf_s(m_pStream, _T("%f\n"), m_fLifeTime);

			// 크기 랜덤 유무
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomScale);

			// 이펙트 크기
			_ftprintf_s(m_pStream, _T("%d\n"), m_iScale);

			// 행렬 값
			_ftprintf_s(m_pStream, _T("%d %d\n"), m_LoadTexture.m_iRow, m_LoadTexture.m_iCol);

			// 알파 블렌딩
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaBlend);

			// 알파 테스팅
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaTest);

			// 경과 시간
			_ftprintf_s(m_pStream, _T("%f\n"), g_fElapseTime);

			// 빌보드행렬
			_ftprintf_s(m_pStream, _T("%d\n"), m_bBillboardApply);
			if (m_bBillboardNotApply == true)
			{
				// 이펙트 버텍스 타입(정면을 보는지 바닥에 깔리는지 판단하는 이펙트)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);
			}

			// 이펙트 버텍스 타입
			if(m_bBillboardApply == false)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);

			// 이펙트 반복 여부
			_ftprintf_s(m_pStream, _T("%d\n"), m_bEffectRepeat);

			// 원근투영
			_ftprintf_s(m_pStream, _T("%d\n"), m_bPerspective);

			// 직교투영
			_ftprintf_s(m_pStream, _T("%d\n"), m_bOrthogonal);

			// 텍스처 개수
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureSRV.size());

			// 텍스처명
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size(); iCnt++)
			{
				_ftprintf_s(m_pStream, _T("%s\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList[iCnt].c_str());

			}

			// 셰이더
			_ftprintf_s(m_pStream, _T("%s\n"), m_LoadShader.m_pszShaderName);

			// 파티클
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle.size(); iCnt++)
			{
				// 이펙트 위치
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.z);

				// 반복이펙트이면 처음위치 저장
				if (m_bEffectRepeat == TRUE)
				{
					_ftprintf_s(m_pStream, _T("%f %f %f\n"),
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.x,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.y,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.z);
				}

				// 이펙트 컬러
				_ftprintf_s(m_pStream, _T("%f %f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.z,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.w);

				// 방향벡터
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.z);

				// 이펙트 크기
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.z);

				// 이펙트 속도
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.z);

				// 반지름
				_ftprintf_s(m_pStream, _T("%f\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_fRadian);
			}
		}
		fclose(m_pStream);
		// 엔진용 txt파일 세이브
		ChangeEffectEngineVersion();
	}
	else
	{
		AfxMessageBox(L"이펙트가 생성되지 않았습니다.");
	}
}

void EffectToolForm::OnBnClickedLoadEffect()
{
	UpdateData(TRUE);

	if (m_szSaveFileName.Right(4) != ".txt")
	{
		m_szSaveFileName += ".txt";
	}

	TCHAR szFileName[256];
	_tcscpy_s(szFileName, L"../data/Object/");
	TCHAR* pszFileName = (TCHAR*)(LPCTSTR)m_szSaveFileName;
	_tcscat_s(szFileName, pszFileName);
	_tfopen_s(&m_pStream, szFileName, _T("rt"));

	if (m_pStream == NULL)
	{
		AfxMessageBox(L"불러오기 실패");
		return;
	}

	int iParticleSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size();

	if (iParticleSize > 0)
	{
		for (int iCnt = 0; iCnt < iParticleSize; iCnt++)
			((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.erase(((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.begin());
	}

	// 이전에 생성된 텍스처가 남아있다면 초기화
	if (m_LoadTexture.m_iTextureCnt > 0)
		m_LoadTexture.ReSetTextureData();

	TCHAR str[256];
	TCHAR tmpStr[256];
	float fVersion = 0.0f;
	int	  iBoolValueRead;

	// 현재 버전 읽기
	_fgetts(str, 256, m_pStream);
	_stscanf(str, _T("%s %f"), tmpStr, &fVersion);

	if (fVersion <= VERSION - EPSILON || fVersion >= VERSION + EPSILON)
	{
		AfxMessageBox(L"기존버전보다 낮거나 높은 버전의 이펙트입니다.");
		return;
	}

	// 최대 파티클 개수
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%d"), &m_iMaxParticleCnt);
	// 최대 파티클 개수만큼 생성 유무
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%d"), &iBoolValueRead);
	m_bCreateMaxParticle = (BOOL)iBoolValueRead;

	// 바운딩박스
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%f %f %f"), &m_fBoxMinX, &m_fBoxMinY, &m_fBoxMinZ);
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%f %f %f"), &m_fBoxMaxX, &m_fBoxMaxY, &m_fBoxMaxZ);

	for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
	{
		// 이펙트 수
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iEffectCnt);

		// 이펙트 타입
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iRotationType);
		
		// 위치 랜덤 유무
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomPos = (BOOL)iBoolValueRead;

		// 속도 랜덤 유무
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomSpeed = (BOOL)iBoolValueRead;

		// 이펙트 속도
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fEffectSpeed);

		// 반지름 랜덤 유무
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomRadian = (BOOL)iBoolValueRead;

		// 반지름
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fRadian);

		// LifeTime 랜덤 유무
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomLifeTime = (BOOL)iBoolValueRead;

		// Life Time
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fLifeTime);

		// 크기 랜덤 유무
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomScale = (BOOL)iBoolValueRead;

		// 이펙트 크기
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iScale);

		// 행렬 값
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d %d"), &m_LoadTexture.m_iRow, &m_LoadTexture.m_iCol);

		// 알파 블렌딩
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_LoadTexture.m_bAlphaBlend = (BOOL)iBoolValueRead;

		// 알파 테스팅
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_LoadTexture.m_bAlphaTest = (BOOL)iBoolValueRead;

		// 경과시간
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &g_fElapseTime);

		// 빌보드행렬
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bBillboardApply = (BOOL)iBoolValueRead;

		if (m_bBillboardApply == false)
		{
			// 이펙트 버텍스 타입(정면을 보는지 바닥에 깔리는지 판단하는 이펙트)
			m_bBillboardNotApply == true;
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%d"), &m_iPlaneShape);
		}

		// 이펙트 반복 여부
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bEffectRepeat = (BOOL)iBoolValueRead;

		// 원근투영
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bPerspective = (BOOL)iBoolValueRead;

		// 직교투영
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bOrthogonal = (BOOL)iBoolValueRead;

		// 텍스처 수
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_LoadTexture.m_iTextureCnt);

		// 텍스처 이름
		for (int iCnt = 0; iCnt < m_LoadTexture.m_iTextureCnt; iCnt++)
		{
			CString szTextureName;
			_fgetts(str, 256, m_pStream);
			_stscanf(str, _T("%s"), tmpStr);
			szTextureName = tmpStr;
			m_LoadTexture.m_szTextureName.push_back((TCHAR*)(LPCTSTR)szTextureName);
		}

		// 셰이더
		CString szShaderName;
		_fgetts(str, 256, m_pStream);
		_stscanf(str, _T("%s"), tmpStr);
		szShaderName = tmpStr;
		m_LoadShader.CreateShaderFileName(szShaderName);

		// 로드 성공
		m_bLoad = TRUE;

		// 이펙트 생성
		ParticleSystem newParticle;
		CreateEffect(newParticle,
			TVector3(m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ),
			TVector3(m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ));

		// 이펙트 요소
		for (int iCnt = 0; iCnt < m_iEffectCnt; iCnt++)
		{
			// 이펙트 위치
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"), 
				&newParticle.m_Particle[iCnt].m_vPos.x,
				&newParticle.m_Particle[iCnt].m_vPos.y,
				&newParticle.m_Particle[iCnt].m_vPos.z);

			// 반복 이펙트이면 처음위치 저장
			if (newParticle.m_bRepeatEffect)
			{
				_fgetts(str, 256, m_pStream);
				_stscanf_s(str, _T("%f %f %f"),
					&newParticle.m_Particle[iCnt].m_vFirstPos.x,
					&newParticle.m_Particle[iCnt].m_vFirstPos.y,
					&newParticle.m_Particle[iCnt].m_vFirstPos.z);
			}

			// 이펙트 컬러
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vColor.x,
				&newParticle.m_Particle[iCnt].m_vColor.y,
				&newParticle.m_Particle[iCnt].m_vColor.z,
				&newParticle.m_Particle[iCnt].m_vColor.w);

			// 방향벡터
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vDirection.x,
				&newParticle.m_Particle[iCnt].m_vDirection.y,
				&newParticle.m_Particle[iCnt].m_vDirection.z);
			m_fDirX = newParticle.m_Particle[iCnt].m_vDirection.x;
			m_fDirY = newParticle.m_Particle[iCnt].m_vDirection.y;
			m_fDirZ = newParticle.m_Particle[iCnt].m_vDirection.z;

			// 이펙트 크기
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vScale.x,
				&newParticle.m_Particle[iCnt].m_vScale.y,
				&newParticle.m_Particle[iCnt].m_vScale.z);

			// 이펙트 속도
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vSpeed.x,
				&newParticle.m_Particle[iCnt].m_vSpeed.y,
				&newParticle.m_Particle[iCnt].m_vSpeed.z);

			// 반지름
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f"), &newParticle.m_Particle[iCnt].m_fRadian);
		}
		// 파티클 생성
		((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.push_back(newParticle);
		((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount++;

		// 폼에 불러온 값 적용
		SetDlgItemInt(IDC_MIN_X, m_fBoxMinX);
		SetDlgItemInt(IDC_MIN_Y, m_fBoxMinY);
		SetDlgItemInt(IDC_MIN_Z, m_fBoxMinZ);
		SetDlgItemInt(IDC_MAX_X, m_fBoxMaxX);
		SetDlgItemInt(IDC_MAX_Y, m_fBoxMaxY);
		SetDlgItemInt(IDC_MAX_Z, m_fBoxMaxZ);
		SetDlgItemInt(IDC_MAXPARTICLECOUNT, m_iMaxParticleCnt);
		m_CreateMaxParticle.SetCheck(m_bCreateMaxParticle);
		SetDlgItemInt(IDC_EFFECTCOUNT, m_iEffectCnt);
		m_RandomPos.SetCheck(m_bRandomPos);
		SetDlgItemInt(IDC_DIR_X, m_fDirX);
		SetDlgItemInt(IDC_DIR_Y, m_fDirY);
		SetDlgItemInt(IDC_DIR_Z, m_fDirZ);
		SetDlgItemInt(IDC_SPEED, m_fEffectSpeed);
		m_RandomSpeed.SetCheck(m_bRandomSpeed);
		SetDlgItemInt(IDC_RADIAN, m_fRadian);
		m_RandomRadian.SetCheck(m_bRandomRadian);
		SetDlgItemInt(IDC_LIFETIME, m_fLifeTime);
		m_RandomLifeTime.SetCheck(m_bRandomLifeTime);
		SetDlgItemInt(IDC_SCALE, m_iScale);
		m_RandomScale.SetCheck(m_bRandomScale);
		SetDlgItemInt(IDC_DELETEINDEX, m_iMaxParticleCnt - 1);
		m_BillboardApply.SetCheck(m_bBillboardApply);
		m_BillboardNotApply.SetCheck(m_bBillboardNotApply);
		m_EffectRepeat.SetCheck(m_bEffectRepeat);
		m_Perspective.SetCheck(m_bPerspective);
		m_Orthogonal.SetCheck(m_bOrthogonal);
		m_Gravity.SetCheck(m_bGravity);
		m_Bound.SetCheck(m_bBound);
		m_RandomDir.SetCheck(m_bRandomDir);

			

		// 텍스트 이름이 존재하면 지운다.
		if (m_LoadTexture.m_szTextureName.size() != 0)
		{
			DeleteVector(m_LoadTexture.m_szTextureName);
			m_LoadTexture.m_iTextureCnt = 0;
		}
	}

	m_bLoad = FALSE;
	fclose(m_pStream);
}


// 최대 파티클 수 제한 함수
void EffectToolForm::OnBnClickedCreateParticle()
{
	if(m_bLoad == FALSE)
		UpdateData(TRUE);
}

void EffectToolForm::OnBnClickedEffectDeleteAll()
{
	if (((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size() > 0)
	{
		for (int iDel = 0; iDel < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size(); iDel++)
			((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iDel].Release();
		DeleteVector(((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem);
		((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount = 0;
	}
}

void EffectToolForm::ChangeEffectEngineVersion()
{
	CString Filter = L".txt";
	CString szSaveFileNameForEngine;
	if (m_szSaveFileName.Right(4) == ".txt")
	{
		szSaveFileNameForEngine = m_szSaveFileName.Left(m_szSaveFileName.GetLength() - Filter.GetLength());
		szSaveFileNameForEngine += "_Engine.txt";
	}
	TCHAR szFileName[256];
	_tcscpy_s(szFileName, L"../data/Object/");
	TCHAR* pszFileName = (TCHAR*)(LPCTSTR)szSaveFileNameForEngine;
	_tcscat_s(szFileName, pszFileName);
	_tfopen_s(&m_pStream, szFileName, _T("wt"));

	if (m_pStream == NULL)
	{
		return;
	}

	if (((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size() != 0)
	{
		// text내용 입력
		TCHAR str[256];

		// 할당된 파티클 개수
		_ftprintf_s(m_pStream, _T("%d\n"), m_iMaxParticleCnt);

		// 바운딩박스
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ); // 최소값
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ); // 최대값

		for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
		{
			// 이펙트 개수
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_iEffectCount); // 이펙트 개수

			// 로테이션 타입
			if (m_iRotationType != 0)
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 1);
				_ftprintf_s(m_pStream, _T("%d\n"), m_eCoordRotationType);
			}
			else
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 0);
			}

			// Life Time
			_ftprintf_s(m_pStream, _T("%f\n"), m_fLifeTime);

			// 행렬 값
			_ftprintf_s(m_pStream, _T("%d %d\n"), m_LoadTexture.m_iRow, m_LoadTexture.m_iCol);

			// 알파 블렌딩
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaBlend);

			// 알파 테스팅
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaTest);

			// 경과 시간
			_ftprintf_s(m_pStream, _T("%f\n"), g_fElapseTime);

			// 빌보드행렬
			_ftprintf_s(m_pStream, _T("%d\n"), m_bBillboardApply);

			if (m_bBillboardApply == false)
			{
				// 이펙트 버텍스 타입(정면을 보는지 바닥에 깔리는지 판단하는 이펙트)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);
			}

			// 이펙트 반복 여부
			_ftprintf_s(m_pStream, _T("%d\n"), m_bEffectRepeat);

			// 원근투영
			_ftprintf_s(m_pStream, _T("%d\n"), m_bPerspective);

			// 직교투영
			_ftprintf_s(m_pStream, _T("%d\n"), m_bOrthogonal);

			// 텍스처 개수
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size());

			// 텍스처명
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size(); iCnt++)
			{
				TCHAR dir[256] = L"../";
				_tcscat_s(dir, ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList[iCnt].c_str());
				_ftprintf_s(m_pStream, _T("%s\n"), dir);
			}

			// 셰이더
			TCHAR dir[256] = L"../";
			_tcscat_s(dir, m_LoadShader.m_pszShaderName);
			_ftprintf_s(m_pStream, _T("%s\n"), dir);

			// 파티클
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle.size(); iCnt++)
			{
				// 이펙트 위치
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.z);

				// 반복이펙트이면 처음위치 저장
				if (m_bEffectRepeat == TRUE)
				{
					_ftprintf_s(m_pStream, _T("%f %f %f\n"),
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.x,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.y,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.z);
				}

				// 이펙트 컬러
				_ftprintf_s(m_pStream, _T("%f %f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.z,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.w);

				// 방향벡터
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.z);

				// 이펙트 크기
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.z);

				// 이펙트 속도
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.z);

				// 반지름
				_ftprintf_s(m_pStream, _T("%f\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_fRadian);
			}
		}
		fclose(m_pStream);
	}
	else
	{
		AfxMessageBox(L"이펙트가 생성되지 않았습니다.");
	}
}

void EffectToolForm::OnBnClickedEffectShapeCtrl(UINT msg)
{
	UpdateData(TRUE);
	
	if (m_bBillboardApply == true)
	{
		GetDlgItem(IDC_CHECK_Billboard_Apply)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BILLBOARD_NOT_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_STANDARD_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FLOOR_PLANE)->EnableWindow(FALSE);
	}
	else
	{
		if (m_bBillboardNotApply == true)
		{
			GetDlgItem(IDC_CHECK_Billboard_Apply)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_BILLBOARD_NOT_APPLY)->EnableWindow(TRUE);
			GetDlgItem(IDC_STANDARD_PLANE)->EnableWindow(TRUE);
			GetDlgItem(IDC_FLOOR_PLANE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_Billboard_Apply)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_BILLBOARD_NOT_APPLY)->EnableWindow(TRUE);
			GetDlgItem(IDC_STANDARD_PLANE)->EnableWindow(FALSE);
			GetDlgItem(IDC_FLOOR_PLANE)->EnableWindow(FALSE);
		}
	}
}

void EffectToolForm::SetCircleRotationType()
{
	if(m_bLoad == FALSE)
		UpdateData(TRUE);
	
	m_eCoordRotationType = COORDINATE_TYPE::RotationZ;

	if (m_iRotationType == 1)
		m_eCoordRotationType = COORDINATE_TYPE::RotationX;
	else if (m_iRotationType == 2)
		m_eCoordRotationType = COORDINATE_TYPE::RotationY;
	else
		m_eCoordRotationType = COORDINATE_TYPE::RotationZ;
}
