#pragma once
#include "afxwin.h"

// EffectLoadShader 대화 상자입니다.

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
	EffectLoadShader(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~EffectLoadShader();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTLOADSHADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	TCHAR*	m_pszShaderName;
	TCHAR	m_szShaderName[256];
public:
	afx_msg void OnBnClickedOpenShader();
	virtual BOOL OnInitDialog();
	void CreateShaderFileName(CString szPathName);

	// 셰이더리스트
	CListBox m_ShaderList;
};
