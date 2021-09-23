#pragma once
#include <afx.h>
class CMyErase :
	public CObject
{
	DECLARE_SERIAL(CMyErase)
public:
	COLORREF m_EraseColor;
	int m_nWidth; // ���� ���� 
	//int m_nStyle; // �� ��Ÿ��
	CArray<CPoint, CPoint&> m_ptArray;
public:
	CMyErase();
	virtual ~CMyErase();
	void Serialize(CArchive& ar);
	void Draw(CDC *pDC);
};

