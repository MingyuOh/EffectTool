#pragma once
#include "afxwin.h"

// EffectLoadShader ��ȭ �����Դϴ�.

struct shaderName
{
	TCHAR*	szVSName;
	TCHAR*	szPSName;
	TCHAR*	szGSName;
};

class EffectLoadShader : public CDialogEx
{
	DECLARE_DYNAMIC(EffectLoadShader)

public:
	EffectLoadShader(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EffectLoadShader();
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTLOADSHADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	TCHAR*	m_pszShaderName;
	TCHAR	m_szShaderName[256];
public:
	afx_msg void OnBnClickedOpenShader();
	virtual BOOL OnInitDialog();
	void CreateShaderFileName(CString szPathName);

	// ���̴�����Ʈ
	CListBox m_ShaderList;
};
