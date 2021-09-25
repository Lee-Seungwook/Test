#include "pch.h"
#include "MyColorFill.h"

IMPLEMENT_SERIAL(CMyColorFill, CObject, 1)

CMyColorFill::CMyColorFill(void)
{
	m_ColorFill = RGB(255, 255, 255);
}

CMyColorFill::~CMyColorFill(void)
{

}

CMyColorFill::CMyColorFill(CPoint ptpoint, COLORREF color)
{
	m_ColorFill = color;
	m_ptPoint = ptpoint;
}

void CMyColorFill::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_ColorFill << m_ptPoint;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_ColorFill >> m_ptPoint;
		m_ptArray.Serialize(ar);
	}
}

void CMyColorFill::Draw(CDC *pDC)
{
	CBrush brush(m_ColorFill);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->ExtFloodFill(m_ptPoint.x, m_ptPoint.y, pDC->GetPixel(m_ptPoint), FLOODFILLSURFACE);
	pDC->SelectObject(oldBrush);
}
