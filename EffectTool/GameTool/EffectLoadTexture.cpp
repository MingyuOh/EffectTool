// EffectLoadTexture.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameTool.h"
#include "EffectLoadTexture.h"
#include "afxdialogex.h"


// EffectLoadTexture 대화 상자입니다.

IMPLEMENT_DYNAMIC(EffectLoadTexture, CDialogEx)

EffectLoadTexture::EffectLoadTexture(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECTLOADTEXTURE, pParent), m_iTextureCnt(0)
	, m_iRow(1)
	, m_iCol(1)
	, m_bAlphaBlend(FALSE)
	, m_bAlphaTest(FALSE)
	, m_fSpriteTime(1)
{
	
}

EffectLoadTexture::~EffectLoadTexture()
{
	DeleteVector(m_szTextureName);
}

void EffectLoadTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURELIST, m_TextureList);
	DDX_Text(pDX, IDC_UVROW, m_iRow);
	DDX_Text(pDX, IDC_UVCOL, m_iCol);
	DDX_Check(pDX, IDC_ALPHABLENDING, m_bAlphaBlend);
	DDX_Check(pDX, IDC_ALPHATESTING, m_bAlphaTest);
	DDX_Text(pDX, IDC_SPRITE_TIME, m_fSpriteTime);
}


BEGIN_MESSAGE_MAP(EffectLoadTexture, CDialogEx)
	ON_BN_CLICKED(IDC_OPEN_TEXTURE, &EffectLoadTexture::OnBnClickedOpenTexture)
	ON_BN_CLICKED(IDOK, &EffectLoadTexture::OnBnClickedOk)
END_MESSAGE_MAP()


// EffectLoadTexture 메시지 처리기입니다.


void EffectLoadTexture::OnBnClickedOpenTexture()
{
	CString selFileName;
	CFileDialog textureDlg(TRUE, L"Image File", NULL,
		OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		L"|All Files(*.*)|*.*|",
		this);

	if (textureDlg.DoModal() == IDOK)
	{
		POSITION pos = textureDlg.GetStartPosition();

		if (m_szTextureName.size() != 0)
		{
			DeleteVector(m_szTextureName);
			m_iTextureCnt = 0;
		}

		while (pos != NULL)
		{
			selFileName = textureDlg.GetNextPathName(pos);
			CreateFileName(selFileName);
			m_iTextureCnt++;
		}
	}
}

void EffectLoadTexture::CreateFileName(CString szPathName)
{
	m_pTextureName = (TCHAR*)(LPCTSTR)szPathName;
	TCHAR drive[256] = { 0, };
	TCHAR dir[256] = { 0, };
	TCHAR filename[256] = { 0, };
	TCHAR ext[256] = { 0, };

	TCHAR path[256] = L"../data/Texture/";
	_tsplitpath_s(m_pTextureName, drive, dir, filename, ext);

	// tga 파일이면 dds로 변경
	if (wcscmp(ext, L".tga") == 0)
		wcscpy_s(ext, L".dds");

	_tcscpy_s(drive, path);
	_tcscat_s(drive, filename);
	_tcscat_s(drive, ext);
	m_TextureList.AddString(drive);
}


void EffectLoadTexture::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
	// 최대 텍스처 개수
	if (m_iTextureCnt > 4)
		m_iTextureCnt = 4;
	for (int iIndex = 0; iIndex < m_iTextureCnt; iIndex++)
	{
		CString str;
		m_TextureList.GetText(iIndex, str);
		m_szTextureName.push_back((TCHAR*)(LPCTSTR)str);
	}
}

void EffectLoadTexture::ReSetTextureData()
{
	DeleteVector(m_szTextureName);
	m_pTextureName = NULL;
	m_iTextureCnt = 0;
	m_iRow = 1;
	m_iCol = 1;
	m_bAlphaBlend = FALSE;
	m_bAlphaTest = FALSE;
}