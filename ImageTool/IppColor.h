#pragma once

#include "IppImage/IppImage.h"

void IppInverse(IppRgbImage& img);

void IppHistogram(IppRgbImage& img, float Rhisto[256], float Ghisto[256], float Bhisto[256]);
void IppHistogramStretching(IppRgbImage& img);
void IppHistogramEqualization(IppRgbImage& img);

bool IppAdd(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
bool IppSub(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
bool IppAve(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
bool IppDiff(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
bool IppAND(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
bool IppOR(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);

void IppResizeNearest(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);
void IppResizeBilinear(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);
void IppResizeCubic(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);

void IppFilterMean(IppRgbImage& imgSrc, IppRgbImage& imgDst);
void IppFilterWeightedMean(IppRgbImage& imgSrc, IppRgbImage& imgDst);

void IppFilterLaplacian(IppRgbImage& imgSrc, IppRgbImage& imgDst);
void IppFilterUnsharpMask(IppRgbImage& imgSrc, IppRgbImage& imgDst);
void IppFilterHighboost(IppRgbImage& imgSrc, IppRgbImage& imgDst, float alpha);

void IppNoiseGaussian(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount);
void IppNoiseSaltNPepper(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount);

void IppFilterMedian(IppRgbImage& imgSrc, IppRgbImage& imgDst);

void IppEdgeRoberts(IppRgbImage& img, IppRgbImage& imgEdge);
void IppEdgeSobel(IppRgbImage& img, IppRgbImage& imgEdge);

void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I);
void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B);

void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);

void IppColorSplitRGB(IppRgbImage& imgColor, IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB);
void IppColorSplitHSI(IppRgbImage& imgColor, IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI);
void IppColorSplitYUV(IppRgbImage& imgColor, IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV);

bool IppColorCombineRGB(IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB, IppRgbImage& imgColor);
bool IppColorCombineHSI(IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI, IppRgbImage& imgColor);
bool IppColorCombineYUV(IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV, IppRgbImage& imgColor);

void IppColorEdge(IppRgbImage& imgSrc, IppByteImage& imgEdge);
