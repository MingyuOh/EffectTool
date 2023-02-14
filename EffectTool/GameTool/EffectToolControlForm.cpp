// EffectToolControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameTool.h"
#include "EffectToolControlForm.h"


// EffectToolControlForm

IMPLEMENT_DYNCREATE(EffectToolControlForm, CFormView)

EffectToolControlForm::EffectToolControlForm()
	: CFormView(IDD_EFFECTTOOLCONTROLFORM)
{

}

EffectToolControlForm::~EffectToolControlForm()
{
}

void EffectToolControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(EffectToolControlForm, CFormView)
END_MESSAGE_MAP()


// EffectToolControlForm 진단입니다.

#ifdef _DEBUG
void EffectToolControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void EffectToolControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// EffectToolControlForm 메시지 처리기입니다.
