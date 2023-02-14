#pragma once
#include "EffectToolForm.h"

// CEffectToolPane

class CEffectToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEffectToolPane)
public:
	EffectToolForm*		m_wndForm;
public:
	CEffectToolPane();
	virtual ~CEffectToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


