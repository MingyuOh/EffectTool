#pragma once
#include "afxwin.h"
#include "CoreSTD.h"

// EffectLoadTexture 대화 상자입니다.

class EffectLoadTexture : public CDialogEx
{
	DECLARE_DYNAMIC(EffectLoadTexture)
public:
	vector<basic_string<TCHAR>>		m_szTextureName;
	TCHAR*							m_pTextureName;
	int								m_iTextureCnt;
	int								m_iRow;
	int								m_iCol;
	BOOL							m_bAlphaBlend;
	BOOL							m_bAlphaTest;
public:
	EffectLoadTexture(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~EffectLoadTexture();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTLOADTEXTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenTexture();
	// // 리스트박스에 표시할 텍스트리스트
	CListBox m_TextureList;
	afx_msg void OnBnClickedOk();
	void CreateFileName(CString szPathName);
	void ReSetTextureData();
	float m_fSpriteTime;
	int m_iRepeatCnt;
};
