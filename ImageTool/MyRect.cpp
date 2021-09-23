#include "pch.h"
#include "MyRect.h"

IMPLEMENT_SERIAL(CMyRect, CObject, 1)

CMyRect::CMyRect(void)
{
	m_RectColor = RGB(0, 0, 0);
	m_FillRect = RGB(255, 255, 255);
	m_nStyle = 0;
	m_nWidth = 5;
}

CMyRect::~CMyRect(void)
{

}

CMyRect::CMyRect(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF Fillcolor)
{
	m_RectColor = PenColor;
	m_FillRect = Fillcolor;
	m_nWidth = PenWidth;
	m_nStyle = PenStyle;
}

void CMyRect::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_RectColor << m_nWidth << m_nStyle << m_FillRect;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_RectColor >> m_nWidth >> m_nStyle >> m_FillRect;
		m_ptArray.Serialize(ar);
	}
}

void CMyRect::Setend(CPoint& end) // 이거로 그려주는 것이 간단한 것이라 판단됨
{
	m_end = end;
}

void CMyRect::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_RectColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
	CPen *pOldPen = pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(m_FillRect);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	int m_nend = m_ptArray.GetSize() - 1;

	pDC->Rectangle(m_ptArray[0].x, m_ptArray[0].y, m_ptArray[m_nend].x, m_ptArray[m_nend].y);
	/*for (int i = 1; i < m_ptArray.GetSize(); i++)
	{
		pDC->LineTo(m_ptArray[i]);
	}*/
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(oldBrush);
}