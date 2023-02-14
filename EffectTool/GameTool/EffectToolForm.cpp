// EffectToolForm.cpp : ���� �����Դϴ�.
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


// EffectToolForm �����Դϴ�.

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


// EffectToolForm �޽��� ó�����Դϴ�.

void EffectToolForm::OnBnClickedCreateEffect()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ó��
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// �����
	if (m_iMaxParticleCnt <= ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size())
	{
		OnBnClickedEffectDeleteAll();
	}

	// ������ ������Ʈ
	// ���ο� ����Ʈ ������ �ð� �ʱ�ȭ
	if (m_bLoad == FALSE)
	{
		g_fElapseTime = 0.0f;
		UpdateData(TRUE);
	}

	// ����Ʈ ������ 0�������̸� �������� �ʴ´�.
	if (m_iEffectCnt <= 0)
		return;

	// �ؽ�ó �̸��� ���� ���
	if (m_LoadTexture.m_szTextureName.size() == 0)
	{
		AfxMessageBox(L"�ؽ�ó�� �������� �ʽ��ϴ�.\n�ؽ�ó�� �������ּ���.");
		return;
	}

	// ���̴� �̸��� ���� ���
	if (m_LoadShader.m_pszShaderName == NULL)
	{
		AfxMessageBox(L"���̴��� �������� �ʽ��ϴ�.\n���̴��� �������ּ���.");
		return;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// �ѹ��� ��ƼŬ�� ����
	if (m_bCreateMaxParticle == TRUE)
	{
		int iCreateParticle = 0;
		// �̹� ������ ��ƼŬ�� �����Ѵٸ�, �����ϰ�  ���� ��ƼŬ �������� �ִ밳����ŭ�� ����
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

	// ���� Ÿ��
	newParticle.m_bAlphaBlending = m_LoadTexture.m_bAlphaBlend;
	newParticle.m_bAlphaTesting = m_LoadTexture.m_bAlphaTest;

	// ����Ʈ ����
	newParticle.m_Effect.Init();

	// �ؽ�ó ���� ����
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

	// ����Ʈ �ݺ� ����
	newParticle.m_bRepeatEffect = m_bEffectRepeat;

	// �������� / ��������
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
	// ����Ʈ ���� ����
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ParticleSystem newParticle;
	CreateEffect(newParticle,
		TVector3(m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ),
		TVector3(m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ));

	// ����Ʈ �Ѱ��Ѱ��� ����
	for (int iEffect = 0; iEffect < m_iEffectCnt; iEffect++)
	{
		// ����Ʈ ����(�ٿ���ڽ� ����) ����
		if (m_bRandomPos == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vPos = TVector3(randstep(m_fBoxMinX + m_fPosX, m_fBoxMaxX), randstep(m_fBoxMinY + m_fPosY, m_fBoxMaxY), randstep(m_fBoxMinZ + m_fPosZ, m_fBoxMaxZ));
			// ó�� ��ġ ����
			newParticle.m_Particle[iEffect].m_vFirstPos = newParticle.m_Particle[iEffect].m_vPos;
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vPos = TVector3(m_fPosX, m_fPosY, m_fPosZ);
			// ó�� ��ġ ����
			newParticle.m_Particle[iEffect].m_vFirstPos = newParticle.m_Particle[iEffect].m_vPos;
		}
		newParticle.m_Particle[iEffect].m_vCenter = newParticle.m_BoundingBoxShape.m_BoundingBox.vCenter;

		// ����Ʈ �÷��� ����
		if (m_bColorRandom == TRUE)
		{
			newParticle.m_Particle[iEffect].m_vColor = TVector4(randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), randstep(0.0f, 1.0f));
		}
		else
		{
			newParticle.m_Particle[iEffect].m_vColor = TVector4(m_fColorRed, m_fColorGreen, m_fColorBlue, m_fColorAlpha);
		}

		// ����Ʈ ���⺤�� ����
		newParticle.m_Particle[iEffect].m_vDirection = TVector3(m_fDirX, m_fDirY, m_fDirZ);

		// ������ ����
		if (m_bRandomRadian == TRUE)
			newParticle.m_Particle[iEffect].m_fRadian = randstep(1.0f, m_fRadian);
		else
			newParticle.m_Particle[iEffect].m_fRadian = m_fRadian;

		// ������Ÿ�� ����
		if (m_bRandomLifeTime == TRUE)
			newParticle.m_fLifeTime = randstep(1.0f, m_fLifeTime);
		else
			newParticle.m_fLifeTime = m_fLifeTime;

		// ����Ʈ ũ�� ����
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

		// ����Ʈ �ӵ� ���� ����
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
	// ��ƼŬ ����
	((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.push_back(newParticle);
	((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount++;

	// ��ƼŬ �� 2�� �̻��̸� ���� ������ ����Ʈ ��ġ �ʱ�ȭ
	ResetEffect();
}

void EffectToolForm::ResetEffect()
{
	// ��ƼŬ ������ 2�� �̻��̸� ��ġ �ʱ�ȭ
	if (m_iMaxParticleCnt > 1)
	{
		int iEffectSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size() - 1;
		for (int iEffect = 0; iEffect < iEffectSize; iEffect++)
		{
			int iParticleSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iEffect].m_Particle.size();
			for (int iParticle = 0; iParticle < iParticleSize; iParticle++)
			{
				// ��ġ �ʱ�ȭ
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
		AfxMessageBox(L"�������� �ʴ� �ε����Դϴ�.");
		return;
	}
	((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.erase(((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.begin() + m_iDeleteIndex);
	((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount--;

	CString msg;
	msg.Format(_T("%d��° ����Ʈ ����"), m_iDeleteIndex);
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
		// ���� ���� �б�
		_ftprintf_s(m_pStream, _T("*VERSION* %f\n"), VERSION);

		// �Ҵ�� ��ƼŬ ����
		_ftprintf_s(m_pStream, _T("%d\n"), m_iMaxParticleCnt);

		// �ִ� ��ƼŬ ���� ���� ����
		_ftprintf_s(m_pStream, _T("%d\n"), m_bCreateMaxParticle);

		// �ٿ���ڽ�
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ);// �ּҰ�
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ);// �ִ밪

		for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
		{
			// ����Ʈ ����
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_iEffectCount); // ����Ʈ ����

			// �����̼� Ÿ��
			if (m_iRotationType != 0)
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 1);
				_ftprintf_s(m_pStream, _T("%d\n"), m_eCoordRotationType);
			}
			else
			{
				_ftprintf_s(m_pStream, _T("%d\n"), 0);
			}

			// ��ġ ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomPos);

			// �ӵ� ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomSpeed);

			// ����Ʈ �ӵ�
			_ftprintf_s(m_pStream, _T("%f\n"), m_fEffectSpeed);

			// ������ ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomRadian);

			// ������
			_ftprintf_s(m_pStream, _T("%f\n"), m_fRadian);

			// LifeTime ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomLifeTime);

			// Life Time
			_ftprintf_s(m_pStream, _T("%f\n"), m_fLifeTime);

			// ũ�� ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bRandomScale);

			// ����Ʈ ũ��
			_ftprintf_s(m_pStream, _T("%d\n"), m_iScale);

			// ��� ��
			_ftprintf_s(m_pStream, _T("%d %d\n"), m_LoadTexture.m_iRow, m_LoadTexture.m_iCol);

			// ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaBlend);

			// ���� �׽���
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaTest);

			// ��� �ð�
			_ftprintf_s(m_pStream, _T("%f\n"), g_fElapseTime);

			// ���������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bBillboardApply);
			if (m_bBillboardNotApply == true)
			{
				// ����Ʈ ���ؽ� Ÿ��(������ ������ �ٴڿ� �򸮴��� �Ǵ��ϴ� ����Ʈ)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);
			}

			// ����Ʈ ���ؽ� Ÿ��
			if(m_bBillboardApply == false)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);

			// ����Ʈ �ݺ� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bEffectRepeat);

			// ��������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bPerspective);

			// ��������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bOrthogonal);

			// �ؽ�ó ����
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureSRV.size());

			// �ؽ�ó��
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size(); iCnt++)
			{
				_ftprintf_s(m_pStream, _T("%s\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList[iCnt].c_str());

			}

			// ���̴�
			_ftprintf_s(m_pStream, _T("%s\n"), m_LoadShader.m_pszShaderName);

			// ��ƼŬ
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle.size(); iCnt++)
			{
				// ����Ʈ ��ġ
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.z);

				// �ݺ�����Ʈ�̸� ó����ġ ����
				if (m_bEffectRepeat == TRUE)
				{
					_ftprintf_s(m_pStream, _T("%f %f %f\n"),
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.x,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.y,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.z);
				}

				// ����Ʈ �÷�
				_ftprintf_s(m_pStream, _T("%f %f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.z,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.w);

				// ���⺤��
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.z);

				// ����Ʈ ũ��
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.z);

				// ����Ʈ �ӵ�
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.z);

				// ������
				_ftprintf_s(m_pStream, _T("%f\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_fRadian);
			}
		}
		fclose(m_pStream);
		// ������ txt���� ���̺�
		ChangeEffectEngineVersion();
	}
	else
	{
		AfxMessageBox(L"����Ʈ�� �������� �ʾҽ��ϴ�.");
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
		AfxMessageBox(L"�ҷ����� ����");
		return;
	}

	int iParticleSize = ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.size();

	if (iParticleSize > 0)
	{
		for (int iCnt = 0; iCnt < iParticleSize; iCnt++)
			((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.erase(((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.begin());
	}

	// ������ ������ �ؽ�ó�� �����ִٸ� �ʱ�ȭ
	if (m_LoadTexture.m_iTextureCnt > 0)
		m_LoadTexture.ReSetTextureData();

	TCHAR str[256];
	TCHAR tmpStr[256];
	float fVersion = 0.0f;
	int	  iBoolValueRead;

	// ���� ���� �б�
	_fgetts(str, 256, m_pStream);
	_stscanf(str, _T("%s %f"), tmpStr, &fVersion);

	if (fVersion <= VERSION - EPSILON || fVersion >= VERSION + EPSILON)
	{
		AfxMessageBox(L"������������ ���ų� ���� ������ ����Ʈ�Դϴ�.");
		return;
	}

	// �ִ� ��ƼŬ ����
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%d"), &m_iMaxParticleCnt);
	// �ִ� ��ƼŬ ������ŭ ���� ����
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%d"), &iBoolValueRead);
	m_bCreateMaxParticle = (BOOL)iBoolValueRead;

	// �ٿ���ڽ�
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%f %f %f"), &m_fBoxMinX, &m_fBoxMinY, &m_fBoxMinZ);
	_fgetts(str, 256, m_pStream);
	_stscanf_s(str, _T("%f %f %f"), &m_fBoxMaxX, &m_fBoxMaxY, &m_fBoxMaxZ);

	for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
	{
		// ����Ʈ ��
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iEffectCnt);

		// ����Ʈ Ÿ��
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iRotationType);
		
		// ��ġ ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomPos = (BOOL)iBoolValueRead;

		// �ӵ� ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomSpeed = (BOOL)iBoolValueRead;

		// ����Ʈ �ӵ�
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fEffectSpeed);

		// ������ ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomRadian = (BOOL)iBoolValueRead;

		// ������
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fRadian);

		// LifeTime ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomLifeTime = (BOOL)iBoolValueRead;

		// Life Time
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &m_fLifeTime);

		// ũ�� ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bRandomScale = (BOOL)iBoolValueRead;

		// ����Ʈ ũ��
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_iScale);

		// ��� ��
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d %d"), &m_LoadTexture.m_iRow, &m_LoadTexture.m_iCol);

		// ���� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_LoadTexture.m_bAlphaBlend = (BOOL)iBoolValueRead;

		// ���� �׽���
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_LoadTexture.m_bAlphaTest = (BOOL)iBoolValueRead;

		// ����ð�
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%f"), &g_fElapseTime);

		// ���������
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bBillboardApply = (BOOL)iBoolValueRead;

		if (m_bBillboardApply == false)
		{
			// ����Ʈ ���ؽ� Ÿ��(������ ������ �ٴڿ� �򸮴��� �Ǵ��ϴ� ����Ʈ)
			m_bBillboardNotApply == true;
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%d"), &m_iPlaneShape);
		}

		// ����Ʈ �ݺ� ����
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bEffectRepeat = (BOOL)iBoolValueRead;

		// ��������
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bPerspective = (BOOL)iBoolValueRead;

		// ��������
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &iBoolValueRead);
		m_bOrthogonal = (BOOL)iBoolValueRead;

		// �ؽ�ó ��
		_fgetts(str, 256, m_pStream);
		_stscanf_s(str, _T("%d"), &m_LoadTexture.m_iTextureCnt);

		// �ؽ�ó �̸�
		for (int iCnt = 0; iCnt < m_LoadTexture.m_iTextureCnt; iCnt++)
		{
			CString szTextureName;
			_fgetts(str, 256, m_pStream);
			_stscanf(str, _T("%s"), tmpStr);
			szTextureName = tmpStr;
			m_LoadTexture.m_szTextureName.push_back((TCHAR*)(LPCTSTR)szTextureName);
		}

		// ���̴�
		CString szShaderName;
		_fgetts(str, 256, m_pStream);
		_stscanf(str, _T("%s"), tmpStr);
		szShaderName = tmpStr;
		m_LoadShader.CreateShaderFileName(szShaderName);

		// �ε� ����
		m_bLoad = TRUE;

		// ����Ʈ ����
		ParticleSystem newParticle;
		CreateEffect(newParticle,
			TVector3(m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ),
			TVector3(m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ));

		// ����Ʈ ���
		for (int iCnt = 0; iCnt < m_iEffectCnt; iCnt++)
		{
			// ����Ʈ ��ġ
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"), 
				&newParticle.m_Particle[iCnt].m_vPos.x,
				&newParticle.m_Particle[iCnt].m_vPos.y,
				&newParticle.m_Particle[iCnt].m_vPos.z);

			// �ݺ� ����Ʈ�̸� ó����ġ ����
			if (newParticle.m_bRepeatEffect)
			{
				_fgetts(str, 256, m_pStream);
				_stscanf_s(str, _T("%f %f %f"),
					&newParticle.m_Particle[iCnt].m_vFirstPos.x,
					&newParticle.m_Particle[iCnt].m_vFirstPos.y,
					&newParticle.m_Particle[iCnt].m_vFirstPos.z);
			}

			// ����Ʈ �÷�
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vColor.x,
				&newParticle.m_Particle[iCnt].m_vColor.y,
				&newParticle.m_Particle[iCnt].m_vColor.z,
				&newParticle.m_Particle[iCnt].m_vColor.w);

			// ���⺤��
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vDirection.x,
				&newParticle.m_Particle[iCnt].m_vDirection.y,
				&newParticle.m_Particle[iCnt].m_vDirection.z);
			m_fDirX = newParticle.m_Particle[iCnt].m_vDirection.x;
			m_fDirY = newParticle.m_Particle[iCnt].m_vDirection.y;
			m_fDirZ = newParticle.m_Particle[iCnt].m_vDirection.z;

			// ����Ʈ ũ��
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vScale.x,
				&newParticle.m_Particle[iCnt].m_vScale.y,
				&newParticle.m_Particle[iCnt].m_vScale.z);

			// ����Ʈ �ӵ�
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f %f %f"),
				&newParticle.m_Particle[iCnt].m_vSpeed.x,
				&newParticle.m_Particle[iCnt].m_vSpeed.y,
				&newParticle.m_Particle[iCnt].m_vSpeed.z);

			// ������
			_fgetts(str, 256, m_pStream);
			_stscanf_s(str, _T("%f"), &newParticle.m_Particle[iCnt].m_fRadian);
		}
		// ��ƼŬ ����
		((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem.push_back(newParticle);
		((CGameToolApp*)AfxGetApp())->m_Main.m_iParticleSystemCount++;

		// ���� �ҷ��� �� ����
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

			

		// �ؽ�Ʈ �̸��� �����ϸ� �����.
		if (m_LoadTexture.m_szTextureName.size() != 0)
		{
			DeleteVector(m_LoadTexture.m_szTextureName);
			m_LoadTexture.m_iTextureCnt = 0;
		}
	}

	m_bLoad = FALSE;
	fclose(m_pStream);
}


