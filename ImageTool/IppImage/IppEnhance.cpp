#include "..\stdafx.h"
#include "IppEnhance.h"
#include "..\pch.h"
#include "math.h"

#include "..\ChatClienLibarary\EnhanceAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib" )

EnhanceAPI api;

// 그레이 스케일 영상 반전
void IppInverse(IppByteImage& img)
{
	api.APIINVERSE(img);
}


// 그레이 스케일 영상 밝기 조절
void IppBrightness(IppByteImage& img, int n)
{
	api.APIBrightness(img, n);
}

 // 트루 컬러 영상 밝기 조절
void IppBrightness(IppRgbImage& img, int n)
{
	api.APICBrightness(img, n);
}

// 그레이 스케일 명암비 조절
void IppContrast(IppByteImage& img, int n)
{
	api.APIContrast(img, n);
}

// 트루컬러 명암비 조절
void IppContrast(IppRgbImage& img, int n)
{
	api.APICContrast(img, n);
}


// 그레이 스케일 감마 보정 연산
void IppGammaCorrection(IppByteImage& img, float gamma)
{
	api.APIGammaCorrection(img, gamma);
}
// pow는 지수 연산을 위해 사용(전자가 거듭제곱할 숫자, 후자가 거듭제곱 지정하는 숫자)

// 트루 컬러 감마 연산
void IppGammaCorrection(IppRgbImage& img, float gamma)
{
	api.APICGammaCorrection(img, gamma);
}

// 히스토그램
void IppHistogram(IppByteImage& img, float histo[256])
{
	api.APIHistogram(img, histo);
}

// 히스토그램 스트레칭 함수
void IppHistogramStretching(IppByteImage& img)
{
	api.APIHistogramStrectching(img);
}

// 히스토그램 균등화
void IppHistogramEqualization(IppByteImage& img)
{
	api.APIHistogramEqualization(img);
}

// 영상 덧셈 연산 구현
bool IppAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APIAdd(img1, img2, img3);

	return true;
}

// 영상 뺄셈 연산 구현
bool IppSub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APISub(img1, img2, img3);

	return true;
}

// 영상 평균 연산 수행
bool IppAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APIAve(img1, img2, img3);

	return true;
}

// 영상 차이 연산 수행 (문자 그대로 차이점을 계산한다.)
bool IppDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APIDiff(img1, img2, img3);

	return true;
}

// 영상의 AND 연산 수행
bool IppAND(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APIAND(img1, img2, img3);

	return true;
}

// 영상의 OR 연산 수행
bool IppOR(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	api.APIOR(img1, img2, img3);

	return true;
}

// 비트 평면 구하기
void IppBitPlane(IppByteImage& img1, IppByteImage& img2, int bit)
{
	api.APIBitPlane(img1, img2, bit);
}