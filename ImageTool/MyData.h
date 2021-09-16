#pragma once
#include <afx.h>
class CMyData :
	public CObject
{
	DECLARE_SERIAL(CMyData)
public:
	COLORREF m_PenColor;
	int m_nWidth; // 선의 굵기 
	//int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyData();
	virtual ~CMyData();
	CMyData(COLORREF PenColor, int PenWidth);
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};

