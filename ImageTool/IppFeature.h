#pragma once

#include "IppImage/IppImage.h"
#include <vector>

#include "ChatClienLibarary/FeatureAPI.h"
#pragma comment( lib, "ChatClienLibrary.lib" )

void IppEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeSobel(IppByteImage& img, IppByteImage& imgEdge);
void IppEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high);

void IppHoughLine(IppByteImage& img, std::vector<APILineParam>& lines, int threshold = 60);
void IppDrawLine(IppByteImage& img, APILineParam line, BYTE c);
void IppDrawLine(IppByteImage& img, int x1, int y1, int x2, int y2, BYTE c);

void IppHarrisCorner(IppByteImage& img, std::vector<APIPoint>& corners, double th);
