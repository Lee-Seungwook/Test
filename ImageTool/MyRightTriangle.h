#pragma once
#include <afx.h>
class CMyRightTriangle :
	public CObject
{
	DECLARE_SERIAL(CMyRightTriangle)
public:
	COLORREF m_TriangleColor;
	COLORREF m_FillTriangle;
	CPoint m_start;
	CPoint m_end;
	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyRightTriangle();
	virtual ~CMyRightTriangle();
	CMyRightTriangle(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF FillColor);
	void Serialize(CArchive& ar);
	void Setend(CPoint& end);
	void Draw(CDC *pDC);
};

