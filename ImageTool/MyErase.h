#pragma once
#include <afx.h>
class CMyErase :
	public CObject
{
	DECLARE_SERIAL(CMyErase)
public:
	int m_PenColor;
	int m_nWidth;
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyErase();
	virtual ~CMyErase();
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};

