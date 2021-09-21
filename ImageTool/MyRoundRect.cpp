#include "pch.h"
#include "MyRoundRect.h"

IMPLEMENT_SERIAL(CMyRoundRect, CObject, 1)

CMyRoundRect::CMyRoundRect(void)
{
	m_RoundRectColor = RGB(0, 0, 0);
	m_FillRoundRect = RGB(255, 255, 255);
	m_nStyle = 0;
	m_nWidth = 5;
}

CMyRoundRect::~CMyRoundRect(void)
{

}

CMyRoundRect::CMyRoundRect(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF Fillcolor)
{
	m_RoundRectColor = PenColor;
	m_FillRoundRect = Fillcolor;
	m_nWidth = PenWidth;
	m_nStyle = PenStyle;
}

void CMyRoundRect::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_RoundRectColor << m_nWidth << m_nStyle << m_FillRoundRect;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_RoundRectColor >> m_nWidth >> m_nStyle >> m_FillRoundRect;
		m_ptArray.Serialize(ar);
	}
}

void CMyRoundRect::Setend(CPoint& end) // 이거로 그려주는 것이 간단한 것이라 판단됨
{
	m_end = end;
}

void CMyRoundRect::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_RoundRectColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
	CPen *pOldPen = pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(m_FillRoundRect);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	int m_nend = m_ptArray.GetSize() - 1;

	pDC->RoundRect(m_ptArray[0].x, m_ptArray[0].y, m_ptArray[m_nend].x, m_ptArray[m_nend].y, 50, 50);
	/*for (int i = 1; i < m_ptArray.GetSize(); i++)
	{
		pDC->LineTo(m_ptArray[i]);
	}*/
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(oldBrush);
}