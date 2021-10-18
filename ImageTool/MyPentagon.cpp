#include "pch.h"
#include "MyPentagon.h"

IMPLEMENT_SERIAL(CMyPentagon, CObject, 1)

CMyPentagon::CMyPentagon(void)
{
	m_PentagonColor = RGB(0, 0, 0);
	m_FillPentagon = RGB(255, 255, 255);
	m_nStyle = 0;
	m_nWidth = 5;
}

CMyPentagon::~CMyPentagon(void)
{

}

CMyPentagon::CMyPentagon(COLORREF PenColor, int PenWidth, int PenStyle, COLORREF Fillcolor)
{
	m_PentagonColor = PenColor;
	m_FillPentagon = Fillcolor;
	m_nWidth = PenWidth;
	m_nStyle = PenStyle;
}

void CMyPentagon::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_PentagonColor << m_nWidth << m_nStyle << m_FillPentagon;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_PentagonColor >> m_nWidth >> m_nStyle >> m_FillPentagon;
		m_ptArray.Serialize(ar);
	}
}

void CMyPentagon::Setend(CPoint& end) // 이거로 그려주는 것이 간단한 것이라 판단됨
{
	m_end = end;
}

void CMyPentagon::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_PentagonColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
	CPen *pOldPen = pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(m_FillPentagon);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	int m_nend = m_ptArray.GetSize() - 1;

	CPoint m_temp1, m_temp2, m_temp3;
	m_temp1.x = m_ptArray[0].x + (m_ptArray[m_nend].x - m_ptArray[0].x) / 2;
	m_temp1.y = m_ptArray[0].y + (m_ptArray[m_nend].y - m_ptArray[0].y) / 2;

	m_temp2.x = m_ptArray[0].x + (m_temp1.x - m_ptArray[0].x) / 2;
	m_temp2.y = m_ptArray[m_nend].y;

	m_temp3.x = m_temp1.x + (m_ptArray[m_nend].x - m_temp1.x) / 2;
	m_temp3.y = m_ptArray[m_nend].y;

	pDC->BeginPath(); // 요거랑
	pDC->MoveTo(m_temp1.x, m_ptArray[0].y);
	pDC->LineTo(m_ptArray[m_nend].x, m_temp1.y);
	pDC->LineTo(m_temp3.x, m_ptArray[m_nend].y);
	pDC->LineTo(m_temp2.x, m_ptArray[m_nend].y);
	pDC->LineTo(m_ptArray[0].x, m_temp1.y);
	pDC->LineTo(m_temp1.x, m_ptArray[0].y);
	pDC->EndPath(); // 이거랑
	pDC->StrokeAndFillPath(); // 이 친구가 없으면 도형으로 그려지지 않는다. 없으면 각각의 직선으로 그려지기 때문이다.
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(oldBrush);
}