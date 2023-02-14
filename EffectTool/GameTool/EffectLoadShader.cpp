// EffectLoadShader.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameTool.h"
#include "EffectLoadShader.h"
#include "afxdialogex.h"


// EffectLoadShader ��ȭ �����Դϴ�.

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


// EffectLoadShader �޽��� ó�����Դϴ�.


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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
