
// ImageToolDoc.cpp: CImageToolDoc 클래스의 구현
//

#include "pch.h"
#include "stdafx.h"
#include "framework.h"

// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"
#include "FileNewDlg.h"
#include <propkey.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <algorithm>
#include <functional>

#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"
#include "IppColor.h"

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

#include "ResizeDlg.h"
#include "RotateDlg.h"

#include "IppFourier.h"
#include "FreqFilteringDlg.h"

#include "IppFeature.h"
#include "CannyEdgeDlg.h"
#include "HarrisCornerDlg.h"
#include "ColorCombineDlg.h"

#include "IppSegment.h"
#include "BinarizationDlg.h"
#include "SelectDiffImageDlg.h"
#include "FourierDescDlg.h"

#include "ChattingClientDlg.h"

#include "UIThread.h"

#include "MyData.h"
#include "MyStick.h"
#include "MyEllipse.h"
#include "MyRect.h"
#include "MyRoundRect.h"
#include "MyTriangle.h"
#include "MyRightTriangle.h"
#include "MyRhombus.h"
#include "MyPentagon.h"
#include "MyErase.h"
#include "MyColorFill.h"

#include <atlstr.h>




#define SHOW_SPECTRUM_PHASE_IMAGE


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
ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
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
ON_COMMAND(ID_IMAGE_RESIZE, &CImageToolDoc::OnImageResize)
ON_COMMAND(ID_IMAGE_ROTATE, &CImageToolDoc::OnImageRotate)
ON_COMMAND(ID_IMAGE_MIRROR, &CImageToolDoc::OnImageMirror)
ON_COMMAND(ID_IMAGE_FLIP, &CImageToolDoc::OnImageFlip)
ON_COMMAND(ID_FOURIER_DFT, &CImageToolDoc::OnFourierDft)
ON_COMMAND(ID_FOURIER_DFTRC, &CImageToolDoc::OnFourierDftrc)
ON_COMMAND(ID_FOURIER_FFT, &CImageToolDoc::OnFourierFft)
ON_COMMAND(ID_FREQ_FILTERING, &CImageToolDoc::OnFreqFiltering)
ON_COMMAND(ID_EDGE_ROBERTS, &CImageToolDoc::OnEdgeRoberts)
ON_COMMAND(ID_EDGE_PREWITT, &CImageToolDoc::OnEdgePrewitt)
ON_COMMAND(ID_EDGE_SOBEL, &CImageToolDoc::OnEdgeSobel)
ON_COMMAND(ID_EDGE_CANNY, &CImageToolDoc::OnEdgeCanny)
ON_COMMAND(ID_HOUGH_LINE, &CImageToolDoc::OnHoughLine)
ON_COMMAND(ID_HARRIS_CORNER, &CImageToolDoc::OnHarrisCorner)
ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
ON_COMMAND(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnColorSplitRgb)
ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnUpdateColorSplitRgb)
ON_COMMAND(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnColorSplitHsi)
ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnUpdateColorSplitHsi)
ON_COMMAND(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnColorSplitYuv)
ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnUpdateColorSplitYuv)
ON_COMMAND(ID_COLOR_COMBINE_RGB, &CImageToolDoc::OnColorCombineRgb)
ON_COMMAND(ID_COLOR_COMBINE_HSI, &CImageToolDoc::OnColorCombineHsi)
ON_COMMAND(ID_COLOR_COMBINE_YUV, &CImageToolDoc::OnColorCombineYuv)
ON_COMMAND(ID_COLOR_EDGE, &CImageToolDoc::OnColorEdge)
ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CImageToolDoc::OnUpdateColorEdge)
ON_COMMAND(ID_COLOR_HISTO, &CImageToolDoc::OnColorHisto)
ON_UPDATE_COMMAND_UI(ID_COLOR_HISTO, &CImageToolDoc::OnUpdateColorHisto)
ON_COMMAND(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnSegmentBinarization)
ON_COMMAND(ID_SEGMENT_LABELING, &CImageToolDoc::OnSegmentLabeling)
ON_COMMAND(ID_CONTOUR_TACING, &CImageToolDoc::OnContourTacing)
ON_COMMAND(ID_MORPOLOGY_EROSION, &CImageToolDoc::OnMorpologyErosion)
ON_COMMAND(ID_MORPOLOGY_DILATION, &CImageToolDoc::OnMorpologyDilation)
ON_COMMAND(ID_MORPOLOGY_OPENING, &CImageToolDoc::OnMorpologyOpening)
ON_COMMAND(ID_MORPOLOGY_CLOSING, &CImageToolDoc::OnMorpologyClosing)
ON_COMMAND(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnGraymorphErosion)
ON_COMMAND(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnGraymorphDilation)
ON_COMMAND(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnGraymorphOpening)
ON_COMMAND(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnGraymorphClosing)
ON_UPDATE_COMMAND_UI(ID__FILTER_GAUSSIAN, &CImageToolDoc::OnUpdateFilterGaussian)
ON_UPDATE_COMMAND_UI(ID_FILTER_DIFFUSION, &CImageToolDoc::OnUpdateFilterDiffusion)
ON_UPDATE_COMMAND_UI(ID_FREQ_FILTERING, &CImageToolDoc::OnUpdateFreqFiltering)
ON_UPDATE_COMMAND_UI(ID_FOURIER_DFT, &CImageToolDoc::OnUpdateFourierDft)
ON_UPDATE_COMMAND_UI(ID_FOURIER_DFTRC, &CImageToolDoc::OnUpdateFourierDftrc)
ON_UPDATE_COMMAND_UI(ID_FOURIER_FFT, &CImageToolDoc::OnUpdateFourierFft)
ON_UPDATE_COMMAND_UI(ID_HOUGH_LINE, &CImageToolDoc::OnUpdateHoughLine)
ON_UPDATE_COMMAND_UI(ID_SEGMENT_LABELING, &CImageToolDoc::OnUpdateSegmentLabeling)
ON_UPDATE_COMMAND_UI(ID_CONTOUR_TACING, &CImageToolDoc::OnUpdateContourTacing)
ON_UPDATE_COMMAND_UI(ID_MORPOLOGY_CLOSING, &CImageToolDoc::OnUpdateMorpologyClosing)
ON_UPDATE_COMMAND_UI(ID_MORPOLOGY_DILATION, &CImageToolDoc::OnUpdateMorpologyDilation)
ON_UPDATE_COMMAND_UI(ID_MORPOLOGY_EROSION, &CImageToolDoc::OnUpdateMorpologyErosion)
ON_UPDATE_COMMAND_UI(ID_MORPOLOGY_OPENING, &CImageToolDoc::OnUpdateMorpologyOpening)
ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnUpdateGraymorphClosing)
ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnUpdateGraymorphDilation)
ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnUpdateGraymorphErosion)
ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnUpdateGraymorphOpening)
ON_COMMAND(ID_TRUECOLOR_EROSION, &CImageToolDoc::OnTruecolorErosion)
ON_COMMAND(ID_TRUECOLOR_DILATION, &CImageToolDoc::OnTruecolorDilation)
ON_COMMAND(ID_TRUECOLOR_OPENING, &CImageToolDoc::OnTruecolorOpening)
ON_COMMAND(ID_TRUECOLOR_CLOSING, &CImageToolDoc::OnTruecolorClosing)
ON_UPDATE_COMMAND_UI(ID_TRUECOLOR_EROSION, &CImageToolDoc::OnUpdateTruecolorErosion)
ON_UPDATE_COMMAND_UI(ID_TRUECOLOR_DILATION, &CImageToolDoc::OnUpdateTruecolorDilation)
ON_UPDATE_COMMAND_UI(ID_TRUECOLOR_OPENING, &CImageToolDoc::OnUpdateTruecolorOpening)
ON_UPDATE_COMMAND_UI(ID_TRUECOLOR_CLOSING, &CImageToolDoc::OnUpdateTruecolorClosing)
ON_COMMAND(ID_SEARCH_DOT, &CImageToolDoc::OnSearchDot)
ON_COMMAND(ID_SEARCH_NOISE, &CImageToolDoc::OnSearchNoise)
ON_UPDATE_COMMAND_UI(ID_BITPLANE_SLICING, &CImageToolDoc::OnUpdateBitplaneSlicing)

