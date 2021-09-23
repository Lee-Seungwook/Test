#include "pch.h"
#include "MyFill.h"

IMPLEMENT_SERIAL(CMyFill, CObject, 1)

CMyFill::CMyFill() 
{
	m_color = RGB(0, 0, 0);
}

CMyFill::~CMyFill() {}

CMyFill::CMyFill(CPoint m_ptpoint, COLORREF m_ptcolor)
{
	m_point = m_ptpoint;
	m_color = m_ptcolor;
}

void CMyFill::Serialize(CArchive& ar) 
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_color << m_point;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_color >> m_point;
		m_ptArray.Serialize(ar);
	}
}

void CMyFill::Draw(CDC* pDc) 
{
	CBrush brush(m_color);
	CBrush* oldBrush = pDc->SelectObject(&brush);

	pDc->ExtFloodFill(m_point.x, m_point.y, pDc->GetPixel(m_point), FLOODFILLSURFACE);
	pDc->SelectObject(oldBrush);
}
