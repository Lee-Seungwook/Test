#pragma once

#include "IppImage/IppImage.h"

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