ON_COMMAND(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnFourierDescriptor)
ON_COMMAND(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnInvariantMoments)
ON_COMMAND(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnZernikeMoments)
ON_COMMAND(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnTemplateMatching)
END_MESSAGE_MAP()

	
struct ThreadParams
{

	IppByteImage imgT;

	IppByteImage imgTy;
	IppByteImage imgTu;
	IppByteImage imgTv;

	IppRgbImage imgC;
	IppRgbImage imgTemp = imgC;
};


CImageToolDoc theDoc;
// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	ThreadP = 0;
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
		m_MyDataList.Serialize(ar);
		m_MyStickList.Serialize(ar);
		m_MyEllipseList.Serialize(ar);
		m_MyRectList.Serialize(ar);
		m_MyRoundRectList.Serialize(ar);
		m_MyTriangleList.Serialize(ar);
		m_MyRightTriangleList.Serialize(ar);
		m_MyRhombusList.Serialize(ar);
		m_MyEraseList.Serialize(ar);
		m_MyColorFillList.Serialize(ar);
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

UINT IntThread(LPVOID targ)
{
	ThreadParams *pra = (ThreadParams*)targ;
	
	IppByteImage imgSrc = pra->imgT;
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	AfxNewBitmap(dib);
	return 0;
}

UINT CMedianThread(LPVOID targ)
{
	ThreadParams *pra1 = (ThreadParams*)targ;
	
	IppByteImage imgSrcY = pra1->imgTy;
	IppByteImage imgSrcU = pra1->imgTu;
	IppByteImage imgSrcV = pra1->imgTv;

	IppByteImage imgY;
	IppByteImage imgU;
	IppByteImage imgV;
	
	IppRgbImage imgDst;

	IppFilterMedian(imgSrcY, imgY);
	IppFilterMedian(imgSrcU, imgU);
	IppFilterMedian(imgSrcV, imgV);
	
	IppColorCombineYUV(imgY, imgU, imgV, imgDst);

	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	AfxNewBitmap(dib);
	return 0;
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
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) // 매크로 사용 (주석문 내용과 동일)
			IppInverse(img);

		//IppDib dib;
		//IppImageToDib(img, dib); // 객체 변환
		CONVERT_IMAGE_TO_DIB(img, dib) // 매크로 사용 (주석문 내용과 동일)

			AfxPrintInfo(_T("[반전] 입력 영상 : %s"), GetTitle()); // 출력창 문자열 설정
		AfxNewBitmap(dib); // 영상 새 창으로 띄움
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) // 매크로 사용 (주석문 내용과 동일)
		IppInverse(img);

		CONVERT_IMAGE_TO_DIB(img, dib) // 매크로 사용 (주석문 내용과 동일)

		AfxPrintInfo(_T("[반전] 입력 영상 : %s"), GetTitle()); // 출력창 문자열 설정
		AfxNewBitmap(dib); // 영상 새 창으로 띄움
	}
}


