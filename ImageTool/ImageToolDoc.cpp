﻿
// ImageToolDoc.cpp: CImageToolDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"
#include "FileNewDlg.h"
#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"

#include "BrightnessContrastDlg.h"
#include "GammaCorrectionDlg.h"

#include "HistogramDlg.h"
#include "ArithmeticLogicalDlg.h"

#include "IppFilter.h"
#include "GaussianDlg.h"
#include "HighboostDlg.h"
#include "AddNoiseDlg.h"
#include "DiffusionDlg.h"

#include "IppGeometry.h"
#include "TranslateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);


// CImageToolDoc

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CImageToolDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CImageToolDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CImageToolDoc::OnUpdateImageInverse)
	ON_COMMAND(ID_COLORIMAGE_INVERSE, &CImageToolDoc::OnColorimageInverse)
	ON_UPDATE_COMMAND_UI(ID_COLORIMAGE_INVERSE, &CImageToolDoc::OnUpdateColorimageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_UPDATE_COMMAND_UI(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnUpdateBrightnessContrast)
	ON_COMMAND(ID_COLORBRIGHTNESS_CONTRAST, &CImageToolDoc::OnColorbrightnessContrast)
	ON_UPDATE_COMMAND_UI(ID_COLORBRIGHTNESS_CONTRAST, &CImageToolDoc::OnUpdateColorbrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageToolDoc::OnGammaCorrection)
//	ON_UPDATE_COMMAND_UI(ID_GAMMA_CORRECTION, &CImageToolDoc::OnUpdateGammaCorrection)
ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
//ON_COMMAND(ID_ARIMETIC_LOGICAL, &CImageToolDoc::OnArimeticLogical)
ON_COMMAND(ID_ARITHMETIC_LOGICAL, &CImageToolDoc::OnArithmeticLogical)
ON_COMMAND(ID_BITPLANE_SLICING, &CImageToolDoc::OnBitplaneSlicing)
ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnFilterWeightedMean)
ON_COMMAND(ID__FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnFilterLaplacian)
ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnFilterUnsharpMask)
ON_COMMAND(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnFilterHighboost)
ON_COMMAND(ID_ADD_NOISE, &CImageToolDoc::OnAddNoise)
ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
ON_COMMAND(ID_FILTER_DIFFUSION, &CImageToolDoc::OnFilterDiffusion)
ON_COMMAND(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnImageTranslation)
END_MESSAGE_MAP()


// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	BOOL ret = TRUE;
	if (theApp.m_pNewDib == NULL)
	{
		CFileNewDlg dlg;
		if (dlg.DoModal() == IDOK) // 확인을 누른 경우
		{
			if (dlg.m_nType == 0) // 콤보 박스 그레이스케일
				ret = m_Dib.CreateGrayBitmap(dlg.m_nWidth, dlg.m_nHeight); // 그레이스케일 비트맵
			else // 콤보 박스 트루 컬러
				ret = m_Dib.CreateRgbBitmap(dlg.m_nWidth, dlg.m_nHeight); // 트루컬러 비트맵
		}
		else
		{
			ret = FALSE;
		}
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}

	return ret;
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc 진단

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc 명령

// 파일 열기
BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName) // 파일 경로를 매개변수로 받음
{
	if (!CDocument::OnOpenDocument(lpszPathName)) 
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	BOOL res = m_Dib.Load(CT2A(lpszPathName)); // 파일 경로 저장
	if (res)
		AfxPrintInfo(_T("[파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"),
			lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());

	return res;
}

// 파일 저장
BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Dib.Save(CT2A(lpszPathName)); // 경로의 파일을 저장
}


void CImageToolDoc::OnWindowDuplicate() // 중복창 띄우기 이벤트 처리
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxNewBitmap(m_Dib); // 기존의 클래스에 있는 영상을 새로운 창으로 띄운다.
}




void CImageToolDoc::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.IsValid()) // 영상이 있는 경우
		m_Dib.CopyToClipboard(); // 클립보드에 복사
}


void CImageToolDoc::OnImageInverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//IppByteImage img;
	//IppDibToImage(m_Dib, img); // 객체 변환
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) // 매크로 사용 (주석문 내용과 동일)
	IppInverse(img);

	//IppDib dib;
	//IppImageToDib(img, dib); // 객체 변환
	CONVERT_IMAGE_TO_DIB(img, dib) // 매크로 사용 (주석문 내용과 동일)

	AfxPrintInfo(_T("[반전] 입력 영상 : %s"), GetTitle()); // 출력창 문자열 설정
	AfxNewBitmap(dib); // 영상 새 창으로 띄움
}


void CImageToolDoc::OnUpdateImageInverse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnColorimageInverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppInverse(img);

	CONVERT_IMAGE_TO_DIB(img, dib) // 매크로 사용 (주석문 내용과 동일)
	
	AfxPrintInfo(_T("[반전] 입력 영상 : %s"), GetTitle()); // 출력창 문자열 설정
	AfxNewBitmap(dib); // 영상 새 창으로 띄움
}


