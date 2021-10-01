
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
#include "MyData.h"
#include "MyStick.h"
#include "MyEllipse.h"
#include "MyRect.h"
#include "MyRoundRect.h"
#include "MyTriangle.h"
#include "MyRightTriangle.h"
#include "MyRhombus.h"
#include "MyPentagon.h"
#include "MyErase.h"
#include "MyColorFill.h"

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
	ON_COMMAND(ID_RIGHTTRI, &CImageToolView::OnRighttri)
	ON_COMMAND(ID_RHOMBUS, &CImageToolView::OnRhombus)
	ON_COMMAND(ID_PENTAGON, &CImageToolView::OnPentagon)
	ON_COMMAND(ID_COLORFILL, &CImageToolView::OnColorfill)
	//ON_WM_SETCURSOR()
	//ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ALL_ERASE, &CImageToolView::OnAllErase)
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
	m_bRightTriangle = FALSE;
	m_bRhombus = FALSE;
	m_bPentagon = FALSE;
	m_bColorFill = FALSE;

	m_bPartErase = FALSE;

	m_bMouseDraw = FALSE;
	m_bMouseFill = FALSE;
	m_bMouseErase = FALSE;
	m_bMousePoly = FALSE;
	m_bAErase = FALSE;
	
	m_color = RGB(0, 0, 0);
	m_FillColor = RGB(255, 255, 255);
	m_nWidth = 3;
	m_nStyle = 0;
	m_nMousetempPoly = 0;

	m_pCurrentMyData = NULL;
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

	if (m_bAErase == TRUE)
	{
		pDC->SelectStockObject(WHITE_PEN);
		pDC->SelectStockObject(WHITE_BRUSH);
		CRect rect;
		GetClientRect(&rect);
		pDC->Rectangle(&rect);
		m_bAErase = FALSE;
	}

	// GetHeadPosition() 함수는 while 문안의 GetNext() 함수와 쌍으로 리스트의 모든 항목을 참조하도록 해준다.
	POSITION pos = pDoc->m_MyDataList.GetHeadPosition();
	while (pos != NULL)
	{
		CMyData *pMyData = pDoc->m_MyDataList.GetNext(pos);
		// CMyData의 Draw() 함수에 곡선을 출력하는 로직이 완벽하게 구현되어 있으므로
		// OnDraw() 함수에서는 단순히 Draw() 함수를 호출해 주기만 하면 곡선이 그려진다.
		pMyData->Draw(pDC);
	}

	POSITION poserase = pDoc->m_MyEraseList.GetHeadPosition();
	while (poserase != NULL)
	{
		CMyErase *pMyErase = pDoc->m_MyEraseList.GetNext(poserase);
		pMyErase->Draw(pDC);
	}

	POSITION posstick = pDoc->m_MyStickList.GetHeadPosition();
	while (posstick != NULL)
	{
		CMyStick *pMyStick = pDoc->m_MyStickList.GetNext(posstick);
		pMyStick->Draw(pDC);
	}

	POSITION posellipse = pDoc->m_MyEllipseList.GetHeadPosition();
	while (posellipse != NULL)
	{
		CMyEllipse *pMyEllipse = pDoc->m_MyEllipseList.GetNext(posellipse);
		pMyEllipse->Draw(pDC);
	}

	POSITION posrect = pDoc->m_MyRectList.GetHeadPosition();
	while (posrect != NULL)
	{
		CMyRect *pMyRect = pDoc->m_MyRectList.GetNext(posrect);
		pMyRect->Draw(pDC);
	}

	POSITION posroundrect = pDoc->m_MyRoundRectList.GetHeadPosition();
	while (posroundrect != NULL)
	{
		CMyRoundRect *pMyRoundRect = pDoc->m_MyRoundRectList.GetNext(posroundrect);
		pMyRoundRect->Draw(pDC);
	}

	POSITION postriangle = pDoc->m_MyTriangleList.GetHeadPosition();
	while (postriangle != NULL)
	{
		CMyTriangle *pMyTriangle = pDoc->m_MyTriangleList.GetNext(postriangle);
		pMyTriangle->Draw(pDC);
	}

	POSITION posrighttriangle = pDoc->m_MyRightTriangleList.GetHeadPosition();
	while (posrighttriangle != NULL)
	{
		CMyRightTriangle *pMyRightTriangle = pDoc->m_MyRightTriangleList.GetNext(posrighttriangle);
		pMyRightTriangle->Draw(pDC);
	}

	POSITION posrhombus = pDoc->m_MyRhombusList.GetHeadPosition();
	while (posrhombus != NULL)
	{
		CMyRhombus *pMyRhombus = pDoc->m_MyRhombusList.GetNext(posrhombus);
		pMyRhombus->Draw(pDC);
	}

	POSITION pospentagon = pDoc->m_MyPentagonList.GetHeadPosition();
	while (pospentagon != NULL)
	{
		CMyPentagon *pMyPentagon = pDoc->m_MyPentagonList.GetNext(pospentagon);
		pMyPentagon->Draw(pDC);
	}

	POSITION poscolorfill = pDoc->m_MyColorFillList.GetHeadPosition();
	while (poscolorfill != NULL)
	{
		CMyColorFill *pMyColorFill = pDoc->m_MyColorFillList.GetNext(poscolorfill);
		pMyColorFill->Draw(pDC);
	}
}

void CImageToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizeToFit();
	m_hCursorDe = AfxGetApp()->LoadCursorW(IDC_CURSOR_DEFAULT);
	m_hCursorDraw = AfxGetApp()->LoadCursorW(IDC_CURSOR_DRAW);
	m_hCursorFill = AfxGetApp()->LoadCursorW(IDC_CURSOR_COLORFILL);
	m_hCursorErase = AfxGetApp()->LoadCursorW(IDC_CURSOR_ERASE);
	m_hCursorPoly = AfxGetApp()->LoadCursorW(IDC_CURSOR_POLY);


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

	if (m_bRightTriangle == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;

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
		dc.LineTo(m_nowP.x, m_afterP.y);
		dc.LineTo(m_nowP);
		dc.EndPath(); // 이거랑
		dc.StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.

		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}

	if (m_bRhombus == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		CPoint m_temp;
		m_temp.x = m_nowP.x + (m_afterP.x - m_nowP.x) / 2;
		m_temp.y = m_nowP.y + (m_afterP.y - m_nowP.y) / 2;

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
		dc.MoveTo(m_temp.x, m_nowP.y);
		dc.LineTo(m_afterP.x, m_temp.y);
		dc.LineTo(m_temp.x, m_afterP.y);
		dc.LineTo(m_nowP.x, m_temp.y);
		dc.LineTo(m_temp.x, m_nowP.y);
		dc.EndPath(); // 이거랑
		dc.StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.

		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}

	if (m_bPentagon == TRUE)
	{
		CClientDC dc(this);
		// CPen pen;
		CPoint m_temp1, m_temp2, m_temp3;
		m_temp1.x = m_nowP.x + (m_afterP.x - m_nowP.x) / 2;
		m_temp1.y = m_nowP.y + (m_afterP.y - m_nowP.y) / 2;

		m_temp2.x = m_nowP.x + (m_temp1.x - m_nowP.x) / 2;
		m_temp2.y = m_afterP.y;

		m_temp3.x = m_temp1.x + (m_afterP.x - m_temp1.x) / 2;
		m_temp3.y = m_afterP.y;

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
		dc.MoveTo(m_temp1.x, m_nowP.y);
		dc.LineTo(m_afterP.x, m_temp1.y);
		dc.LineTo(m_temp3.x, m_afterP.y);
		dc.LineTo(m_temp2.x, m_afterP.y);
		dc.LineTo(m_nowP.x, m_temp1.y);
		dc.LineTo(m_temp1.x, m_nowP.y);
		dc.EndPath(); // 이거랑
		dc.StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.

		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
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
	if (m_nLine == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyData = new CMyData(m_color, m_nWidth);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyDataList.AddTail(m_pCurrentMyData);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함
		

		m_nowP = point;
	}

	if (m_bStick == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyStick = new CMyStick(m_color, m_nWidth, m_nStyle);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyStickList.AddTail(m_pCurrentMyStick);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함
		

		m_nowP = point;
	}

	if (m_bEllipse == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyEllipse = new CMyEllipse(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyEllipseList.AddTail(m_pCurrentMyEllipse);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bRect == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyRect = new CMyRect(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyRectList.AddTail(m_pCurrentMyRect);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bRoundRect == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyRoundRect = new CMyRoundRect(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyRoundRectList.AddTail(m_pCurrentMyRoundRect);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bTriangle == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyTriangle = new CMyTriangle(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyTriangleList.AddTail(m_pCurrentMyTriangle);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bRightTriangle == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyRightTriangle = new CMyRightTriangle(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyRightTriangleList.AddTail(m_pCurrentMyRightTriangle);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bRhombus == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyRhombus = new CMyRhombus(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyRhombusList.AddTail(m_pCurrentMyRhombus);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bPentagon == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyPentagon = new CMyPentagon(m_color, m_nWidth, m_nStyle, m_FillColor);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyPentagonList.AddTail(m_pCurrentMyPentagon);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bPartErase == TRUE)
	{
		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyErase = new CMyErase();
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyEraseList.AddTail(m_pCurrentMyErase);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
	}

	if (m_bColorFill == TRUE)
	{
		CClientDC dc(this);
		CBrush brush(m_ColorFill);
		CBrush* oldBrush = dc.SelectObject(&brush);
	

		dc.ExtFloodFill(point.x, point.y, dc.GetPixel(point), FLOODFILLSURFACE);
		dc.SelectObject(oldBrush);
		m_bColorFill = FALSE;
		m_bMouseFill = FALSE;

		CImageToolDoc *pDoc = GetDocument();
		// 마우스가 눌린 순간에 new 연산자를 이용하여 새로운 곡선을 저장할 CMyData 객체를 생성
		m_pCurrentMyColorFill = new CMyColorFill(point, m_ColorFill);
		// CTypedPtrList 클래스의 AddTai() 함수를 호출하여 CMyData 객체 추가
		pDoc->m_MyColorFillList.AddTail(m_pCurrentMyColorFill);
		// 도큐먼트 데이터가 변경되었음을 알리기 위한 SetmodifiedFlag() 함수 호출
		pDoc->SetModifiedFlag();
		// 마우스 커서가 다른 윈도우 위로 이동해도 메시지를 계속 잡아 올 수 있도록 함


		m_nowP = point;
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

			m_pCurrentMyData->m_ptArray.Add(point);

			CPen pen(PS_SOLID, m_nWidth, m_color); // 색과 굵기
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_nowP);
			dc.LineTo(point);
			dc.SelectObject(oldPen);
			m_nowP = point;
		}
	}

	if (m_bPartErase == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);

			m_pCurrentMyErase->m_ptArray.Add(point);

			CPen pen(PS_SOLID, 15, RGB(255, 255, 255)); // 색과 굵기
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(m_nowP);
			dc.LineTo(point);
			dc.SelectObject(oldPen);
			m_nowP = point;
		}
	}

	if (m_bStick == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			m_pCurrentMyStick->m_ptArray.Add(point);
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
			m_pCurrentMyRect->m_ptArray.Add(point);
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
			m_pCurrentMyEllipse->m_ptArray.Add(point);
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
			m_pCurrentMyRoundRect->m_ptArray.Add(point);
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
			m_pCurrentMyTriangle->m_ptArray.Add(point);
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

			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar2, 3);


			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bRightTriangle == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			m_pCurrentMyRightTriangle->m_ptArray.Add(point);
			// CPen pen;

			POINT ar1[] = { m_nowP.x, m_nowP.y, m_afterP.x, m_afterP.y, m_nowP.x, m_afterP.y };
			POINT ar2[] = { m_nowP.x, m_nowP.y, point.x, point.y, m_nowP.x, point.y };

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

			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar2, 3);


			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bRhombus == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			m_pCurrentMyRhombus->m_ptArray.Add(point);
			// CPen pen;
			CPoint m_temp1, m_temp2;
			m_temp1.x = m_nowP.x + (m_afterP.x - m_nowP.x) / 2;
			m_temp1.y = m_nowP.y + (m_afterP.y - m_nowP.y) / 2;

			m_temp2.x = m_nowP.x + (point.x - m_nowP.x) / 2;
			m_temp2.y = m_nowP.y + (point.y - m_nowP.y) / 2;

			POINT ar1[] = { m_temp1.x, m_nowP.y, m_afterP.x, m_temp1.y, m_temp1.x, m_afterP.y, m_nowP.x, m_temp1.y };
			POINT ar2[] = { m_temp2.x, m_nowP.y, point.x, m_temp2.y, m_temp2.x, point.y, m_nowP.x, m_temp2.y };

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
			dc.Polygon(ar1, 4); // 배열을 통해 좌표를 설정하고, 꼭짓점 수를 넣는다.

			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar2, 4);


			dc.SelectObject(oldBrush);
			dc.SelectObject(oldPen);
			// 직선의 끝점의 좌표를 갱신
			m_afterP = point;
		}
	}

	if (m_bPentagon == TRUE)
	{
		if (nFlags & MK_LBUTTON)
		{
			CClientDC dc(this);
			m_pCurrentMyPentagon->m_ptArray.Add(point);
			// CPen pen;
			CPoint m_temp1, m_temp2, m_temp3, m_temp4, m_temp5, m_temp6;
			m_temp1.x = m_nowP.x + (m_afterP.x - m_nowP.x) / 2;
			m_temp1.y = m_nowP.y + (m_afterP.y - m_nowP.y) / 2;

			m_temp2.x = m_nowP.x + (m_temp1.x - m_nowP.x) / 2;
			m_temp2.y = m_afterP.y;

			m_temp3.x = m_temp1.x + (m_afterP.x - m_temp1.x) / 2;
			m_temp3.y = m_afterP.y;

			m_temp4.x = m_nowP.x + (point.x - m_nowP.x) / 2;
			m_temp4.y = m_nowP.y + (point.y - m_nowP.y) / 2;
			m_temp5.x = m_nowP.x + (m_temp4.x - m_nowP.x) / 2;
			m_temp6.x = m_temp4.x + (point.x - m_temp4.x) / 2;



			POINT ar1[] = { m_temp1.x, m_nowP.y, m_afterP.x, m_temp1.y, m_temp3.x, m_afterP.y, m_temp2.x, m_afterP.y, m_nowP.x, m_temp1.y };
			POINT ar2[] = { m_temp4.x, m_nowP.y, point.x, m_temp4.y, m_temp6.x, point.y, m_temp5.x, point.y, m_nowP.x, m_temp4.y };

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
			dc.Polygon(ar1, 5); // 배열을 통해 좌표를 설정하고, 꼭짓점 수를 넣는다.

			// 새로운 직선을 그린다.
			dc.SetROP2(R2_NOT);
			dc.Polygon(ar2, 5);


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

//void CImageToolView::OnDrawline()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	m_nLine = TRUE;
//}


void CImageToolView::OnDrawLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bMouseDraw = !m_bMouseDraw;
	m_bMousePoly = FALSE;
	m_bMouseErase = FALSE;
	m_bMouseFill = FALSE;
	
	m_nLine = !m_nLine;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
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

void CImageToolView::OnEllipse()
{	
	if (m_nMousetempPoly != 2)
	{
		m_nMousetempPoly = 2;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseErase = FALSE;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;

	m_bEllipse = !m_bEllipse;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;

}


void CImageToolView::OnRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nMousetempPoly != 3)
	{
		m_nMousetempPoly = 3;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseErase = FALSE;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;

	m_bRect = !m_bRect;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;

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
	m_bAErase = TRUE;
	Invalidate(TRUE);
}


void CImageToolView::OnParterase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bMouseErase = !m_bMouseErase;
	m_bMousePoly = FALSE;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;

	m_bPartErase = !m_bPartErase;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
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
	if (m_nMousetempPoly != 1)
	{
		m_nMousetempPoly = 1;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bStick = !m_bStick;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
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
	if (m_nMousetempPoly != 4)
	{
		m_nMousetempPoly = 4;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bRoundRect = !m_bRoundRect;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnTriangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nMousetempPoly != 5)
	{
		m_nMousetempPoly = 5;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bTriangle = !m_bTriangle;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnRighttri()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nMousetempPoly != 6)
	{
		m_nMousetempPoly = 6;
		m_bMousePoly = FALSE;
	}
	m_bRightTriangle = !m_bRightTriangle;
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}

void CImageToolView::OnRhombus()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nMousetempPoly != 7)
	{
		m_nMousetempPoly = 7;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bRhombus = !m_bRhombus;
	m_bPentagon = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnPentagon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nMousetempPoly != 8)
	{
		m_nMousetempPoly = 8;
		m_bMousePoly = FALSE;
	}
	m_bMousePoly = !m_bMousePoly;
	m_bMouseFill = FALSE;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;

	m_bPentagon = !m_bPentagon;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;
}


