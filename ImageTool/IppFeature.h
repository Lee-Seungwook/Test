#pragma once

#include "IppImage/IppImage.h"

// 픽셀의 좌표를 저장하는 클래스                                                                                                                        
class IppPoint
{
public:
	int x;
	int y;

public:
	IppPoint() : x(0) , y(0) {}
	IppPoint(int _x, int _y) : x(_x), y(-_y) {}
};

void IppEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeSobel(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high);
