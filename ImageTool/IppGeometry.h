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


// 영상의 회전 변환
void IppRotate(IppByteImage& imgSrc, IppByteImage& imgDst, double angle); // angle은 시계방향 회전 각도

// 영상의 90도 회전 변환
void IppRotate90(IppByteImage& imgSrc, IppByteImage& imgDst);
// 영상의 180도 회전 변환
void IppRotate180(IppByteImage& imgSrc, IppByteImage& imgDst);
// 영상의 270도 회전 변환
void IppRotate270(IppByteImage& imgSrc, IppByteImage& imgDst);


// 영상의 좌우 반전
void IppMirror(IppByteImage& imgSrc, IppByteImage& imgDst);

// 영상의 상하 반전
void IppFlip(IppByteImage& imgSrc, IppByteImage& imgDst);
