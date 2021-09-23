#pragma once
#include <afx.h>
class CMyRhombus :
	public CObject
{
	DECLARE_SERIAL(CMyRhombus)
public:
	COLORREF m_RhombusColor;
	COLORREF m_FillRhombus;
	CPoint m_start;
	CPoint m_end;
	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyRhombus();
	virtual ~CMyRhombus();
	CMyRhombus(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF FillColor);
	void Serialize(CArchive& ar);
	void Setend(CPoint& end);
	void Draw(CDC *pDC);
};

