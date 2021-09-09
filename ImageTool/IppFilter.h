#pragma once

#include "IppImage/IppImage.h"

// 필터 구현
// 평균 값 필터 구현
void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc 영상에 3 x 3 크기의 마스크를 이용한 평균값 필터링을 수행, imgDst에 저장

// 가중 평균 값 필터링 수행
void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc 영상에 3 x 3 크기의 마스크를 이용한 가중 평균값 필터링을 수행, imgDst에 저장

// 가우시안 필터 구현
void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma); // imgsrc 영상에 표준 편차가 sigma인 가우시안 필터링을 수행, imgDst에 저장

// 라플라시안 필터
void IppFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst);

// 언사프 마스크 필터
void IppFilterUnsharpMask(IppByteImage& imgSrc, IppByteImage& imgDst);

// 하이부스트 필터
void IppFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha);

// 잡음 추가
// 가우시안 잡음 추가
void IppNoiseGaussian(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);

// 소금, 후추 잡음 추가
void IppNoiseSaltNPepper(IppByteImage& imgSrc, IppByteImage& imgDst, int amount);


// 잡음 제거 
// 미디언 필터링 함수 추가 
void IppFilterMedian(IppByteImage& imgSrc, IppByteImage& imgDst);

// 비등방성 확산 필터 함수 추가
void IppFilterDiffusion(IppByteImage& imgSrc, IppFloatImage& imgDst, float lambda, float k, int iter);