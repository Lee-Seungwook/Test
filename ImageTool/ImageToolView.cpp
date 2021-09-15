
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

#include "ThickDlg.h"
#include "LineStyleDlg.h"

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
//	ON_COMMAND(ID_DRAWLINE, &CImageToolView::OnDrawline)
ON_COMMAND(ID_DRAW_LINE, &CImageToolView::OnDrawLine)
ON_COMMAND(ID_END_LINE, &CImageToolView::OnEndLine)
ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CImageToolView::OnUpdateDrawLine)
ON_UPDATE_COMMAND_UI(ID_END_LINE, &CImageToolView::OnUpdateEndLine)
//ON_WM_PAINT()
ON_COMMAND(ID_DRAW_COLOR, &CImageToolView::OnDrawColor)
ON_COMMAND(ID_THICK, &CImageToolView::OnThick)
ON_COMMAND(ID_ALLERASE, &CImageToolView::OnAllerase)
ON_COMMAND(ID_PARTERASE, &CImageToolView::OnParterase)
//ON_WM_PAINT()
ON_COMMAND(ID_STRAIGHTLINE, &CImageToolView::OnStraightline)
ON_COMMAND(ID_LINE_STYLE, &CImageToolView::OnLineStyle)
ON_COMMAND(ID_FILL_COLOR, &CImageToolView::OnFillColor)
ON_COMMAND(ID_ROUND_RECT, &CImageToolView::OnRoundRect)
ON_COMMAND(ID_TRIANGLE, &CImageToolView::OnTriangle)
END_MESSAGE_MAP()

// CImageToolView 생성/소멸

CImageToolView::CImageToolView() noexcept : m_nZoom(1)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bPaint = FALSE;

	m_nLine = FALSE;

	m_bStick = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bRoundRect = FALSE;
	m_bTriangle = FALSE;

	m_bPartErase = FALSE;

	m_color = RGB(0, 0, 0);
	m_FillColor = RGB(255, 255, 255);
	m_nWidth = 3;
	m_nStyle = 0;
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

	/* pDC->MoveTo(m_nowP);
	pDC->LineTo(m_afterP);*/ // 각각 배열로 값들을 저장해서 draw함수에서 다시 설정해주면 창의 크기를 변경해도 그림이 남아있는다!! (배열을 생성해서 각각의 답을 저장)



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
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 255)); // 흰색으로 설정
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
	m_bEllipse = !m_bEllipse;
	m_bRect = FALSE;
	m_nLine = FALSE;
	m_bStick = FALSE;
	m_bPartErase = FALSE;

}


void CImageToolView::OnRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bRect = !m_bRect;
	m_bEllipse = FALSE;
	m_nLine = FALSE;
	m_bStick = FALSE;
	m_bPartErase = FALSE;

}

void CImageToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_bPaint = FALSE;
	

	m_afterP.x = point.x;
	m_afterP.y = point.y;

	if (m_bStick == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = m_color;
		lbr.lbHatch = 0;

		CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
		CPen* oldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_COPYPEN);
		dc.MoveTo(m_nowP);
		dc.LineTo(m_afterP);
		dc.SelectObject(oldPen);
	}

	if (m_bRect == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = m_color;
		lbr.lbHatch = 0;

		CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
		CPen* oldPen = dc.SelectObject(&pen);

		CBrush brush;
		brush.CreateSolidBrush(m_FillColor);
		CBrush* oldBrush = dc.SelectObject(&brush);
		
		dc.SetROP2(R2_COPYPEN);
		/*dc.MoveTo(m_nowP);
		dc.LineTo(m_afterP);*/
		dc.Rectangle(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y);
		
		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}

	if (m_bEllipse == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = m_color;
		lbr.lbHatch = 0;

		CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
		CPen* oldPen = dc.SelectObject(&pen);

		CBrush brush;
		brush.CreateSolidBrush(m_FillColor);
		CBrush* oldBrush = dc.SelectObject(&brush);

		dc.SetROP2(R2_COPYPEN);
		/*dc.MoveTo(m_nowP);
		dc.LineTo(m_afterP);*/
		dc.Ellipse(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y);

		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}

	if (m_bRoundRect == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = m_color;
		lbr.lbHatch = 0;

		CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
		CPen* oldPen = dc.SelectObject(&pen);

		CBrush brush;
		brush.CreateSolidBrush(m_FillColor);
		CBrush* oldBrush = dc.SelectObject(&brush);

		dc.SetROP2(R2_COPYPEN);
		/*dc.MoveTo(m_nowP);
		dc.LineTo(m_afterP);*/
		dc.RoundRect(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y, 50, 50);

		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}

	if (m_bTriangle == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		CPoint m_point1, m_point2, m_point3;
		m_point1 = m_point2 = m_point3 = point;

		m_point1.x = m_nowP.x - (m_afterP.x - m_nowP.x);
		m_point2 = m_nowP;
		POINT ar1[] = { m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y, m_point1.x, m_afterP.y };
		LOGBRUSH lbr;
		lbr.lbStyle = BS_SOLID;
		lbr.lbColor = m_color;
		lbr.lbHatch = 0;

		CPen pen(PS_GEOMETRIC | m_nStyle | PS_ENDCAP_FLAT | PS_JOIN_MITER, m_nWidth, &lbr);// 선의 스타일, 굵기, 색상
		CPen* oldPen = dc.SelectObject(&pen);

		CBrush brush;
		brush.CreateSolidBrush(m_FillColor);
		CBrush* oldBrush = dc.SelectObject(&brush);
		// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
		
		dc.SetROP2(R2_COPYPEN);
		dc.BeginPath(); // 요거랑
		dc.MoveTo(m_nowP);
		dc.LineTo(m_afterP);
		dc.LineTo(m_point1.x, m_afterP.y);
		dc.LineTo(m_point2);
		dc.EndPath(); // 이거랑
		dc.StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.
		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
		// 직선의 끝점의 좌표를 갱신
		
	}

	ReleaseCapture();
	

	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	m_bPaint = TRUE;
	m_nowP = m_afterP = point;

	m_ptFrom = point;

	if (m_bPartErase == TRUE)
	{
		CClientDC dc(this);
		CPen pen;
		CBrush brush(RGB(255, 255, 255));

		pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		dc.SelectObject(&pen);
		dc.SelectObject(&brush);
		dc.Rectangle(point.x - 15, point.y - 15, point.x + 15, point.y + 15);
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CImageToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 상태바에 마우스 좌표 및 이미지 정보 표시
	CPoint pt = point + GetScrollPosition();
	pt.x /= m_nZoom;
	pt.y /= m_nZoom;
	ShowImageInfo(pt); // 포인터 위치를 매개변수로 전달

	if (m_nLine == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);

			CPen pen(PS_SOLID, m_nWidth, m_color); // 색과 굴기
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_ptFrom);
			dc.LineTo(point);
			dc.SelectObject(oldPen);

			Line line;
			line.ptTo = point;
			line.color = m_color;
			line.width = m_nWidth;
			m_lines.Add(line);

			m_ptFrom = point;
		}
	}

	if (m_bPartErase == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			CPen pen;
			CBrush brush(RGB(255, 255, 255));

			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			dc.SelectObject(&pen);
			dc.SelectObject(&brush);
			dc.Rectangle(point.x - 15, point.y - 15, point.x + 15, point.y + 15);
		}
	}

	if (m_bStick == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			// CPen pen;
			LOGBRUSH lbr;
			lbr.lbStyle = BS_SOLID;
			lbr.lbColor = m_color;
			lbr.lbHatch = 0;

			CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 색과 굴기
			CPen* oldPen = dc.SelectObject(&pen);
			// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
			dc.SetROP2(R2_NOT);
			dc.MoveTo(m_nowP);
			dc.LineTo(m_afterP);
			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.MoveTo(m_nowP);
			dc.LineTo(point);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bRect == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			// CPen pen;
			LOGBRUSH lbr;
			lbr.lbStyle = BS_SOLID;
			lbr.lbColor = m_color;
			lbr.lbHatch = 0;

			CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
			CPen* oldPen = dc.SelectObject(&pen);

			CBrush brush;
			brush.CreateSolidBrush(m_FillColor);
			CBrush* oldBrush = dc.SelectObject(&brush);
			// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(m_afterP);*/
			dc.Rectangle(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y);
			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(point);*/
			dc.Rectangle(m_nowP.x, m_nowP.y, point.x, point.y);
			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bEllipse == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			// CPen pen;
			LOGBRUSH lbr;
			lbr.lbStyle = BS_SOLID;
			lbr.lbColor = m_color;
			lbr.lbHatch = 0;

			CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
			CPen* oldPen = dc.SelectObject(&pen);

			CBrush brush;
			brush.CreateSolidBrush(m_FillColor);
			CBrush* oldBrush = dc.SelectObject(&brush);
			// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(m_afterP);*/
			dc.Ellipse(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y);
			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(point);*/
			dc.Ellipse(m_nowP.x, m_nowP.y, point.x, point.y);
			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bRoundRect == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			// CPen pen;
			LOGBRUSH lbr;
			lbr.lbStyle = BS_SOLID;
			lbr.lbColor = m_color;
			lbr.lbHatch = 0;

			CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0);// 선의 스타일, 굵기, 색상
			CPen* oldPen = dc.SelectObject(&pen);

			CBrush brush;
			brush.CreateSolidBrush(m_FillColor);
			CBrush* oldBrush = dc.SelectObject(&brush);
			// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(m_afterP);*/
			dc.RoundRect(m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y, 50, 50);
			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			/*dc.MoveTo(m_nowP);
			dc.LineTo(point);*/
			dc.RoundRect(m_nowP.x, m_nowP.y, point.x, point.y, 50, 50);
			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bTriangle == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			// CPen pen;
			CPoint m_point1, m_point2, m_point3;
			m_point1 = m_point2 = m_point3 = point;
			
			m_point1.x = m_nowP.x - (m_afterP.x - m_nowP.x);
			m_point2 = m_nowP;
			m_point3.x = m_nowP.x - (point.x - m_nowP.x);
			POINT ar1[] = { m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y, m_point1.x, m_afterP.y };
			POINT ar2[] = { m_nowP.x, m_nowP.y, point.x, point.y, m_point3.x, point.y };

			LOGBRUSH lbr;
			lbr.lbStyle = BS_SOLID;
			lbr.lbColor = m_color;
			lbr.lbHatch = 0;

			CPen pen(PS_GEOMETRIC | m_nStyle | PS_ENDCAP_FLAT | PS_JOIN_MITER, m_nWidth, &lbr);// 선의 스타일, 굵기, 색상
			CPen* oldPen = dc.SelectObject(&pen);

			CBrush brush;
			brush.CreateSolidBrush(m_FillColor);
			CBrush* oldBrush = dc.SelectObject(&brush);
			// 이전에 그린 직선을 지우기 위해서 레스터 오퍼레이션을 R2_NOT으로 지정
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar1, 3); // 배열을 통해 좌표를 설정하고, 꼭짓점 수를 넣는다.
			/*dc.BeginPath();
			dc.MoveTo(m_nowP);
			dc.LineTo(m_afterP);
			dc.LineTo(m_point1.x, m_afterP.y);
			dc.LineTo(m_point2);
			dc.EndPath();
			dc.StrokeAndFillPath();*/
			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar2, 3);
			/*dc.BeginPath();
			dc.MoveTo(m_nowP);
			dc.LineTo(point);
			dc.LineTo(m_point3.x, point.y);
			dc.LineTo(m_point2);
			dc.EndPath();
			dc.StrokeAndFillPath();*/

			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

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


