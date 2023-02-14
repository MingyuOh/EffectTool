// EffectLoadShader.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameTool.h"
#include "EffectLoadShader.h"
#include "afxdialogex.h"


// EffectLoadShader 대화 상자입니다.

IMPLEMENT_DYNAMIC(EffectLoadShader, CDialogEx)

EffectLoadShader::EffectLoadShader(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECTLOADSHADER, pParent)
{
}

EffectLoadShader::~EffectLoadShader()
{
	
}

void EffectLoadShader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHADERLIST, m_ShaderList);
}


BEGIN_MESSAGE_MAP(EffectLoadShader, CDialogEx)
	ON_BN_CLICKED(IDC_OPEN_SHADER, &EffectLoadShader::OnBnClickedOpenShader)
END_MESSAGE_MAP()


// EffectLoadShader 메시지 처리기입니다.


void EffectLoadShader::OnBnClickedOpenShader()
{
	CString selFileName;
	CFileDialog vsdlg(TRUE, L"HLSL", NULL, 
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, 
		L"HLSL Files(*.hlsl)|*.hlsl|All Files(*.*)|*.*|", 
		this);

	if (vsdlg.DoModal() == IDOK)
	{
		selFileName = vsdlg.GetPathName();
		CreateShaderFileName(selFileName);
		m_ShaderList.InsertString(0, m_szShaderName);
	}
}

void EffectLoadShader::CreateShaderFileName(CString szPathName)
{
	m_pszShaderName = (TCHAR*)(LPCTSTR)szPathName;
	TCHAR drive[256] = { 0, };
	TCHAR dir[256] = { 0, };
	TCHAR filename[256] = { 0, };
	TCHAR ext[256] = { 0, };

	TCHAR path[256] = L"../data/Shader/";
	_tsplitpath_s(m_pszShaderName, drive, dir, filename, ext);

	_tcscpy_s(drive, path);
	_tcscat_s(drive, filename);
	_tcscat_s(drive, ext);

	_tcscpy_s(m_szShaderName, drive);
	m_pszShaderName = m_szShaderName;
}


BOOL EffectLoadShader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pszShaderName = NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
