#pragma once
#include <afx.h>
class CMyPentagon :
	public CObject
{
	DECLARE_SERIAL(CMyPentagon)
public:
	COLORREF m_PentagonColor;
	COLORREF m_FillPentagon;
	CPoint m_start;
	CPoint m_end;
	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyPentagon();
	virtual ~CMyPentagon();
	CMyPentagon(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF FillColor);
	void Serialize(CArchive& ar);
	void Setend(CPoint& end);
	void Draw(CDC *pDC);
};