void CImageToolDoc::OnUpdateImageInverse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
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
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)
		AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)
		AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnArithmeticLogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) // 대화 상자를 띄우고 확인을 누르면 if 문 실행
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;

		if (pDoc1->m_Dib.GetBitCount() != pDoc2->m_Dib.GetBitCount()) {
			AfxMessageBox(_T("같은 유형의 이미지를 선택하세요!!"));
			return;
		}

		if (m_Dib.GetBitCount() == 8)
		{
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
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(pDoc1->m_Dib, img1)
			CONVERT_DIB_TO_RGBIMAGE(pDoc2->m_Dib, img2)
			IppRgbImage img3;

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
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppByteImage imgDst;
		IppFilterMean(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

			AfxPrintInfo(_T("[평균 값 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
		IppRgbImage imgDst;
		IppFilterMean(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[평균 값 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterWeightedMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
			IppByteImage imgDst;
		IppFilterWeightedMean(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

			AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
		IppRgbImage imgDst;
		IppFilterWeightedMean(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
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

void CImageToolDoc::OnUpdateFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		IppFilterLaplacian(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[라플라시안 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
		IppRgbImage imgDst;
		IppFilterLaplacian(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[라플라시안 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		IppFilterUnsharpMask(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[언샤프 마스크 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
		IppRgbImage imgDst;
		IppFilterUnsharpMask(imgSrc, imgDst);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[언샤프 마스크 필터] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnFilterHighboost()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHighboostDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
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
	else if (m_Dib.GetBitCount() == 24)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
			IppRgbImage imgDst;
			//float alpha = 1.2f; // 대화상자를 새로 생성하여 알파값을 입력받기 때문에 삭제함
			IppFilterHighboost(imgSrc, imgDst, dlg.m_fAlpha);
			CONVERT_IMAGE_TO_DIB(imgDst, dib)

			AfxPrintInfo(_T("[하이부스트 필터] 입력 영상 : %s, alpha = %4.2f"), GetTitle(), dlg.m_fAlpha);
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnAddNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
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
	else if (m_Dib.GetBitCount() == 24)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
			IppRgbImage imgDst;

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
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		ThreadParams arg1;
		
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		
		IppByteImage imgDst;
		arg1.imgT = imgSrc;
		
		AfxBeginThread(IntThread, &arg1);
		// CONVERT_IMAGE_TO_DIB(imgSrc, dib)
		AfxPrintInfo(_T("[미디언 필터] 입력 영상 : %s"), GetTitle());
		
		// AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		ThreadParams arg1;
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
		IppByteImage imgY, imgU, imgV;
		arg1.imgC = imgSrc;
		IppColorSplitYUV(imgSrc, imgY, imgU, imgV);
		arg1.imgTy = imgY;
		arg1.imgTu = imgU;
		arg1.imgTv = imgV;

		AfxBeginThread(CMedianThread, &arg1);
		
		AfxPrintInfo(_T("[미디언 필터] 입력 영상 : %s"), GetTitle());
		
	}
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

void CImageToolDoc::OnUpdateFilterDiffusion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
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


void CImageToolDoc::OnImageResize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (m_Dib.GetBitCount() == 8)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
				IppByteImage imgDst;
			switch (dlg.m_nInterpolation)
			{
			case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			}
			CONVERT_IMAGE_TO_DIB(imgDst, dib)

				TCHAR* interpolation[] = { _T("최근방 이웃 보간법"), _T("양선형 보간법"), _T("3차 회선 보간법") };
			AfxPrintInfo(_T("[크기 변환] 입력 영상 : %s, , 새 가로 크기 : %d, 새 세로 크기 : %d, 보간법 : %s")
				, GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
			AfxNewBitmap(dib);
		}
	}

	if (m_Dib.GetBitCount() == 24)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgSrc)
				IppRgbImage imgDst;
			switch (dlg.m_nInterpolation)
			{
			case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
			}
			CONVERT_IMAGE_TO_DIB(imgDst, dib)

				TCHAR* interpolation[] = { _T("최근방 이웃 보간법"), _T("양선형 보간법"), _T("3차 회선 보간법") };
			AfxPrintInfo(_T("[크기 변환] 입력 영상 : %s, , 새 가로 크기 : %d, 새 세로 크기 : %d, 보간법 : %s")
				, GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnImageRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CRotateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nRotate) // 라디오 버튼 선택에 따름
		{
		case 0: IppRotate90(imgSrc, imgDst); break;
		case 1: IppRotate180(imgSrc, imgDst); break;
		case 2: IppRotate270(imgSrc, imgDst); break;
		case 3: IppRotate(imgSrc, imgDst, (double)dlg.m_fAngle); break;
		}
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* rotate[] = { _T("90도"), _T("180도"), _T("270도") };
		if (dlg.m_nRotate != 3) // 각도 정해놓은 함수인 경우
			AfxPrintInfo(_T("[회전 변환] 입력 영상 : %s, 회전 각도 : %s"), GetTitle(),rotate[dlg.m_nRotate]);
		else // 각도 정해놓은 함수가 아닌 경우
			AfxPrintInfo(_T("[회전 변환] 입력 영상 : %s, 회전 각도 : %4.2f도"), GetTitle(), dlg.m_fAngle);

		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnImageMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppMirror(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	AfxPrintInfo(_T("[좌우 변환] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnImageFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFlip(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)
	AfxPrintInfo(_T("[상하 대칭] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::DeleteContents()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// IsEmpty() 함수를 호출하여 리스트가 비었을 때까지 while 루프를 돌면서
	// RemoveHead() 함수를 호출한다.
	while (!m_MyDataList.IsEmpty())
	{
		m_MyDataList.RemoveHead();
	}

	while (!m_MyStickList.IsEmpty())
	{
		m_MyStickList.RemoveHead();
	}

	while (!m_MyEllipseList.IsEmpty())
	{
		m_MyEllipseList.RemoveHead();
	}

	while (!m_MyRectList.IsEmpty())
	{
		m_MyRectList.RemoveHead();
	}

	while (!m_MyRoundRectList.IsEmpty())
	{
		m_MyRoundRectList.RemoveHead();
	}

	while (!m_MyTriangleList.IsEmpty())
	{
		m_MyTriangleList.RemoveHead();
	}

	while (!m_MyRightTriangleList.IsEmpty())
	{
		m_MyRightTriangleList.RemoveHead();
	}

	while (!m_MyRhombusList.IsEmpty())
	{
		m_MyRhombusList.RemoveHead();
	}

	while (!m_MyPentagonList.IsEmpty())
	{
		m_MyPentagonList.RemoveHead();
	}

	while (!m_MyEraseList.IsEmpty())
	{
		m_MyEraseList.RemoveHead();
	}
	
	while (!m_MyColorFillList.IsEmpty())
	{
		m_MyColorFillList.RemoveHead();
	}

	UpdateAllViews(NULL);
	CDocument::DeleteContents();
}


void CImageToolDoc::OnFourierDft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 128 * 128)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
		AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
		AfxNewBitmap(dibPhase);
#endif

	fourier.DFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)
		AfxPrintInfo(_T("[푸리에변환/DFT] 입력 영상: %s, 처리 시간: %dmsec"), GetTitle(), t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierDftrc()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	// 영상의 크기가 큰 경우 경고 메시지 출력
	if (w * h > 256 * 256)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	// 모래시계 커서로 설정
	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img); // 영상 설정

	DWORD t1 = timeGetTime(); // 연산 시간을 측정하기 위함
	fourier.DFTRC(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec); // 스펙트럼 영상 출력

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase); // 위상각 영상 출력
#endif

	fourier.DFTRC(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2); // 푸리에 변환된 영상을 복사 받는다.

	CONVERT_IMAGE_TO_DIB(img2, dib)
	AfxPrintInfo(_T("[푸리에변환/DFTRC] 입력 영상: %s, 처리 시간: %dmsec"), GetTitle(), t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFourierFft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	// 영상의 가로 세로 크기가 2의 승수인지를 검사
	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.FFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec); // 스펙트럼 영상 출력

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase); // 위상각 영상 출력
#endif

	fourier.FFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/FFT] 입력 영상 : %s, 입력 영상 크기 : %dx%d, 처리 시간 : %dmsec"), GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnFreqFiltering()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0 ~ %d)"), __min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		// 저역 통과 필터 선택
		if (dlg.m_nFilterType == 0)
		{
			// 이상적 필터 선택
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			// 가우시안 필터 선택
			else
				fourier.LowPassGaussian(dlg.m_nCutoff);
		}
		// 고역 통과 필터 선택
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		IppByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib)

		TCHAR* type[] = { _T("저역 통과 필터"), _T("고역 통과 필터") };
		TCHAR* shape[] = { _T("이상적(Ideal)"), _T("가우시안(Gaussian") };
		AfxPrintInfo(_T("[주파수 공간 필터링] 입력 영상 : %s, 필터 종류 : %s, 필터 모양 : %s, 차단 주파수 : %d"), 
			GetTitle(), type[dlg.m_nFilterType], shape[dlg.m_nFilterShape], dlg.m_nCutoff);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnEdgeRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppByteImage imgEdge;
		IppEdgeRoberts(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/로버츠] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppRgbImage imgEdge;
		IppEdgeRoberts(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/로버츠] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgEdge;
		IppEdgePrewitt(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/프리윗] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgEdge;
		IppColorEdge(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/프리윗] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppByteImage imgEdge;
		IppEdgeSobel(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/소벨] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppRgbImage imgEdge;
		IppEdgeSobel(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

			AfxPrintInfo(_T("[마스크 기반 엣지 검출/소벨] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnEdgeCanny()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCannyEdgeDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				IppByteImage imgEdge;
			IppEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
			CONVERT_IMAGE_TO_DIB(imgEdge, dib)

				AfxPrintInfo(_T("[캐니 엣지 검출] 입력 영상 : %s, sigma : %4.2f, Low Th : %4.2f, High Th : %4.2f"), GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
			AfxNewBitmap(dib);
		}
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
			IppByteImage imgGray;
			IppByteImage imgEdge;
			imgGray.Convert(imgColor);
			IppEdgeCanny(imgGray, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
			CONVERT_IMAGE_TO_DIB(imgEdge, dib)

				AfxPrintInfo(_T("[캐니 엣지 검출] 입력 영상 : %s, sigma : %4.2f, Low Th : %4.2f, High Th : %4.2f"), GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
			AfxNewBitmap(dib);
		}
	}
}

void CImageToolDoc::OnHoughLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<APILineParam> lines;
	IppHoughLine(imgEdge, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T("검출된 직선이 없습니다."));
		return;
	}

	std::sort(lines.begin(), lines.end());

	// 최대 10개의 직선만 화면에 그려줌.
	int cnt = min(10, lines.size());
	for (int i = 0; i < cnt; i++) 
		IppDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[허프 선 검출] 입력 영상: %s, 중요 직선: rho = %4.2f, angle = %4.2f,vote = %d"),
		GetTitle(), lines[0].rho, (lines[0].ang * 180 / 3.14f), lines[0].vote);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnHarrisCorner()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHarrisCornerDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				std::vector<APIPoint> corners;
			IppHarrisCorner(img, corners, dlg.m_nHarrisTh); // 입력 받은 임계값과 좌표를 저장할 배열을 넘겨줌

			BYTE** ptr = img.GetPixels2D();

			int x, y;
			for (APIPoint cp : corners)
			{
				x = cp.x;
				y = cp.y;

				// 영상에 3 x 3 점을 찍는다. (픽셀을 검은색으로 바꿔서)
				ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
				ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
				ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
			}

			CONVERT_IMAGE_TO_DIB(img, dib)
				AfxPrintInfo(_T("[해리스 코너 검출] 입력 영상 : %s, Threshold: %d, 검출된 코너 갯수 : %d"), GetTitle(), dlg.m_nHarrisTh, corners.size());
			AfxNewBitmap(dib);
		}
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
			IppByteImage img;
			img.Convert(imgColor);
			std::vector<APIPoint> corners;
			IppHarrisCorner(img, corners, dlg.m_nHarrisTh); // 입력 받은 임계값과 좌표를 저장할 배열을 넘겨줌

			BYTE** ptr = img.GetPixels2D();

			int x, y;
			for (APIPoint cp : corners)
			{
				x = cp.x;
				y = cp.y;

				// 영상에 3 x 3 점을 찍는다. (픽셀을 검은색으로 바꿔서)
				ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
				ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
				ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
			}

			CONVERT_IMAGE_TO_DIB(img, dib)
				AfxPrintInfo(_T("[해리스 코너 검출] 입력 영상 : %s, Threshold: %d, 검출된 코너 갯수 : %d"), GetTitle(), dlg.m_nHarrisTh, corners.size());
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnColorGrayscale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgGray;
	imgGray.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGray, dib)

	AfxPrintInfo(_T("[그레이스케일 변환] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgR, imgG, imgB;
	IppColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR)
	CONVERT_IMAGE_TO_DIB(imgG, dibG)
	CONVERT_IMAGE_TO_DIB(imgB, dibB)

	AfxPrintInfo(_T("[색상 평면 나누기 / RGB] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
}


void CImageToolDoc::OnUpdateColorSplitRgb(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgH, imgS, imgI;
	IppColorSplitHSI(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH)
	CONVERT_IMAGE_TO_DIB(imgS, dibS)
	CONVERT_IMAGE_TO_DIB(imgI, dibI)

	AfxPrintInfo(_T("[색상 평면 나누기 / HSI] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
}


void CImageToolDoc::OnUpdateColorSplitHsi(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY, dibY)
	CONVERT_IMAGE_TO_DIB(imgU, dibU)
	CONVERT_IMAGE_TO_DIB(imgV, dibV)

	AfxPrintInfo(_T("[색상 평면 나누기 / YUV] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
}


void CImageToolDoc::OnUpdateColorSplitYuv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorCombineRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgR)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgG)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgB)

		IppRgbImage imgColor;
		if (IppColorCombineRGB(imgR, imgG, imgB, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력 영상: R: %s, G: %s, B: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgH)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgS)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgI)

		IppRgbImage imgColor;
		if (IppColorCombineHSI(imgH, imgS, imgI, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력 영상: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgY)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgU)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgV)

		IppRgbImage imgColor;
		if (IppColorCombineYUV(imgY, imgU, imgV, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력 영상: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[컬러 엣지 검출] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorEdge(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorHisto()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(img, imgY, imgU, imgV);
	IppHistogramEqualization(imgY);
	IppRgbImage imgRes;
	IppColorCombineYUV(imgY, imgU, imgV, imgRes);
	CONVERT_IMAGE_TO_DIB(imgRes, dib);

	AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorHisto(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnSegmentBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBinarizationDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
		dlg.SetImage(m_Dib); // 입력 영상 전달
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
				IppByteImage imgRes;
			IppBinarization(img, imgRes, dlg.m_nThreshold);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

				AfxPrintInfo(_T("[이진화] 입력 영상: %s, 임계값: %d"), GetTitle(), dlg.m_nThreshold);
			AfxNewBitmap(dib);
		}
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
		IppByteImage imgGray;
		imgGray.Convert(imgColor);
		CONVERT_IMAGE_TO_DIB(imgGray, c_Dib) // 입력 영상 전달을 위해 그레이 스케일로 변환

		dlg.SetImage(c_Dib); // 입력 영상 전달
		if (dlg.DoModal() == IDOK)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
			IppByteImage img;
			img.Convert(imgColor);
			IppByteImage imgRes;
			IppBinarization(img, imgRes, dlg.m_nThreshold);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxPrintInfo(_T("[이진화] 입력 영상: %s, 임계값: %d"), GetTitle(), dlg.m_nThreshold);
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnSegmentLabeling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppIntImage imgLabel;
	std::vector<APILabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	// 객체를 감싸는 사각형 그리기
	BYTE** ptr = img.GetPixels2D();
	for (APILabelInfo& info : labels)
	{
		for (int j = info.miny; j <= info.maxy; j++)
			ptr[j][info.minx] = ptr[j][info.maxx] = 128;

		for (int i = info.minx; i <= info.maxx; i++)
			ptr[info.miny][i] = ptr[info.maxy][i] = 128;
	}

	CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[레이블링] 입력 영상 : %s, 객체 객수 : %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
	
}


void CImageToolDoc::OnContourTacing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppIntImage imgLabel;
	std::vector<APILabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	IppByteImage imgContour(img.GetWidth(), img.GetHeight());
	BYTE** ptr = imgContour.GetPixels2D();
	for (APILabelInfo& info : labels)
	{
		std::vector<APIPoint> cp;
		IppContourTracing(img, info.pixels[0].x, info.pixels[0].y, cp);

		for (APIPoint& pt : cp)
			ptr[pt.y][pt.x] = 255;

	}

	CONVERT_IMAGE_TO_DIB(imgContour, dib)

	AfxPrintInfo(_T("[외곽선 추적] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorpologyErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지 / 침식] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorpologyDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지 / 팽창] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorpologyOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지 / 열기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnMorpologyClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지 / 닫기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지 / 침식] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지 / 팽창] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지 / 열기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnGraymorphClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지 / 닫기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFreqFiltering(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateFourierDft(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateFourierDftrc(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateFourierFft(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateHoughLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateSegmentLabeling(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateContourTacing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateMorpologyClosing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateMorpologyDilation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateMorpologyErosion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateMorpologyOpening(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateGraymorphClosing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateGraymorphDilation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateGraymorphErosion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnUpdateGraymorphOpening(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnTruecolorErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppRgbImage imgDst;
	IppMorphologyColorErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[트루 컬러 모폴로지 / 침식] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnTruecolorDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppRgbImage imgDst;
	IppMorphologyColorDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[트루 컬러 모폴로지 / 팽창] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnTruecolorOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppRgbImage imgDst;
	IppMorphologyColorOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[트루 컬러 모폴로지 / 열기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnTruecolorClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppRgbImage imgDst;
	IppMorphologyColorClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[트루 컬러 모폴로지 / 닫기] 입력 영상 : %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateTruecolorErosion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnUpdateTruecolorDilation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnUpdateTruecolorOpening(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnUpdateTruecolorClosing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnSearchDot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBinarizationDlg dlg;
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgDst;
		IppByteImage imgDot;
		imgDot = img;

		dlg.SetImage(m_Dib);
		if (dlg.DoModal() == IDOK)
			IppBinarization(img, imgDst, dlg.m_nThreshold);
		else if (dlg.DoModal() == IDCANCEL)
			return;
		
		IppInverse(imgDst);

		IppByteImage imgRes;
		IppEdgeSobel(imgDst, imgRes);

		IppIntImage imgLabel;
		std::vector<APILabelInfo> labels;
		int label_cnt = IppLabeling(imgRes, imgLabel, labels);
		

		// 객체를 감싸는 사각형 그리기
		BYTE** ptr = imgDot.GetPixels2D();
		for (APILabelInfo& info : labels)
		{
			for (int j = info.miny; j <= info.maxy; j++)
				ptr[j-1][info.minx] = ptr[j][info.minx] = ptr[j+1][info.minx] = ptr[j-1][info.maxx] = ptr[j][info.maxx] = ptr[j+1][info.maxx] =  255;
			

			for (int i = info.minx; i <= info.maxx; i++)
				ptr[info.miny][i-1] = ptr[info.miny][i] = ptr[info.miny][i+1] = ptr[info.maxy][i-1] = ptr[info.maxy][i] = ptr[info.maxy][i+1] = 255;
		}
		CONVERT_IMAGE_TO_DIB(imgDot, dib)

		AfxPrintInfo(_T("[점 검출 / 데드 픽셀 검출] 입력 영상 : %s, 검출 개수 : %d"), GetTitle(), label_cnt);
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgDst;
		IppByteImage imgGray;
		
		IppRgbImage imgDot;
		imgDot = img;
		
		imgGray.Convert(img);
		CONVERT_IMAGE_TO_DIB(imgGray, c_Dib)
		
		dlg.SetImage(c_Dib);
		if (dlg.DoModal() == IDOK)
			IppBinarization(imgGray, imgDst, dlg.m_nThreshold);
		else if (dlg.DoModal() == IDCANCEL)
			return;

		
		IppInverse(imgDst);
		IppByteImage imgRes;
		IppEdgeSobel(imgDst, imgRes);
		IppIntImage imgLabel;
		std::vector<APILabelInfo> labels;
		int label_cnt = IppLabeling(imgRes, imgLabel, labels);

		
		// 객체를 감싸는 사각형 그리기
		RGBBYTE** ptr = imgDot.GetPixels2D();
		for (APILabelInfo& info : labels)
		{
			for (int j = info.miny; j <= info.maxy; j++)
				ptr[j - 1][info.minx].r = ptr[j][info.minx].r = ptr[j + 1][info.minx].r = ptr[j - 1][info.maxx].r = ptr[j][info.maxx].r = ptr[j + 1][info.maxx].r = 255;


			for (int i = info.minx; i <= info.maxx; i++)
				ptr[info.miny][i - 1].r = ptr[info.miny][i].r = ptr[info.miny][i + 1].r = ptr[info.maxy][i - 1].r = ptr[info.maxy][i].r = ptr[info.maxy][i + 1].r = 255;
		}
		
		CONVERT_IMAGE_TO_DIB(imgDot, dib)

			AfxPrintInfo(_T("[점 검출 / 데드 픽셀 검출] 입력 영상 : %s, 검출 개수 : %d"), GetTitle(), label_cnt);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnSearchNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgEdge;
	
		IppEdgeSobel(img, imgEdge);
		CONVERT_IMAGE_TO_DIB(imgEdge, tempdib)
		AfxNewBitmap(tempdib);
	
		IppByteImage imgRes;

		IppAdd(img, imgEdge, imgRes);
		
		CONVERT_IMAGE_TO_DIB(imgRes, dib) // 영상 출력을 위해 비트맵이미지로 재변환
		AfxPrintInfo(_T("[노이즈 검출] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib); // 영상 출력
		
		
		
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppRgbImage imgEdge;
		
		IppEdgeSobel(img, imgEdge);
		
		CONVERT_IMAGE_TO_DIB(imgEdge, tempdib)
		AfxNewBitmap(tempdib);

		IppRgbImage imgRes;

		IppAdd(img, imgEdge, imgRes);
		
		CONVERT_IMAGE_TO_DIB(imgRes, dib) // 영상 출력을 위해 비트맵이미지로 재변환
			AfxPrintInfo(_T("[노이즈 검출] 입력 영상 : %s"), GetTitle());
		AfxNewBitmap(dib); // 영상 출력
	}
}


void CImageToolDoc::OnUpdateBitplaneSlicing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}



void CImageToolDoc::OnFourierDescriptor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFourierDescDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppIntImage imgLabel;
		std::vector<APILabelInfo> labels;
		int label_cnt = IppLabeling(img, imgLabel, labels);

		IppByteImage imgContour;
		imgContour.CreateImage(img.GetWidth(), img.GetHeight());
		BYTE** ptr = imgContour.GetPixels2D();

		for (APILabelInfo& info : labels)
		{
			std::vector<APIPoint> cp;
			IppFourierDescriptor(img, info.pixels[0].x, info.pixels[0].y, dlg.m_nPercent, cp);

			for (APIPoint pt : cp)
			{
				ptr[pt.y][pt.x] = 255;
			}

		}

		CONVERT_IMAGE_TO_DIB(imgContour, dib)

		AfxPrintInfo(_T("[푸리에 기술자] 입력 영상 : %s, 객체 개수 : %d, 복원 비율 : %d%%"), GetTitle(), label_cnt, dlg.m_nPercent);
		AfxNewBitmap(dib);
	}

}


void CImageToolDoc::OnInvariantMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		double m[7] = { 0., };
	IppInvariantMoments(img, m);

	double mm[7] = { 0., };
	CString strMoments = _T("");
	for (int i = 0; i < 7; i++)
	{
		if (m[i] == 0) continue;
		mm[i] = abs(log(abs(m[i])));
		strMoments.AppendFormat(_T("m[%d] = %6.3lf"), i + 1, mm[i]);
		if (i < 6) strMoments.Append(_T(", "));
	}

	AfxPrintInfo(_T("[불변 모멘트] 입력 영상 : %s, 불변 모멘트(log) : %s"), GetTitle(), strMoments);
}


void CImageToolDoc::OnZernikeMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	AfxPrintInfo(_T("[저니키 모멘트] 입력 영상 : %s, 저니키 모멘트(Mag) (n <= 6):"), GetTitle());
	double zr, zi;
	CString strMoments = _T("");
	for (int n = 0; n <= 6; n++)
	{
		strMoments = _T(" ");
		for (int m = (n % 2); m <= n; m += 2)
		{
			IppZernikeMoments(img, n, m, zr, zi);
			strMoments.AppendFormat(_T("|zm(%d, %d)| = %7.3lf"), n, m, hypot(zr, zi));
			if (m < n - 1) strMoments.Append(_T(", "));
		}
		AfxPrintInfo(strMoments);
	}
}


void CImageToolDoc::OnTemplateMatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TCHAR szFilter[] = _T("Bitmap Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	IppDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("파일을 불러오지 못했습니다."));
		return;
	}
	AfxNewBitmap(dibTmpl);

	if (m_Dib.GetWidth() < dibTmpl.GetWidth() || m_Dib.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("템플릿 영상의 크기가 입력 영상보다 큽니다."));
		return;
	}

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	CONVERT_DIB_TO_BYTEIMAGE(dibTmpl, imgTmpl)
	IppIntImage imgMap;
	APIPoint dp = IppTemplateMatching(img, imgTmpl, imgMap);

#if 0
	{
		IppByteImage imgCvt(img.GetWidth(), img.GetHeight());
		BYTE* pCvt = imgCvt.GetPixels();
		int* pMap = imgMap.GEtPixels();

		int max_value = 0;
		for (int i = 0; i < img.GetSize(); i++)
		{
			if (pMap[i] > max_value) max_value = pMap[i];
		}

		if (max_value != 0)
		{
			for (int i = 0; i < img.GetSize(); i++)
			{
				pCvt[i] = pMap[i] * 255 / max_value;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgCvt, dibMap)
		AfxNewBitmap(dibMap);
	}

#endif
	{// 입력 앙상에 BOX 그리기
		int tw2 = imgTmpl.GetWidth() / 2;
		int th2 = imgTmpl.GetHeight() / 2;

		int minx = dp.x - tw2;
		int maxx = dp.x + tw2;
		int miny = dp.y - th2;
		int maxy = dp.y + th2;

		BYTE** ptr = img.GetPixels2D();

		for (int j = miny; j < maxy; j++)
			ptr[j][minx] = ptr[j][maxx] = 255;

		for (int i = minx; i < maxx; i++)
			ptr[miny][i] = ptr[maxy][i] = 255;
	}

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[템플릿 매칭] 입력 영상: %s, 템플릿 영상: %s, 검출 좌표: (%d, %d)"),
		GetTitle(), dlg.GetFileName(), dp.x, dp.y);
	AfxNewBitmap(dib);
}
