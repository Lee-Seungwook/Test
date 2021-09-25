
// ImageToolView.h: CImageToolView 클래스의 인터페이스
//

#pragma once

struct Line
{
	CPoint ptFrom;
	CPoint ptTo;
	COLORREF color;
	int width;
};

class CImageToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CImageToolView() noexcept;
	DECLARE_DYNCREATE(CImageToolView)

	// 특성입니다.
public:
	CImageToolDoc* GetDocument() const;
	CArray<Line> m_lines;
	CPoint m_ptFrom;
	CPoint m_ptTemp;
	COLORREF m_color; // 선의 색상 설정
	COLORREF m_FillColor; // 채우기 색 설정
	COLORREF m_ColorFill; // 눌러서 채우기 설정
	HCURSOR m_hCursorDe; // 기본 마우스 포인터
	HCURSOR m_hCursorDraw; // 그리가 펜 모양 포인터
	HCURSOR m_hCursorFill; // 색 채우기 페인트 모양 포인터
	HCURSOR m_hCursorErase; // 지우개 모양 포인터
	HCURSOR m_hCursorPoly; // 도형 그리기 포인터

	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	int m_nCheck; // 항목 선택 여부 확인
	int m_nMousetempPoly; // 도형 커서의 이전 명령을 저장하기 위함

	CMyData *m_pCurrentMyData;
	CMyStick *m_pCurrentMyStick;
	CMyEllipse *m_pCurrentMyEllipse;
	CMyRect *m_pCurrentMyRect;
	CMyRoundRect *m_pCurrentMyRoundRect;
	CMyTriangle *m_pCurrentMyTriangle;
	CMyRightTriangle *m_pCurrentMyRightTriangle;
	CMyRhombus *m_pCurrentMyRhombus;
	CMyPentagon *m_pCurrentMyPentagon;
	CMyErase *m_pCurrentMyErase;
	CMyColorFill *m_pCurrentMyColorFill;

// 작업입니다.
public:
	CPoint m_nowP; // 시작 좌표
	CPoint m_afterP; // 마지막 좌표
	BOOL m_bPaint;
	BOOL m_nLine; // 펜 그리기
	BOOL m_bStick; // 직선
	BOOL m_bRect; // 사각형
	BOOL m_bEllipse; // 원
	BOOL m_bRoundRect; // 둥근 사각형
	BOOL m_bTriangle; // 삼각형
	BOOL m_bRightTriangle; // 직각 삼각형
	BOOL m_bRhombus; // 마름모
	BOOL m_bPentagon; // 오각형
	BOOL m_bColorFill; // 색상 채우기 

	BOOL m_bMouseDraw; // 그리기 조건
	BOOL m_bMouseFill; // 색 채우기 조건
	BOOL m_bPartErase; // 지우개 (부분 지우기)
	BOOL m_bMouseErase; // 지우개 커서
	BOOL m_bMousePoly; // 도형 커서
	

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CImageToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_nZoom; // 확대를 위한 변수
	void SetScrollSizeToFit();



	// 상태바에 영상 정보 표시
	void ShowImageInfo(CPoint point);

	afx_msg void OnViewZoom1();
	afx_msg void OnUpdateViewZoom1(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom2();
	afx_msg void OnUpdateViewZoom2(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom3();
	afx_msg void OnUpdateViewZoom3(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom4();
	afx_msg void OnUpdateViewZoom4(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEllipse();
	afx_msg void OnRectangle();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	int m_nDrawMode;
	afx_msg void OnUpdateEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRectangle(CCmdUI *pCmdUI);
	//	afx_msg void OnDrawline();
	afx_msg void OnDrawLine();
	afx_msg void OnEndLine();
	afx_msg void OnUpdateDrawLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEndLine(CCmdUI *pCmdUI);
	//	afx_msg void OnPaint();
	afx_msg void OnDrawColor();
	afx_msg void OnThick();
	afx_msg void OnAllerase();
	afx_msg void OnParterase();
	//	afx_msg void OnPaint();
	afx_msg void OnStraightline();
	afx_msg void OnLineStyle();
	afx_msg void OnFillColor();
	afx_msg void OnRoundRect();
	afx_msg void OnTriangle();
	afx_msg void OnRighttri();
	afx_msg void OnRhombus();
	afx_msg void OnPentagon();
	afx_msg void OnColorfill();
	//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // ImageToolView.cpp의 디버그 버전
inline CImageToolDoc* CImageToolView::GetDocument() const
{
	return reinterpret_cast<CImageToolDoc*>(m_pDocument);
}
#endif