// �ִ� ��ƼŬ �� ���� �Լ�
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
		// text���� �Է�
		TCHAR str[256];

		// �Ҵ�� ��ƼŬ ����
		_ftprintf_s(m_pStream, _T("%d\n"), m_iMaxParticleCnt);

		// �ٿ���ڽ�
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMinX, m_fBoxMinY, m_fBoxMinZ); // �ּҰ�
		_ftprintf_s(m_pStream, _T("%f %f %f\n"), m_fBoxMaxX, m_fBoxMaxY, m_fBoxMaxZ); // �ִ밪

		for (int iObj = 0; iObj < m_iMaxParticleCnt; iObj++)
		{
			// ����Ʈ ����
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_iEffectCount); // ����Ʈ ����

			// �����̼� Ÿ��
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

			// ��� ��
			_ftprintf_s(m_pStream, _T("%d %d\n"), m_LoadTexture.m_iRow, m_LoadTexture.m_iCol);

			// ���� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaBlend);

			// ���� �׽���
			_ftprintf_s(m_pStream, _T("%d\n"), m_LoadTexture.m_bAlphaTest);

			// ��� �ð�
			_ftprintf_s(m_pStream, _T("%f\n"), g_fElapseTime);

			// ���������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bBillboardApply);

			if (m_bBillboardApply == false)
			{
				// ����Ʈ ���ؽ� Ÿ��(������ ������ �ٴڿ� �򸮴��� �Ǵ��ϴ� ����Ʈ)
				_ftprintf_s(m_pStream, _T("%d\n"), m_iPlaneShape);
			}

			// ����Ʈ �ݺ� ����
			_ftprintf_s(m_pStream, _T("%d\n"), m_bEffectRepeat);

			// ��������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bPerspective);

			// ��������
			_ftprintf_s(m_pStream, _T("%d\n"), m_bOrthogonal);

			// �ؽ�ó ����
			_ftprintf_s(m_pStream, _T("%d\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size());

			// �ؽ�ó��
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList.size(); iCnt++)
			{
				TCHAR dir[256] = L"../";
				_tcscat_s(dir, ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Effect.m_pTexture->m_pTextureNameList[iCnt].c_str());
				_ftprintf_s(m_pStream, _T("%s\n"), dir);
			}

			// ���̴�
			TCHAR dir[256] = L"../";
			_tcscat_s(dir, m_LoadShader.m_pszShaderName);
			_ftprintf_s(m_pStream, _T("%s\n"), dir);

			// ��ƼŬ
			for (int iCnt = 0; iCnt < ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle.size(); iCnt++)
			{
				// ����Ʈ ��ġ
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vPos.z);

				// �ݺ�����Ʈ�̸� ó����ġ ����
				if (m_bEffectRepeat == TRUE)
				{
					_ftprintf_s(m_pStream, _T("%f %f %f\n"),
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.x,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.y,
						((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vFirstPos.z);
				}

				// ����Ʈ �÷�
				_ftprintf_s(m_pStream, _T("%f %f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.z,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vColor.w);

				// ���⺤��
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vDirection.z);

				// ����Ʈ ũ��
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vScale.z);

				// ����Ʈ �ӵ�
				_ftprintf_s(m_pStream, _T("%f %f %f\n"),
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.x,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.y,
					((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_vSpeed.z);

				// ������
				_ftprintf_s(m_pStream, _T("%f\n"), ((CGameToolApp*)AfxGetApp())->m_Main.m_ParticleSystem[iObj].m_Particle[iCnt].m_fRadian);
			}
		}
		fclose(m_pStream);
	}
	else
	{
		AfxMessageBox(L"����Ʈ�� �������� �ʾҽ��ϴ�.");
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
