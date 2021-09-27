#pragma once
#include <afx.h>
class CMyColorFill :
	public CObject
{
	DECLARE_SERIAL(CMyColorFill)
public:
	COLORREF m_ColorFill;
	CPoint m_ptPoint;
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyColorFill();
	virtual ~CMyColorFill();
	CMyColorFill(CPoint ptPoint, COLORREF color);
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};

