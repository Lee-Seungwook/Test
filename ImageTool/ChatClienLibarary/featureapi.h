#pragma once

#include "..\ImageTool\IppImage\IppImage.h"

#include "..\ImageTool\stdafx.h"
#include <vector>

#ifdef _DLL_FEATURE_
#define DLL_FEATURE _declspec(dllexport)
#else		FEATURE
#define DLL_FEATURE __declspec(dllimport)
#endif // _DLL_FEATURE_

class APILineParam
{
public:
	double rho;
	double ang;
	int vote; // 축적된 배열에서 누적된 값을 저장

public:
	APILineParam() : rho(0), ang(0), vote(0) {}
	APILineParam(double r, double a, int v) : rho(r), ang(a), vote(v) {}
};

inline bool operator< (const APILineParam& lhs, const APILineParam& rhs) // 연산자 재정의(축적 배열의 값을 비교하기 위해서)
{
	return lhs.vote > rhs.vote;
}

class APIPoint
{
public:
	int x;
	int y;

public:
	APIPoint() : x(0), y(0) {}
	APIPoint(int _x, int _y) : x(_x), y(_y) {}
};

class DLL_FEATURE FeatureAPI
{
public:
	void APIEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge);
	void APIEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge);
	void APIEdgeSobel(IppByteImage& img, IppByteImage& imgEdge);
	void APIEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high);
		 
	void APIHoughLine(IppByteImage& img, std::vector<APILineParam>& lines, int threshold = 60);
	void APIDrawLine(IppByteImage& img, APILineParam line, BYTE c);
	void APIDrawLine(IppByteImage& img, int x1, int y1, int x2, int y2, BYTE c);
		 
	void APIHarrisCorner(IppByteImage& img, std::vector<APIPoint>& corners, double th);
};