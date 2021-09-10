// HistogramDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"
#include "IppImage/IppConvert.h"
#include "IppImage/IppDib.h"
#include "IppImage/IppImage.h"
#include "IppImage/IppEnhance.h"


// CHistogramDlg 대화 상자

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(int) * 256);
	memset(m_HistogramB, 0, sizeof(int) * 256);
	memset(m_HistogramG, 0, sizeof(int) * 256);
	memset(m_HistogramR, 0, sizeof(int) * 256);
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CHistogramDlg::SetImage(IppDib* pDib)
{
	if (pDib != NULL && pDib->GetBitCount() == 8)
	{
		IppByteImage img;
		IppDibToImage(*pDib, img);

		// 정규화된 히스토그램을 구한다.
		float histo[256] = { 0.f, };
		IppHistogram(img, histo); // 정규화된 히스토그램을 histo 배열에 저장

		// 정규화된 히스토그램에서 최댓값을 구한다.
		float max_histo = histo[0];
		for (int i = 1; i < 256; i++)
			if (histo[i] > max_histo)
				max_histo = histo[i];

		// m_Histogram 배열의 최댓값이 100이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++)
		{
			m_Histogram[i] = static_cast<int>(histo[i] * 100 / max_histo);
		}
	}
	else if (pDib != NULL && pDib->GetBitCount() == 24)
	{
		IppRgbImage img;
		IppDibToImage(*pDib, img);

		// 정규화된 히스토그램을 구한다.
		float histoB[256] = { 0.f, };
		float histoG[256] = { 0.f, };
		float histoR[256] = { 0.f, };

		IppHistogramRgb(img, histoB, histoG, histoR); // 정규화된 히스토그램을 histo 배열에 저장

		// 정규화된 히스토그램에서 최댓값을 구한다.
		float max_histoB = histoB[0];
		float max_histoG = histoG[0];
		float max_histoR = histoR[0];

		for (int i = 1; i < 256; i++)
		{
			if (histoB[i] > max_histoB)
				max_histoB = histoB[i];
			if (histoG[i] > max_histoG)
				max_histoG = histoG[i];
			if (histoR[i] > max_histoR)
				max_histoR = histoR[i];
		}

		// m_Histogram 배열의 최댓값이 100이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++)
		{
			m_HistogramB[i] = static_cast<int>(histoB[i] * 100 / max_histoB);
			m_HistogramG[i] = static_cast<int>(histoG[i] * 100 / max_histoG);
			m_HistogramR[i] = static_cast<int>(histoR[i] * 100 / max_histoR);
		}
	}
	else
	{
		memset(m_Histogram, 0, sizeof(int) * 256);
	}
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CGdiObject* pOldpen = dc.SelectStockObject(DC_PEN);
	if (m_HistogramB[10] > 0 || m_HistogramG[40] > 0 || m_HistogramR[90] > 0 == true)
	{
		// 히스토그램 박스
		dc.SetDCPenColor(RGB(128, 128, 128));
		dc.MoveTo(20, 20);
		dc.LineTo(20, 120);
		dc.LineTo(275, 120);
		dc.LineTo(275, 20);

		dc.MoveTo(20, 140);
		dc.LineTo(20, 240);
		dc.LineTo(275, 240);
		dc.LineTo(275, 140);

		dc.MoveTo(20, 260);
		dc.LineTo(20, 360);
		dc.LineTo(275, 360);
		dc.LineTo(275, 260);

		// 빨강
		dc.SetDCPenColor(RGB(128, 0, 0));
		for (int i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 120);
			dc.LineTo(20 + i, 120 - m_HistogramR[i]);
		}

		// 그레이스케일 레벨 출력
		for (int i = 0; i < 256; i++)
		{
			dc.SetDCPenColor(RGB(i, 0, 0));
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 145);
		}
		

		// 파랑
		dc.SetDCPenColor(RGB(0, 128, 0));
		for (int i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 240);
			dc.LineTo(20 + i, 240 - m_HistogramB[i]);
		}

		// 그레이스케일 레벨 출력
		for (int i = 0; i < 256; i++)
		{
			dc.SetDCPenColor(RGB(0, i, 0));
			dc.MoveTo(20 + i, 250);
			dc.LineTo(20 + i, 265);
		}
		

		// 초록
		dc.SetDCPenColor(RGB(0, 0, 128));
		for (int i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 360);
			dc.LineTo(20 + i, 360 - m_HistogramG[i]);
		}

		// 그레이스케일 레벨 출력
		for (int i = 0; i < 256; i++)
		{
			dc.SetDCPenColor(RGB(0, 0, i));
			dc.MoveTo(20 + i, 370);
			dc.LineTo(20 + i, 385);
		}
		dc.SelectObject(pOldpen);
	}
	else
	{
		// 히스토그램 박스
		dc.SetDCPenColor(RGB(128, 128, 128));
		dc.MoveTo(20, 20);
		dc.LineTo(20, 120);
		dc.LineTo(275, 120);
		dc.LineTo(275, 20);

		// 각 그레이스케일에 해당하는 히스토그램 출력
		dc.SetDCPenColor(RGB(0, 0, 0));
		for (int i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 120);
			dc.LineTo(20 + i, 120 - m_Histogram[i]);
		}

		// 그레이스케일 레벨 출력
		for (int i = 0; i < 256; i++)
		{
			dc.SetDCPenColor(RGB(i, i, i));
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 145);
		}
		dc.SelectObject(pOldpen);
	}

}
