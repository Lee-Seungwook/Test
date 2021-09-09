#pragma once

#include "IppImage.h"

// 색상 반전
void IppInverse(IppByteImage& img);
void IppInverse(IppRgbImage& img);


// 밝기 변화
void IppBrightness(IppByteImage& img, int n);
void IppBrightness(IppRgbImage& img, int n);


// 명암비 변화
void IppContrast(IppByteImage& img, int n);
void IppContrast(IppRgbImage& img, int n);


// 감마 보정
void IppGammaCorrection(IppByteImage& img, float gamma);
void IppGammaCorrection(IppRgbImage& img, float gamma);

// 히스토그램
void IppHistogram(IppByteImage& img, float histo[256]);
void IppHistogramRgb(IppRgbImage& img, float histoB[256], float histoG[256], float histoR[256]);

// 히스토그램 스트레칭
void IppHistogramStretching(IppByteImage& img);

// 히스토그램 균등화
void IppHistogramEqualization(IppByteImage& img);

// 영상의 덧셈 연산 수행
bool IppAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

// 영상의 뺄셈 연산 수행
bool IppSub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

// 영상의 평균 연산 수행
bool IppAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

// 영상의 차이 연산 수행
bool IppDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

// 영상의 AND 연산 수행
bool IppAND(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

// 영상의 OR 연산 수행
bool IppOR(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);


// 비트 평면 구하는 연산 수행
void IppBitPlane(IppByteImage& img1, IppByteImage& img2, int bit);