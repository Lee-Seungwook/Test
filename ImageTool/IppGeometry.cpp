#include "stdafx.h"
#include "IppGeometry.h"
#include "math.h"

const double PI = 3.14159265358979323846;

#include "ChatClienLibarary/GeometryAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib" )

GeometryAPI api;

// 이동 변환 함수 구현
void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy)
{
	
	api.APITranslate(imgSrc, imgDst, sx, sy);
}

// 이동 변환 함수 트루컬러 구현
void IppTranslate(IppRgbImage& imgSrc, IppRgbImage& imgDst, int sx, int sy)
{
	
	api.APICTranslate(imgSrc, imgDst, sx, sy);
}


// 최근방 이웃 보간법 구현
void IppResizeNearest(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	
	api.APIResizeNearest(imgSrc, imgDst, nw, nh);
}


void IppResizeBilinear(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	
	api.APIResizeBilinear(imgSrc, imgDst, nw, nh);
}


// 3차 회선 보간법을 이용한 크기 변환 구현
void IppResizeCubic(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	
	api.APIResizeCubic(imgSrc, imgDst, nw, nh);
}


// 영상의 회전 변환 구현
void IppRotate(IppByteImage& imgSrc, IppByteImage& imgDst, double angle)
{
	
	api.APIRotate(imgSrc, imgDst, angle);
}


// 영상의 90도 회전 변환
void IppRotate90(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	
	api.APIRotate90(imgSrc, imgDst);
}

// 영상의 180도 회전 변환
void IppRotate180(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	
	api.APIRotate180(imgSrc, imgDst);
}

// 영상의 270도 회전 변환
void IppRotate270(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	
	api.APIRotate270(imgSrc, imgDst);
}


// 영상의 좌우 반전 구현
void IppMirror(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	
	api.APIMirror(imgSrc, imgDst);
}

// 영상의 상하 반전 구현
void IppFlip(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	
	api.APIFlip(imgSrc, imgDst);
}
