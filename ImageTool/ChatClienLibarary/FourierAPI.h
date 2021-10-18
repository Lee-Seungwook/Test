#pragma once
#include "..\ImageTool\stdafx.h"
#include "..\ImageTool\IppImage\IppImage.h"

#ifdef _DLL_FOURIER_
#define DLL_FOURIER _declspec(dllexport)
#else		FOURIER
#define DLL_FOURIER __declspec(dllimport)
#endif // _DLL_FOURIER_

class DLL_FOURIER APIFourier
{
public:
	int width;
	int height;
	IppDoubleImage real; // 실수부
	IppDoubleImage imag; // 허수부

public:
	APIFourier();

	void APISetImage(IppByteImage& img);
	void APIGetImage(IppByteImage& img);
	void APIGetSpectrumImage(IppByteImage& img);
	void APIGetPhaseImage(IppByteImage& img);

	// 영상의 푸리에 변환 함수
	void APIDFT(int dir);
	void APIDFTRC(int dir);
	void APIFFT(int dir);

	// 주파수 공간에서의 필터링 함수
	void APILowPassIdeal(int cutoff);
	void APIHighPassIdeal(int cutoff);
	void APILowPassGaussian(int cutoff);
	void APIHighPassGaussian(int cutoff);
};

class DLL_FOURIER FourierAPI
{
public:
	// 전역 함수 선언
	void APIDFT1d(double* re, double* im, int N, int dir);
	void APIFFT1d(double* re, double* im, int N, int dir);
	bool APIIsPowerOf2(int n);
};