void CImageToolDoc::OnUpdateColorimageInverse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnBrightnessContrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK) // 확인 버튼을 눌렀을 때
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) // 매크로 호출

		IppBrightness(img, dlg.m_nBrightness); // 밝기 조절
		IppContrast(img, dlg.m_nContrast); // 명암비 조절

		CONVERT_IMAGE_TO_DIB(img, dib)

		// 출력 창 표시 및 새 창으로 표시
		AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상 : %s, 밝기 : %d, 명암비 : %d%%"), GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateBrightnessContrast(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}




void CImageToolDoc::OnColorbrightnessContrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK) // 확인 버튼을 눌렀을 때
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)

		IppBrightness(img, dlg.m_nBrightness); // 밝기 조절
		IppContrast(img, dlg.m_nContrast); // 명암비 조절

		CONVERT_IMAGE_TO_DIB(img, dib)

		// 출력 창 표시 및 새 창으로 표시
		AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상 : %s, 밝기 : %d, 명암비 : %d%%"), GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
		AfxNewBitmap(dib);
	}
}

void CImageToolDoc::OnUpdateColorbrightnessContrast(CCmdUI *pCmdUI)

{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnGammaCorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8) {
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib)

				AfxPrintInfo(_T("[감마 보정] 입력 영상 : %s, 감마 : %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24) {
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxPrintInfo(_T("[감마 보정] 입력 영상 : %s, 감마 : %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);
		}
	}
}


//void CImageToolDoc::OnUpdateGammaCorrection(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
//}


void CImageToolDoc::OnViewHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
}


void CImageToolDoc::OnHistoStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppHistogramStretching(img);
	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppHistogramEqualization(img);
	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnArithmeticLogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) // 대화 상자를 띄우고 확인을 누르면 if 문 실행
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, img1)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, img2)
		IppByteImage img3;

		bool ret = false;

		switch (dlg.m_nFunction) // 라디오 버튼에 따름
		{
		case 0: ret = IppAdd(img1, img2, img3); break;
		case 1: ret = IppSub(img1, img2, img3); break;
		case 2: ret = IppAve(img1, img2, img3); break;
		case 3: ret = IppDiff(img1, img2, img3); break;
		case 4: ret = IppAND(img1, img2, img3); break;
		case 5: ret = IppOR(img1, img2, img3); break;
		}

		if (ret)
		{
			CONVERT_IMAGE_TO_DIB(img3, dib) // 영상 출력을 위해 비트맵이미지로 재변환
			TCHAR* op[] = { _T("덧셈"), _T("뺄셈"), _T("평균"), _T("차이"), _T("논리 AND"), _T("논리 OR") };
			AfxPrintInfo(_T("[산술 및 논리 연산] [%s] 입력 영상 #1 : %s, 입력 영상 #2 : %s"), op[dlg.m_nFunction], pDoc1->GetTitle()
			, pDoc2->GetTitle());
			AfxNewBitmap(dib); // 영상 출력
		}
		else
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
	}
}


void CImageToolDoc::OnBitplaneSlicing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgPlane;

	for (int i = 0; i < 8; i++)
	{
		IppBitPlane(img, imgPlane, i);
		CONVERT_IMAGE_TO_DIB(imgPlane, dib)
			AfxNewBitmap(dib);
	}
	AfxPrintInfo(_T("[비트 평면 분할] 입력 영상 : &s"), GetTitle());
}


void CImageToolDoc::OnFilterMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[평균 값 필터] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterWeightedMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[가우시안 필터] 입력 영상 : %s, Sigma: %4.2f"), GetTitle(), dlg.m_fSigma);
		AfxNewBitmap(dib);
	}

}


void CImageToolDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[라플라시안 필터] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[언샤프 마스크 필터] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterHighboost()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHighboostDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		//float alpha = 1.2f; // 대화상자를 새로 생성하여 알파값을 입력받기 때문에 삭제함
		IppFilterHighboost(imgSrc, imgDst, dlg.m_fAlpha);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[하이부스트 필터] 입력 영상 : %s, alpha = %4.2f"), GetTitle(), dlg.m_fAlpha);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnAddNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;

		if (dlg.m_nNoiseType == 0)
			IppNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			IppNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);
		
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* noise[] = { _T("가우시안"), _T("소금 & 후추") };
		AfxPrintInfo(_T("[잡음 추가] 입력 영상 : %s,  잡음 종류 : %s,  잡음 양 : %d"), GetTitle(), noise[dlg.m_nNoiseType], dlg.m_nAmount);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	AfxPrintInfo(_T("[미디언 필터] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFilterDiffusion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[비등방성 확산 필터] 입력 영상 : %s, Lambda : %4.2f, K : %4.2f, 반복 횟수 : %d"), 
			GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);;
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppByteImage imgDst;
			IppTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
			CONVERT_IMAGE_TO_DIB(imgDst, dib)
				AfxPrintInfo(_T("[이동 변환] 입력 영상 : %s, 가로 이동 : %d, 세로 이동 : %d"), GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
			IppRgbImage imgDst;
			IppTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
			CONVERT_IMAGE_TO_DIB(imgDst, dib)
				AfxPrintInfo(_T("[이동 변환] 입력 영상 : %s, 가로 이동 : %d, 세로 이동 : %d"), GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
			AfxNewBitmap(dib);
		}
	
	}
}
