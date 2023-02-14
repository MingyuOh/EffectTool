#pragma once
#include "afxwin.h"
#include "CoreSTD.h"

// EffectLoadTexture ��ȭ �����Դϴ�.

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
	EffectLoadTexture(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EffectLoadTexture();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTLOADTEXTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenTexture();
	// // ����Ʈ�ڽ��� ǥ���� �ؽ�Ʈ����Ʈ
	CListBox m_TextureList;
	afx_msg void OnBnClickedOk();
	void CreateFileName(CString szPathName);
	void ReSetTextureData();
	float m_fSpriteTime;
	int m_iRepeatCnt;
};
