#pragma once

#include "IppImage/IppImage.h"
#include <vector>

// 직선의 방정식 파라미터를 저장할 클래스
class IppLineParam
{
public:
	double rho;
	double ang;
	int vote; // 축적된 배열에서 누적된 값을 저장

public:
	IppLineParam() : rho(0), ang(0), vote(0) {}
	IppLineParam(double r, double a, int v) : rho(r), ang(a), vote(v) {}
};

inline bool operator< (const IppLineParam& lhs, const IppLineParam& rhs) // 연산자 재정의(축적 배열의 값을 비교하기 위해서)
{
	return lhs.vote > rhs.vote;
}

// 픽셀의 좌표를 저장하는 클래스                                                                                                                        
class IppPoint
{
public:
	int x;
	int y;

public:
	IppPoint() : x(0), y(0) {}
	IppPoint(int _x, int _y) : x(_x), y(_y) {}
};

void IppEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeSobel(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high);

void IppHoughLine(IppByteImage& img, std::vector<IppLineParam>& lines, int threshold = 60);
void IppDrawLine(IppByteImage& img, IppLineParam line, BYTE c);
void IppDrawLine(IppByteImage& img, int x1, int y1, int x2, int y2, BYTE c);

void IppHarrisCorner(IppByteImage& img, std::vector<IppPoint>& corners, double th);
