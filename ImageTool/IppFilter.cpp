#include "pch.h"
#include "IppFilter.h"
#include "stdafx.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <algorithm>

#include "..\ChatClienLibrary\FilterAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib" )
const double PI = 3.14159265358979323846;

const float  PI_F = 3.14159265358979323846f;

FilterAPI api;

// 평균값 필터
void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIFilterMean(imgSrc, imgDst);
}

// 가중 평균값 필터
void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIFilterWeightedMean(imgSrc, imgDst);
}


// 가우시안 필터 구현
void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma)
{
	api.APIFilterGaussian(imgSrc, imgDst, sigma);
}


// 라플라시안 필터 구현
void IppFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIFilterLaplacian(imgSrc, imgDst);
}


 // 언샤프 마스크 필터링
void IppFilterUnsharpMask(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIFilterUnsharpMask(imgSrc, imgDst);
}


// 하이부스트 필터
void IppFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha)
{
	api.APIFilterHighboost(imgSrc, imgDst, alpha);
}



// 가우시안 임의 잡음 생성 
void IppNoiseGaussian(IppByteImage& imgSrc, IppByteImage& imgDst, int amount)
{
	api.APINoiseGaussian(imgSrc, imgDst, amount);
}


// 소금 후추 잡음 생성
void IppNoiseSaltNPepper(IppByteImage& imgSrc, IppByteImage& imgDst, int amount) // amount는 소금 후추 잡음이 추가될 픽셀의 양을 제어하는 역할
{
	api.APINoiseSaltNPepper(imgSrc, imgDst, amount);
}


// 미디언 필터 구현
void IppFilterMedian(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIFilterMedian(imgSrc, imgDst);
}

// 비등방성 확산 필터 구현
void IppFilterDiffusion(IppByteImage& imgSrc, IppFloatImage& imgDst, float lambda, float k, int iter)
{
	api.APIFilterDiffusion(imgSrc, imgDst, lambda, k, iter);
}