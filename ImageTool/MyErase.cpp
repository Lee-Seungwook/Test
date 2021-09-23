#include "pch.h"
#include "MyErase.h"

IMPLEMENT_SERIAL(CMyErase, CObject, 1)

CMyErase::CMyErase(void)
{
	m_EraseColor = RGB(255, 255, 255);
	/*m_nStyle = 0;*/
	m_nWidth = 15;
}

CMyErase::~CMyErase(void)
{

}

void CMyErase::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_nWidth;
		m_ptArray.Serialize(ar);
	}
	else {
		ar >> m_nWidth;
		m_ptArray.Serialize(ar);
	}
}

void CMyErase::Draw(CDC *pDC)
{
	CPen pen(PS_SOLID, m_nWidth, m_EraseColor);
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_COPYPEN);

	pDC->MoveTo(m_ptArray[0]);
	for (int i = 1; i < m_ptArray.GetSize(); i++)
	{
		pDC->LineTo(m_ptArray[i]);
	}
	pDC->SelectObject(pOldPen);
}