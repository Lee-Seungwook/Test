#include "stdafx.h"

#include "IppColor.h"
#include "IppFeature.h"
#include "IppGeometry.h"
#include "IppImage/IppEnhance.h"
#include "IppFilter.h"
#include "IppSegment.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <algorithm>

#include "ChatClienLibarary/ColorAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib" )

const double PI = 3.14159265358979323846;
const float  PI_F = 3.14159265358979323846f;

ColorAPI api;

// 색상 반전
void IppInverse(IppRgbImage& img)
{
	api.APIInverse(img);
}

// 히스토그램
void IppHistogram(IppRgbImage& img, float Rhisto[256], float Ghisto[256], float Bhisto[256])
{
	api.APIHistogram(img, Rhisto, Ghisto, Bhisto);
}

// 히스토그램 스트레칭
void IppHistogramStretching(IppRgbImage& img)
{
	api.APIHistogramStretching(img);
}

// 히스토그램 균등화
void IppHistogramEqualization(IppRgbImage& img)
{
	api.APIHistogramEqualization(img);
}

// 덧셈 연산
bool IppAdd(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APIAdd(img1, img2, img3);
}

// 뺄셈 연산
bool IppSub(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APISub(img1, img2, img3);
}

// 평균 연산 수행
bool IppAve(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APIAve(img1, img2, img3);
}

// 차이 계산
bool IppDiff(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APIDiff(img1, img2, img3);
}

// AND 연산
bool IppAND(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APIAND(img1, img2, img3);
}

// OR 연산
bool IppOR(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	return api.APIOR(img1, img2, img3);
}

// 평균 값 필터
void IppFilterMean(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIFilterMean(imgSrc, imgDst);
}

// 가중 평균값 필터링
void IppFilterWeightedMean(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIFilterWeightedMean(imgSrc, imgDst);
}

// 라플라시안 필터
void IppFilterLaplacian(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIFilterLaplacian(imgSrc, imgDst);
}

// 언샤프 마스크 필터
void IppFilterUnsharpMask(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIFilterUnsharpMask(imgSrc, imgDst);
}

// 하이부스트 필터
void IppFilterHighboost(IppRgbImage& imgSrc, IppRgbImage& imgDst, float alpha)
{
	api.APIFilterHighboost(imgSrc, imgDst, alpha);
}

// 가우시안 임의 잡음 생성
void IppNoiseGaussian(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount)
{
	api.APINoiseGaussian(imgSrc, imgDst, amount);
}

// 소금& 후추 잡음 생성
void IppNoiseSaltNPepper(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount)
{
	api.APINoiseSaltNPepper(imgSrc, imgDst, amount);
}

// 미디언 필터
void IppFilterMedian(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIFilterMedian(imgSrc, imgDst);
}

// 로버츠 엣지 검출
void IppEdgeRoberts(IppRgbImage& img, IppRgbImage& imgEdge)
{
	api.APIEdgeRoberts(img, imgEdge);
}

// 소벨 엣지 검출
void IppEdgeSobel(IppRgbImage& img, IppRgbImage& imgEdge)
{
	api.APIEdgeSobel(img, imgEdge);
}

void IppResizeNearest(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	api.APIResizeNearest(imgSrc, imgDst, nw, nh);
}

void IppResizeBilinear(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	api.APIResizeBilinear(imgSrc, imgDst, nw, nh);
}

// 모폴로지 연산
void IppMorphologyColorErosion(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIMorphologyColorErosion(imgSrc, imgDst);
}

// 모폴로지 팽창
void IppMorphologyColorDilation(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIMorphologyColorDilation(imgSrc, imgDst);
}

// 영상의 열기
void IppMorphologyColorOpening(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIMorphologyColorOpening(imgSrc, imgDst);
}

// 영상의 닫기
void IppMorphologyColorClosing(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	api.APIMorphologyColorClosing(imgSrc, imgDst);
}

void IppResizeCubic(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	api.APIResizeCubic(imgSrc, imgDst, nw, nh);
}

//-------------------------------------------------------------------------
// R, G, B, H, S, I 의 범위는 [0, 1]
//-------------------------------------------------------------------------
void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I)
{
	api.APIRGB_TO_HSI(R, G, B, H, S, I);
}

//-------------------------------------------------------------------------
// R, G, B, H, S, I 의 범위는 [0, 1]
//-------------------------------------------------------------------------
void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B)
{
	api.APIHSI_TO_RGB(H, S, I, R, G, B);

}

//-------------------------------------------------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
//-------------------------------------------------------------------------
void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V)
{
	api.APIRGB_TO_YUV(R, G, B, Y, U, V);
}

//-------------------------------------------------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
//-------------------------------------------------------------------------
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B)
{
	api.APIYUV_TO_RGB(Y, U, V, R, G, B);
}

// RGB 색상 평면 나누기
void IppColorSplitRGB(IppRgbImage& imgColor, IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB)
{
	api.APIColorSplitRGB(imgColor, imgR, imgG, imgB);
}

// HSI 색상 평면 나누기
void IppColorSplitHSI(IppRgbImage& imgColor, IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI)
{
	api.APIColorSplitHSI(imgColor, imgH, imgS, imgI);
}

// YUV 색상 평면 나누기
void IppColorSplitYUV(IppRgbImage& imgColor, IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV)
{
	api.APIColorSplitYUV(imgColor, imgY, imgU, imgV);
} 

// RGB 색상 평면 합치기
bool IppColorCombineRGB(IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB, IppRgbImage& imgColor)
{
	return api.APIColorCombineRGB(imgR, imgG, imgB, imgColor);
}

// HSI 색상 평면 합치기
bool IppColorCombineHSI(IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI, IppRgbImage& imgColor)
{
	return api.APIColorCombineHSI(imgH, imgS, imgI, imgColor);
}

// YUV 색상 평면 합치기
bool IppColorCombineYUV(IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV, IppRgbImage& imgColor)
{
	return api.APIColorCombineYUV(imgY, imgU, imgV, imgColor);
}

// 컬러 엣지 검출
void IppColorEdge(IppRgbImage& imgSrc, IppByteImage& imgEdge)
{
	api.APIColorEdge(imgSrc, imgEdge);
}

