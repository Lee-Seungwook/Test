#pragma once

#include "IppImage/IppImage.h"

// 이동 변환
void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy);
void IppTranslate(IppRgbImage& imgSrc, IppRgbImage& imgDst, int sx, int sy);

// 최근방 이웃 보간법
void IppResizeNearest(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);

// 양선형 보간법
void IppResizeBilinear(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);

// 3차 회선 보간법
void IppResizeCubic(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);
double cubic_interpolation(double v1, double v2, double v3, double v4, double d); // 행의 4개의 점을 이용한 보간법을 이용하여 값을 반환

