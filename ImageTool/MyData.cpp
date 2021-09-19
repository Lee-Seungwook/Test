#include "pch.h"
#include "MyData.h"

IMPLEMENT_SERIAL(CMyData, CObject, 1)

CMyData::CMyData(void)
{
	m_PenColor = RGB(0, 0, 0);
	/*m_nStyle = 0;*/
	m_nWidth = 5;
}

CMyData::~CMyData(void)
{

}

CMyData::CMyData(COLORREF PenColor, int PenWidth)
{
	m_PenColor = PenColor;
	m_nWidth = PenWidth;
}

void CMyData::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_PenColor << m_nWidth;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_PenColor >> m_nWidth;
		m_ptArray.Serialize(ar);
	}
}

void CMyData::Draw(CDC *pDC)
{
	CPen pen(PS_SOLID, m_nWidth, m_PenColor);
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(m_ptArray[0]);
	for (int i = 1; i < m_ptArray.GetSize(); i++)
	{
		pDC->LineTo(m_ptArray[i]);
	}
	pDC->SelectObject(pOldPen);
}