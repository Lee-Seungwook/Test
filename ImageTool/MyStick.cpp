#include "pch.h"
#include "MyStick.h"

IMPLEMENT_SERIAL(CMyStick, CObject, 1)

CMyStick::CMyStick(void)
{
	m_StickColor = RGB(0, 0, 0);
	m_nStyle = 0;
	m_nWidth = 5;
}

CMyStick::~CMyStick(void)
{

}

CMyStick::CMyStick(COLORREF PenColor, int PenWidth, int PenStyle)
{
	m_StickColor = PenColor;
	m_nWidth = PenWidth;
	m_nStyle = PenStyle;
}

void CMyStick::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_StickColor << m_nWidth << m_nStyle;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_StickColor >> m_nWidth >> m_nStyle;
		m_ptArray.Serialize(ar);
	}
}

void CMyStick::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_StickColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // »ö°ú ±¼±â
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(m_ptArray[0]);
	for (int i = 1; i < 2; i++)
	{
		pDC->LineTo(m_ptArray[i]);
	}
	pDC->SelectObject(pOldPen);
}