void CImageToolView::OnColorfill()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bMouseFill = !m_bMouseFill;
	m_bMouseDraw = FALSE;
	m_bMouseErase = FALSE;
	m_bMousePoly = FALSE;

	m_bColorFill = !m_bColorFill;
	m_bPentagon = FALSE;
	m_bRhombus = FALSE;
	m_bRightTriangle = FALSE;
	m_bTriangle = FALSE;
	m_bRoundRect = FALSE;
	m_bRect = FALSE;
	m_bEllipse = FALSE;
	m_bStick = FALSE;
	m_nLine = FALSE;
	m_bPartErase = FALSE;

	CColorDialog dlg(m_ColorFill, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_ColorFill = dlg.GetColor();
	}
}





BOOL CImageToolView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bMouseDraw == TRUE)
	{
		::SetCursor(m_hCursorDraw);
		return TRUE;
	}
	else if (m_bMouseFill == TRUE)
	{
		::SetCursor(m_hCursorFill);
		return TRUE;
	}
	else if (m_bMouseErase == TRUE)
	{
		::SetCursor(m_hCursorErase);
		return TRUE;
	}
	else if (m_bMousePoly == TRUE)
	{
		::SetCursor(m_hCursorPoly);
		return TRUE;
	}
	else
	{
		::SetCursor(m_hCursorDe);
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageToolView::OnAllErase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bAErase = TRUE;
	CClientDC dc(this);
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(&rect);
}
