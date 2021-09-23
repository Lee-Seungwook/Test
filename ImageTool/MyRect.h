#pragma once
#include <afx.h>
class CMyRect :
	public CObject
{
	DECLARE_SERIAL(CMyRect)
public:
	COLORREF m_RectColor;
	COLORREF m_FillRect;
	CPoint m_start;
	CPoint m_end;
	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyRect();
	virtual ~CMyRect();
	CMyRect(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF FillColor);
	void Serialize(CArchive& ar);
	void Setend(CPoint& end);
	void Draw(CDC *pDC);
};

