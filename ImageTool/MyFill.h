#pragma once
#include <afx.h>
class CMyFill :
	public CObject
{
	DECLARE_SERIAL(CMyFill)
public:
	COLORREF m_color;
	CPoint m_point;
	//int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyFill();
	virtual ~CMyFill();
	CMyFill(CPoint m_ptpoint, COLORREF m_ptcolor);
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};