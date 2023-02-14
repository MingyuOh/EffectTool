// EffectToolPane.cpp : 구현 파일입니다.
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


// CEffectToolPane 메시지 처리기입니다.


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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
