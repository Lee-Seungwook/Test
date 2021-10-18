#include "pch.h"
#include "MyRhombus.h"

IMPLEMENT_SERIAL(CMyRhombus, CObject, 1)

CMyRhombus::CMyRhombus(void)
{
	m_RhombusColor = RGB(0, 0, 0);
	m_FillRhombus = RGB(255, 255, 255);
	m_nStyle = 0;
	m_nWidth = 5;
}

CMyRhombus::~CMyRhombus(void)
{

}

CMyRhombus::CMyRhombus(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF Fillcolor)
{
	m_RhombusColor = PenColor;
	m_FillRhombus = Fillcolor;
	m_nWidth = PenWidth;
	m_nStyle = PenStyle;
}

void CMyRhombus::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_RhombusColor << m_nWidth << m_nStyle << m_FillRhombus;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_RhombusColor >> m_nWidth >> m_nStyle >> m_FillRhombus;
		m_ptArray.Serialize(ar);
	}
}

void CMyRhombus::Setend(CPoint& end) // 이거로 그려주는 것이 간단한 것이라 판단됨
{
	m_end = end;
}

void CMyRhombus::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_RhombusColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
	CPen *pOldPen = pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(m_FillRhombus);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	int m_nend = m_ptArray.GetSize() - 1;

	CPoint m_temp;
	m_temp.x = m_ptArray[0].x + (m_ptArray[m_nend].x - m_ptArray[0].x) / 2;
	m_temp.y = m_ptArray[0].y + (m_ptArray[m_nend].y - m_ptArray[0].y) / 2;

	pDC->BeginPath(); // 요거랑
	pDC->MoveTo(m_temp.x, m_ptArray[0].y);
	pDC->LineTo(m_ptArray[m_nend].x, m_temp.y);
	pDC->LineTo(m_temp.x, m_ptArray[m_nend].y);
	pDC->LineTo(m_ptArray[0].x, m_temp.y);
	pDC->LineTo(m_temp.x, m_ptArray[0].y);
	pDC->EndPath(); // 이거랑
	pDC->StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(oldBrush);
}