//void CImageToolView::OnDrawline()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	m_nLine = TRUE;
//}


void CImageToolView::OnDrawLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nLine = !m_nLine;
	m_bStick = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnEndLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nLine = FALSE;
}


void CImageToolView::OnUpdateDrawLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nLine == 1);
}


void CImageToolView::OnUpdateEndLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nLine == 0);
}


//void CImageToolView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
//					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.
//	/*for (int i = 0; i < m_lines.GetSize(); i++) // Invalidate시에 복원해주는 루틴... 적
//	{
//		Line line = m_lines.GetAt(i);
//		CPen pen(PS_SOLID, line.width, line.color);
//		CPen* oldPen = dc.SelectObject(&pen);
//		dc.MoveTo(line.ptFrom);
//		dc.LineTo(line.ptTo);
//		dc.SelectObject(oldPen);
//	}*/
//}


void CImageToolView::OnDrawColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlg(m_color, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_color = dlg.GetColor();
	}
}


void CImageToolView::OnThick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CThickDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nWidth = dlg.m_nThick;
	}
}

//void CImageToolView::DrawLine(CDC *pDC, CPoint point)
//{
//	CPen pen;
//	pen.CreatePen(PS_SOLID, 1, m_color);
//	CPen * pOldPen = (CPen *)pDC->SelectObject(&pen);
//
//	pDC->MoveTo(m_nowP.x, m_nowP.y);
//	pDC->LineTo(m_afterP.x, m_afterP.y);
//}


void CImageToolView::OnAllerase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Invalidate(TRUE);
}


void CImageToolView::OnParterase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPartErase = !m_bPartErase;
	m_nLine = FALSE;
	m_bStick = FALSE;
}


//void CImageToolView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
//					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.
//
//	dc.MoveTo(m_nowP);
//	dc.LineTo(m_afterP);
//}


void CImageToolView::OnStraightline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bStick = !m_bStick;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnLineStyle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CLineStyleDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nStyle = dlg.m_nLineStyle;
	}
}


void CImageToolView::OnFillColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlg(m_FillColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_FillColor = dlg.GetColor();
	}
}


void CImageToolView::OnRoundRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bRoundRect = !m_bRoundRect;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnTriangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bTriangle = !m_bTriangle;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}
