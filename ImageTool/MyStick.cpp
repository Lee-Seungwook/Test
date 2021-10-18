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

void CMyStick :: Setend(CPoint& end) // 이거로 그려주는 것이 간단한 것이라 판단됨
{
	m_end = end;
}

void CMyStick::Draw(CDC *pDC)
{
	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = m_StickColor;
	lbr.lbHatch = 0;

	CPen pen(PS_GEOMETRIC | m_nStyle, m_nWidth, &lbr, 0, 0); // 색과 굴기
	CPen *pOldPen = pDC->SelectObject(&pen);

	int m_nend = m_ptArray.GetSize() - 1;
	pDC->MoveTo(m_ptArray[0]);
	pDC->LineTo(m_ptArray[m_nend]);
	
	pDC->SelectObject(pOldPen);
}