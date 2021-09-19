#pragma once
#include <afx.h>
class CMyStick :
	public CObject
{
	DECLARE_SERIAL(CMyStick)
public:
	COLORREF m_StickColor;
	CPoint m_start;
	CPoint m_end;
	int m_nWidth; // 선의 굵기 
	int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyStick();
	virtual ~CMyStick();
	CMyStick(COLORREF PenColor, int PenWidth, int PenStyle);
	void Serialize(CArchive& ar);
	
	void Draw(CDC *pDC);
};

