// EffectToolPane.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameTool.h"
#include "EffectToolPane.h"


// CEffectToolPane

IMPLEMENT_DYNAMIC(CEffectToolPane, CDockablePane)

CEffectToolPane::CEffectToolPane()
{

}

CEffectToolPane::~CEffectToolPane()
{
}


BEGIN_MESSAGE_MAP(CEffectToolPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CEffectToolPane �޽��� ó�����Դϴ�.


int CEffectToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndForm = EffectToolForm::CreateOne(this);
	return 0;
}


void CEffectToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int CEffectToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
