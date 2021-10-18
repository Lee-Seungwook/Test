#pragma once
#include "..\ImageTool\stdafx.h"
#include "..\ImageTool\IppImage\IppImage.h"

#ifdef _DLL_FILTER_
#define DLL_FILTER _declspec(dllexport)
#else		
#define DLL_FILTER __declspec(dllimport)
#endif // _DLL_FILTER_

class DLL_FILTER FilterAPI
{
public:
	// 필터 구현
// 평균 값 필터 구현
	void APIFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc 영상에 3 x 3 크기의 마스크를 이용한 평균값 필터링을 수행, imgDst에 저장

	// 가중 평균 값 필터링 수행
	void APIFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc 영상에 3 x 3 크기의 마스크를 이용한 가중 평균값 필터링을 수행, imgDst에 저장

	// 가우시안 필터 구현
	void APIFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma); // imgsrc 영상에 표준 편차가 sigma인 가우시안 필터링을 수행, imgDst에 저장

	// 라플라시안 필터
	void APIFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst);

	// 언사프 마스크 필터
	void APIFilterUnsharpMask(IppByteImage& imgSrc, IppByteImage& imgDst);

	// 하이부스트 필터
	void APIFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha);

	// 잡음 추가
	// 가우시안 잡음 추가
	void APINoiseGaussian(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);

	// 소금, 후추 잡음 추가
	void APINoiseSaltNPepper(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);


	// 잡음 제거 
	// 미디언 필터링 함수 추가 
	void APIFilterMedian(IppByteImage& imgSrc, IppByteImage& imgDst);

	// 비등방성 확산 필터 함수 추가
	void APIFilterDiffusion(IppByteImage& imgSrc, IppFloatImage& imgDst, float lambda, float k, int iter);
};