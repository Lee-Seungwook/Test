
// ImageToolView.cpp: CImageToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "MainFrm.h"
#include "ImageToolDoc.h"
#include "ImageToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ELLIPSE_MODE 1
#define RECTANGLE_MODE 2
// CImageToolView

IMPLEMENT_DYNCREATE(CImageToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOM1, &CImageToolView::OnViewZoom1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM1, &CImageToolView::OnUpdateViewZoom1)
	ON_COMMAND(ID_VIEW_ZOOM2, &CImageToolView::OnViewZoom2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM2, &CImageToolView::OnUpdateViewZoom2)
	ON_COMMAND(ID_VIEW_ZOOM3, &CImageToolView::OnViewZoom3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM3, &CImageToolView::OnUpdateViewZoom3)
	ON_COMMAND(ID_VIEW_ZOOM4, &CImageToolView::OnViewZoom4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM4, &CImageToolView::OnUpdateViewZoom4)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_ELLIPSE, &CImageToolView::OnEllipse)
	ON_COMMAND(ID_RECTANGLE, &CImageToolView::OnRectangle)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE, &CImageToolView::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(ID_RECTANGLE, &CImageToolView::OnUpdateRectangle)
END_MESSAGE_MAP()

// CImageToolView 생성/소멸

CImageToolView::CImageToolView() noexcept : m_nZoom(1)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bPaint = FALSE;
}

CImageToolView::~CImageToolView()
{
}

BOOL CImageToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageToolView 그리기

void CImageToolView::OnDraw(CDC* pDC)
{
	CImageToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth(); // 영상의 가로
		int h = pDoc->m_Dib.GetHeight(); // 영상의 세로
		pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, w * m_nZoom, h * m_nZoom); // 확대 및 영상의 가로, 세로를 반영하여 그린다.
	}

}

void CImageToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizeToFit();

	
}

void CImageToolView::SetScrollSizeToFit(void)
{
	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	CImageToolDoc* pDoc = GetDocument(); // 클래스의 포인터를 받아옴
	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth(); // 영상의 가로
		int h = pDoc->m_Dib.GetHeight(); // 영상의 세로 

		sizeTotal.cx = w * m_nZoom; // 확대 정보를 곱하여 크기 설정
		sizeTotal.cy = h * m_nZoom; // 확대 정보를 곱하여 크기 설정
	}
	else // 비트맵이 정상적으로 생성되지 않은경우
	{
		sizeTotal.cx = sizeTotal.cy = 100; // 100으로 설정
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE); // 차일드 프레임의 크기를 뷰의 크기에 맞춰주는 작업을 수행, 영상이 프레임보다 작은 경우 프레임을 영상에 맞춘다.
}


// CImageToolView 인쇄


void CImageToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageToolView 진단

#ifdef _DEBUG
void CImageToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageToolDoc* CImageToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageToolDoc)));
	return (CImageToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageToolView 메시지 처리기

// 윈도우의 배경이 지워져야 하는 경우에 발생하는 메시지, 영상 크기변경시 깜빡이는 것을 없애는 기능이다.
BOOL CImageToolView::OnEraseBkgnd(CDC* pDC)
{
	CBrush br; // 브러쉬 생성
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200)); // 빗금 무늬 생성
	FillOutsideRect(pDC, &br); // 스크롤되는 바깥 옆역을 채운다.

	return TRUE;
}


void CImageToolView::OnViewZoom1()
{
	m_nZoom = 1;
	SetScrollSizeToFit(); // 스크롤바 설정
	Invalidate(TRUE); // 뷰 영역을 다시 그림
}


void CImageToolView::OnUpdateViewZoom1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 1); // 메뉴에 체크 설정
}


void CImageToolView::OnViewZoom2()
{
	m_nZoom = 2;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 2);
}


void CImageToolView::OnViewZoom3()
{
	m_nZoom = 3;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 3);
}


void CImageToolView::OnViewZoom4()
{
	m_nZoom = 4;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}


void CImageToolView::OnUpdateViewZoom4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 4);
}

void CImageToolView::OnEllipse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CClientDC DC(this);
	DC.Ellipse(m_nowP.x, m_nowP.y, m_nowP.x + 100, m_nowP.y + 100);
	CString m_text;
	AfxPrintInfo(_T("현재 위치는 %d, %d 입니다."), m_nowP.x, m_nowP.y);
	m_nDrawMode = ELLIPSE_MODE;
}


void CImageToolView::OnRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CClientDC DC(this);
	DC.Rectangle(m_nowP.x, m_nowP.y, m_nowP.x + 100, m_nowP.y + 100);
	CString m_text;
	AfxPrintInfo(_T("현재 위치는 %d, %d 입니다."), m_nowP.x, m_nowP.y);
	m_nDrawMode = RECTANGLE_MODE;
}

void CImageToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bPaint = FALSE;

	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bPaint = TRUE;
	m_nowP.x = point.x;
	m_nowP.y = point.y;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CImageToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 상태바에 마우스 좌표 및 이미지 정보 표시
	CPoint pt = point + GetScrollPosition();
	pt.x /= m_nZoom;
	pt.y /= m_nZoom;
	ShowImageInfo(pt); // 포인터 위치를 매개변수로 전달

	if (!m_bPaint) return;
	CClientDC dc(this);
	dc.MoveTo(m_nowP.x, m_nowP.y);
	dc.LineTo(point.x, point.y);
	m_nowP = point;

	CScrollView::OnMouseMove(nFlags, point);
}


void CImageToolView::ShowImageInfo(CPoint point)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CImageToolDoc* pDoc = GetDocument();
	int w = pDoc->m_Dib.GetWidth();
	int h = pDoc->m_Dib.GetHeight();
	int c = pDoc->m_Dib.GetPaletteNums();

	CString strText;

	// 상태바에 마우스 좌표 표시

	if (point.x >= 0 && point.y >= 0 && point.x < w && point.y < h)
	{
		strText.Format(_T("(%d, %d)"), point.x, point.y);
		pFrame->m_wndStatusBar.SetPaneText(0, strText); // 왼쪽에 출력
	}

	// 상태바에 이미지 정보 표시

	if (c == 0) // 트루 컬러 영상
	{
		strText.Format(_T("w:%d  h:%d  c:16M"), w, h);
	}
	else // 그레이스케일 영상
	{
		strText.Format(_T("w:%d  h:%d  c:%d"), w, h, c);
	}
	pFrame->m_wndStatusBar.SetPaneText(1, strText); // 오른쪽에 출력
}


void CImageToolView::OnUpdateEllipse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? 1 : 0);
}


void CImageToolView::OnUpdateRectangle(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nDrawMode == RECTANGLE_MODE ? 1 : 0);
}
