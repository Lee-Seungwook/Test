#pragma once
#include <afx.h>
class CMyErase :
	public CObject
{
	DECLARE_SERIAL(CMyErase)
public:
	COLORREF m_EraseColor;
	int m_nWidth; // 선의 굵기 
	//int m_nStyle; // 선 스타일
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyErase();
	virtual ~CMyErase();
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};

