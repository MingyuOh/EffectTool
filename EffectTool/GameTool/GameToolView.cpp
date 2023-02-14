
// GameToolView.cpp : CGameToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GameTool.h"
#endif

#include "GameToolDoc.h"
#include "GameToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameToolView

IMPLEMENT_DYNCREATE(CGameToolView, CView)

BEGIN_MESSAGE_MAP(CGameToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGameToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CGameToolView ����/�Ҹ�

CGameToolView::CGameToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CGameToolView::~CGameToolView()
{
}

BOOL CGameToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGameToolView �׸���

void CGameToolView::OnDraw(CDC* /*pDC*/)
{
	CGameToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CGameToolView �μ�


void CGameToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGameToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGameToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGameToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CGameToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CGameToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGameToolView ����

#ifdef _DEBUG
void CGameToolView::AssertValid() const
{
	CView::AssertValid();
}

void CGameToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameToolDoc* CGameToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameToolDoc)));
	return (CGameToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameToolView �޽��� ó����


void CGameToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	UINT x, y;
	x = cx;
	y = cy;
	CGameToolApp* pApp = ((CGameToolApp*)AfxGetApp());
	pApp->m_Main.ResizeDevice(x, y);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
