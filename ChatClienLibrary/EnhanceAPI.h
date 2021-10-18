#pragma once
#include "..\ImageTool\stdafx.h"
#include "..\ImageTool\IppImage\IppImage.h"

#ifdef _DLL_ENHANCE_
#define DLL_ENHANCE _declspec(dllexport)
#else
#define DLL_ENHANCE __declspec(dllimport)
#endif // _DLL_ENHANCE_

class DLL_ENHANCE EnhanceAPI
{
public:
	void APIINVERSE(IppByteImage& img);

	void APIBrightness(IppByteImage& img, int n);

	void APICBrightness(IppRgbImage& img, int n);

	void APIContrast(IppByteImage& img, int n);

	void APICContrast(IppRgbImage& img, int n);

	void APIGammaCorrection(IppByteImage& img, float gamma);

	void APICGammaCorrection(IppRgbImage& img, float gamma);

	void APIHistogram(IppByteImage& img, float histo[256]);

	void APIHistogramStrectching(IppByteImage& img);

	// 히스토그램 균등화
	void APIHistogramEqualization(IppByteImage& img);

	// 영상의 덧셈 연산 수행
	bool APIAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

	// 영상의 뺄셈 연산 수행
	bool APISub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

	// 영상의 평균 연산 수행
	bool APIAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

	// 영상의 차이 연산 수행
	bool APIDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

	// 영상의 AND 연산 수행
	bool APIAND(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

	// 영상의 OR 연산 수행
	bool APIOR(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);


	// 비트 평면 구하는 연산 수행
	void APIBitPlane(IppByteImage& img1, IppByteImage& img2, int bit);
};
