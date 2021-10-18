#pragma once
#include "..\ImageTool\IppImage\IppImage.h"

#include "..\ImageTool\stdafx.h"

#ifdef _DLL_COLOR_
#define DLL_COLOR _declspec(dllexport)
#else		COLOR
#define DLL_COLOR __declspec(dllimport)
#endif // _DLL_COLOR_

class DLL_COLOR ColorAPI
{
public:
	void APIInverse(IppRgbImage& img);

	void APIHistogram(IppRgbImage& img, float Rhisto[256], float Ghisto[256], float Bhisto[256]);
	void APIHistogramStretching(IppRgbImage& img);
	void APIHistogramEqualization(IppRgbImage& img);

	bool APIAdd(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
	bool APISub(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
	bool APIAve(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
	bool APIDiff(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
	bool APIAND(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);
	bool APIOR(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3);

	void APIResizeNearest(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);
	void APIResizeBilinear(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);
	void APIResizeCubic(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh);

	void APIFilterMean(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIFilterWeightedMean(IppRgbImage& imgSrc, IppRgbImage& imgDst);

	void APIFilterLaplacian(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIFilterUnsharpMask(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIFilterHighboost(IppRgbImage& imgSrc, IppRgbImage& imgDst, float alpha);

	void APINoiseGaussian(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount);
	void APINoiseSaltNPepper(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount);

	void APIFilterMedian(IppRgbImage& imgSrc, IppRgbImage& imgDst);

	void APIEdgeRoberts(IppRgbImage& img, IppRgbImage& imgEdge);
	void APIEdgeSobel(IppRgbImage& img, IppRgbImage& imgEdge);

	void APIMorphologyColorErosion(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIMorphologyColorDilation(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIMorphologyColorOpening(IppRgbImage& imgSrc, IppRgbImage& imgDst);
	void APIMorphologyColorClosing(IppRgbImage& imgSrc, IppRgbImage& imgDst);

	void APIRGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I);
	void APIHSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B);

	void APIRGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
	void APIYUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);

	void APIColorSplitRGB(IppRgbImage& imgColor, IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB);
	void APIColorSplitHSI(IppRgbImage& imgColor, IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI);
	void APIColorSplitYUV(IppRgbImage& imgColor, IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV);

	bool APIColorCombineRGB(IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB, IppRgbImage& imgColor);
	bool APIColorCombineHSI(IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI, IppRgbImage& imgColor);
	bool APIColorCombineYUV(IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV, IppRgbImage& imgColor);

	void APIColorEdge(IppRgbImage& imgSrc, IppByteImage& imgEdge);
};