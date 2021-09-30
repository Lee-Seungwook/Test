#include "stdafx.h"

#include "IppColor.h"
#include "IppFeature.h"
#include "IppGeometry.h"
#include "IppImage/IppEnhance.h"
#include "IppFilter.h"
#include "IppSegment.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <algorithm>

const double PI = 3.14159265358979323846;
const float  PI_F = 3.14159265358979323846f;

// 색상 반전
void IppInverse(IppRgbImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i].r = 255 - p[i].r;
		p[i].g = 255 - p[i].g;
		p[i].b = 255 - p[i].b;
	}
}

// 히스토그램
void IppHistogram(IppRgbImage& img, float Rhisto[256], float Ghisto[256], float Bhisto[256])
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	// 히스토그램 계산
	int Rcnt[256], Gcnt[256], Bcnt[256];
	memset(Rcnt, 0, sizeof(int) * 256);
	memset(Gcnt, 0, sizeof(int) * 256);
	memset(Bcnt, 0, sizeof(int) * 256);
	for (int i = 0; i < size; i++)
	{
		// 픽셀의 개수를 계산
		Rcnt[p[i].r]++;
		Gcnt[p[i].g]++;
		Bcnt[p[i].b]++;
	}

	// 히스토그램 정규화(histogram normalization)
	for (int i = 0; i < 256; i++)
	{
		Rhisto[i] = static_cast<float>(Rcnt[i]) / size; // cnt 배열에 저장된 값을 전체 픽셀의 개수로 나누어 정규화된 히스토그램 값을 histo 배열에 저장
		Ghisto[i] = static_cast<float>(Gcnt[i]) / size;
		Bhisto[i] = static_cast<float>(Bcnt[i]) / size;
	}
}

// 히스토그램 스트레칭
void IppHistogramStretching(IppRgbImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	// 최대, 최소 그레이스케일 값 계산
	BYTE R_max, R_min, G_max, G_min, B_max, B_min;
	R_max = R_min = p[0].r; // 초기화
	G_max = G_min = p[0].g;
	B_max = B_min = p[0].b;
	for (int i = 1; i < size; i++)
	{
		if (R_max < p[i].r) R_max = p[i].r;
		if (R_min > p[i].r) R_min = p[i].r;

		if (G_max < p[i].g) R_max = p[i].g;
		if (G_min > p[i].g) R_min = p[i].g;

		if (B_max < p[i].b) R_max = p[i].b;
		if (B_min > p[i].b) R_min = p[i].b;
	}

	if (R_max == R_min || G_max == G_min || B_max == B_min)
		return;

	// 히스토그램 스트레칭
	for (int i = 0; i < size; i++)
	{
		p[i].r = (p[i].r - R_min) * 255 / (R_max - R_min); // 공식 적용
		p[i].g = (p[i].g - G_min) * 255 / (G_max - G_min); // 공식 적용
		p[i].b = (p[i].b - B_min) * 255 / (B_max - B_min); // 공식 적용
	}
}

// 히스토그램 균등화
void IppHistogramEqualization(IppRgbImage& img)
{
	int size = img.GetSize();
	RGBBYTE* p = img.GetPixels();

	// 히스토그램 계산
	float Rhist[256], Ghist[256], Bhist[256];
	IppHistogram(img, Rhist, Ghist, Bhist);

	// 히스토그램 누적 함수 계산
	float Rcdf[256] = { 0, 0, };
	float Gcdf[256] = { 0, 0, };
	float Bcdf[256] = { 0, 0, };

	Rcdf[0] = Rhist[0];
	Gcdf[0] = Ghist[0];
	Bcdf[0] = Bhist[0];

	for (int i = 1; i < 256; i++)
	{
		Rcdf[i] = Rcdf[i - 1] + Rhist[i];
		Gcdf[i] = Gcdf[i - 1] + Ghist[i];
		Bcdf[i] = Bcdf[i - 1] + Bhist[i];
	}

	// 히스토그램 균등화
	for (int i = 0; i < size; i++)
	{
		p[i].r = static_cast<BYTE>(limit(Rcdf[p[i].r] * 255));
		p[i].g = static_cast<BYTE>(limit(Gcdf[p[i].g] * 255));
		p[i].b = static_cast<BYTE>(limit(Bcdf[p[i].b] * 255));
	}
}

// 덧셈 연산
bool IppAdd(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].r = limit(p1[i].r + p2[i].r); // 덧셈 연산 수행
		p3[i].g = limit(p1[i].g + p2[i].g);
		p3[i].b = limit(p1[i].b + p2[i].b);
	}

	return true;
}

// 뺄셈 연산
bool IppSub(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].r = limit(p1[i].r - p2[i].r); // 덧셈 연산 수행
		p3[i].g = limit(p1[i].g - p2[i].g);
		p3[i].b = limit(p1[i].b - p2[i].b);
	}

	return true;
}

// 평균 연산 수행
bool IppAve(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].r = (p1[i].r + p2[i].r) / 2;
		p3[i].g = (p1[i].g - p2[i].g) / 2;
		p3[i].b = (p1[i].b - p2[i].b) / 2;
	}

	return true;
}

// 차이 계산
bool IppDiff(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	int Rdiff, Gdiff, Bdiff;
	for (int i = 0; i < size; i++)
	{
		Rdiff = p1[i].r - p2[i].r; // 영상의 뺄셈 연산 수행
		Gdiff = p1[i].g - p2[i].g; // 영상의 뺄셈 연산 수행
		Bdiff = p1[i].b - p2[i].b; // 영상의 뺄셈 연산 수행

		p3[i].r = static_cast<BYTE>((Rdiff >= 0) ? Rdiff : -Rdiff); // 절대값 변환
		p3[i].g = static_cast<BYTE>((Gdiff >= 0) ? Gdiff : -Gdiff); // 절대값 변환
		p3[i].b = static_cast<BYTE>((Bdiff >= 0) ? Bdiff : -Bdiff); // 절대값 변환
	}

	return true;
}

// AND 연산
bool IppAND(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].r = limit(p1[i].r & p2[i].r); // 덧셈 연산 수행
		p3[i].g = limit(p1[i].g & p2[i].g);
		p3[i].b = limit(p1[i].b & p2[i].b);
	}

	return true;
}

// OR 연산
bool IppOR(IppRgbImage& img1, IppRgbImage& img2, IppRgbImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight()) // 입력받은 영상의 가로 세로 크기가 동일해야 연산을 수행
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	RGBBYTE *p1 = img1.GetPixels();
	RGBBYTE *p2 = img2.GetPixels();
	RGBBYTE *p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i].r = limit(p1[i].r | p2[i].r); // 덧셈 연산 수행
		p3[i].g = limit(p1[i].g | p2[i].g);
		p3[i].b = limit(p1[i].b | p2[i].b);
	}

	return true;
}

// 평균 값 필터
void IppFilterMean(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc; // 대입 연산자 오버로딩으로 클래스간 연산 가능, 이미지 복사

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1},
	};

	int i, j, m, n, Rsum, Gsum, Bsum;
	for (j = 1; j < h - 1; j++) // 최외곽 픽셀은 계산하지 않는다.
		for (i = 1; i < w - 1; i++) // 영상 픽셀
		{
			Rsum = 0;
			Gsum = 0;
			Bsum = 0;
			for (m = 0; m < 3; m++) // 마스크 배열
				for (n = 0; n < 3; n++)
				{
					Rsum += (pSrc[j - 1 + m][i - 1 + n].r * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
					Gsum += (pSrc[j - 1 + m][i - 1 + n].g * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
					Bsum += (pSrc[j - 1 + m][i - 1 + n].b * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
				}

			pDst[j][i].r = static_cast<BYTE>(limit(Rsum / 9. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
			pDst[j][i].g = static_cast<BYTE>(limit(Gsum / 9. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
			pDst[j][i].b = static_cast<BYTE>(limit(Bsum / 9. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
		}
}

// 가중 평균값 필터링
void IppFilterWeightedMean(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc; // 대입 연산자 오버로딩으로 클래스간 연산 가능, 이미지 복사

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1},
	};

	int i, j, m, n, Rsum, Gsum ,Bsum;
	for (j = 1; j < h - 1; j++) // 최외곽 픽셀은 계산하지 않는다.
		for (i = 1; i < w - 1; i++) // 영상 픽셀
		{
			Rsum = 0;
			Gsum = 0;
			Bsum = 0;
			for (m = 0; m < 3; m++) // 마스크 배열
				for (n = 0; n < 3; n++)
				{
					Rsum += (pSrc[j - 1 + m][i - 1 + n].r * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
					Gsum += (pSrc[j - 1 + m][i - 1 + n].g * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
					Bsum += (pSrc[j - 1 + m][i - 1 + n].b * mask[m][n]); // 영상 픽셀을 1부터 시작했기 때문에 1을 빼준다.
				}

			pDst[j][i].r = static_cast<BYTE>(limit(Rsum / 16. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
			pDst[j][i].g = static_cast<BYTE>(limit(Gsum / 16. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
			pDst[j][i].b = static_cast<BYTE>(limit(Bsum / 16. + 0.5)); // 마스크의 총 합으로 나눠준다, 0.5는 반올림인듯
		}
}

// 라플라시안 필터
void IppFilterLaplacian(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h); // 결과 영상을 새로 생성함으로써 모든 픽셀 값을 0으로 초기화

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, Rsum, Gsum, Bsum;
	for (j = 1; j < h - 1; j++) // 4방향에 대한 픽셀 값을 직접 참조하여 결과 픽셀 값을 계산
		for (i = 1; i < w - 1; i++) // 최외곽 픽셀은 연산에서 제외
		{
			Rsum = pSrc[j - 1][i].r + pSrc[j][i - 1].r + pSrc[j + 1][i].r + pSrc[j][i + 1].r - 4 * pSrc[j][i].r;
			Gsum = pSrc[j - 1][i].g + pSrc[j][i - 1].g + pSrc[j + 1][i].g + pSrc[j][i + 1].g - 4 * pSrc[j][i].g;
			Bsum = pSrc[j - 1][i].b + pSrc[j][i - 1].b + pSrc[j + 1][i].b + pSrc[j][i + 1].b - 4 * pSrc[j][i].b;

			pDst[j][i].r = static_cast<BYTE>(limit(Rsum + 128));
			pDst[j][i].g = static_cast<BYTE>(limit(Gsum + 128));
			pDst[j][i].b = static_cast<BYTE>(limit(Bsum + 128));
		}
}

// 언샤프 마스크 필터
void IppFilterUnsharpMask(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, Rsum, Gsum, Bsum;
	for (j = 1; j < h - 1; j++) // 4방향에 대한 픽셀 값을 직접 참조하여 결과 픽셀 값을 계산
		for (i = 1; i < w - 1; i++)
		{
			Rsum = 5 * pSrc[j][i].r - pSrc[j - 1][i].r - pSrc[j][i - 1].r - pSrc[j + 1][i].r - pSrc[j][i + 1].r;
			Gsum = 5 * pSrc[j][i].g - pSrc[j - 1][i].g - pSrc[j][i - 1].g - pSrc[j + 1][i].g - pSrc[j][i + 1].g;
			Bsum = 5 * pSrc[j][i].b - pSrc[j - 1][i].b - pSrc[j][i - 1].b - pSrc[j + 1][i].b - pSrc[j][i + 1].b;

			pDst[j][i].r = static_cast<BYTE>(limit(Rsum));
			pDst[j][i].g = static_cast<BYTE>(limit(Gsum));
			pDst[j][i].b = static_cast<BYTE>(limit(Bsum));
		}
}

// 하이부스트 필터
void IppFilterHighboost(IppRgbImage& imgSrc, IppRgbImage& imgDst, float alpha)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	float Rsum, Gsum, Bsum;

	for (j = 1; j < h - 1; j++) // 4방향에 대한 픽셀 값을 직접 참조하여 결과 픽셀 값을 계산
		for (i = 1; i < w - 1; i++)
		{
			Rsum = (4 + alpha) * pSrc[j][i].r - pSrc[j - 1][i].r - pSrc[j][i - 1].r - pSrc[j + 1][i].r - pSrc[j][i + 1].r; // alpha 값으로 명암비를 조절한다.
			Gsum = (4 + alpha) * pSrc[j][i].g - pSrc[j - 1][i].g - pSrc[j][i - 1].g - pSrc[j + 1][i].g - pSrc[j][i + 1].g; // alpha 값으로 명암비를 조절한다.
			Bsum = (4 + alpha) * pSrc[j][i].b - pSrc[j - 1][i].b - pSrc[j][i - 1].b - pSrc[j + 1][i].b - pSrc[j][i + 1].b; // alpha 값으로 명암비를 조절한다.
			pDst[j][i].r = static_cast<BYTE>(limit(Rsum + 0.5f)); // 반올림하여 정수형으로 전환한다.
			pDst[j][i].g = static_cast<BYTE>(limit(Gsum + 0.5f)); // 반올림하여 정수형으로 전환한다.
			pDst[j][i].b = static_cast<BYTE>(limit(Bsum + 0.5f)); // 반올림하여 정수형으로 전환한다.
		}
}

// 가우시안 임의 잡음 생성
void IppNoiseGaussian(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount)
{
	int size = imgSrc.GetSize();

	imgDst = imgSrc;
	RGBBYTE* pDst = imgDst.GetPixels();

	unsigned int seed = static_cast<unsigned int>(time(NULL)); //  시스템 시간을 초 단위로 불러와 시드 값으로 사용
	std::default_random_engine generator(seed); // 
	std::normal_distribution<double> distribution(0.0, 1.0); // 가우시안 난수(정상 분포 난수),

	double rn;
	for (int i = 0; i < size; i++)
	{
		rn = distribution(generator) * 255 * amount / 100;
		pDst[i].r = static_cast<BYTE>(limit(pDst[i].r + rn));
		pDst[i].g = static_cast<BYTE>(limit(pDst[i].g + rn));
		pDst[i].b = static_cast<BYTE>(limit(pDst[i].b + rn));
	}
}

// 소금& 후추 잡음 생성
void IppNoiseSaltNPepper(IppRgbImage& imgSrc, IppRgbImage& imgDst, int amount)
{
	int size = imgSrc.GetSize();

	imgDst = imgSrc;
	RGBBYTE* pDst = imgDst.GetPixels();

	unsigned int seed = static_cast<unsigned int>(time(NULL));
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, size - 1); // 소금 후추 잡음 생성할 난수 생성

	int num = size * amount / 100; // 잡음이 추가될 픽셀의 개수를 구함
	for (int i = 0; i < num; i++)
	{
		pDst[distribution(generator)] = (i & 0x01) * 255; // 잡음이 추가될 좌표
	}
}

// 미디언 필터
void IppFilterMedian(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	BYTE mR[9], mG[9], mB[9];

	for (j = 1; j < h - 1; j++) // 중위 수를 계산하기 위한 반복문 밑 sort 함수 적용
		for (i = 1; i < w - 1; i++)
		{
			mR[0] = pSrc[j - 1][i - 1].r; mR[1] = pSrc[j - 1][i].r; mR[2] = pSrc[j - 1][i + 1].r;
			mR[3] = pSrc[j][i - 1].r; mR[4] = pSrc[j][i].r; mR[5] = pSrc[j][i + 1].r;
			mR[6] = pSrc[j + 1][i - 1].r; mR[7] = pSrc[j + 1][i].r; mR[8] = pSrc[j + 1][i + 1].r;

			mG[0] = pSrc[j - 1][i - 1].g; mG[1] = pSrc[j - 1][i].g; mG[2] = pSrc[j - 1][i + 1].g;
			mG[3] = pSrc[j][i - 1].g; mG[4] = pSrc[j][i].g; mG[5] = pSrc[j][i + 1].g;
			mG[6] = pSrc[j + 1][i - 1].g; mG[7] = pSrc[j + 1][i].g; mG[8] = pSrc[j + 1][i + 1].g;

			mB[0] = pSrc[j - 1][i - 1].b; mB[1] = pSrc[j - 1][i].b; mB[2] = pSrc[j - 1][i + 1].b;
			mB[3] = pSrc[j][i - 1].b; mB[4] = pSrc[j][i].b; mB[5] = pSrc[j][i + 1].b;
			mB[6] = pSrc[j + 1][i - 1].b; mB[7] = pSrc[j + 1][i].b; mB[8] = pSrc[j + 1][i + 1].b;

			std::sort(mR, mR + 9); // 주소를 매개변수로 사용하기 때문에 배열의 주소를 넘겨준다.
			std::sort(mG, mG + 9); // 주소를 매개변수로 사용하기 때문에 배열의 주소를 넘겨준다.
			std::sort(mB, mB + 9); // 주소를 매개변수로 사용하기 때문에 배열의 주소를 넘겨준다.

			pDst[j][i].r = mR[4];
			pDst[j][i].g = mG[4];
			pDst[j][i].b = mB[4];
		}
}

// 로버츠 엣지 검출
void IppEdgeRoberts(IppRgbImage& img, IppRgbImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	RGBBYTE** p1 = img.GetPixels2D();
	RGBBYTE** p2 = imgEdge.GetPixels2D();

	int i, j, Rh1, Rh2, Gh1, Gh2, Bh1, Bh2;
	double Rhval, Ghval, Bhval;
	for (j = 1; j < h; j++)
		for (i = 1; i < w - 1; i++)
		{
			Rh1 = p1[j][i].r - p1[j - 1][i - 1].r;
			Rh2 = p1[j][i].r - p1[j - 1][i + 1].r;

			Gh1 = p1[j][i].g - p1[j - 1][i - 1].g;
			Gh2 = p1[j][i].g - p1[j - 1][i + 1].g;

			Bh1 = p1[j][i].b - p1[j - 1][i - 1].b;
			Bh2 = p1[j][i].b - p1[j - 1][i + 1].b;

			Rhval = sqrt(static_cast<double>(Rh1 * Rh1 + Rh2 * Rh2));
			Ghval = sqrt(static_cast<double>(Gh1 * Gh1 + Gh2 * Gh2));
			Bhval = sqrt(static_cast<double>(Bh1 * Bh1 + Bh2 * Bh2));

			p2[j][i].r = static_cast<BYTE>(limit(Rhval + 0.5));
			p2[j][i].g = static_cast<BYTE>(limit(Ghval + 0.5));
			p2[j][i].b = static_cast<BYTE>(limit(Bhval + 0.5));
		}
}

// 소벨 엣지 검출
void IppEdgeSobel(IppRgbImage& img, IppRgbImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	RGBBYTE** p1 = img.GetPixels2D();
	RGBBYTE** p2 = imgEdge.GetPixels2D();

	int i, j, Rh1, Rh2, Gh1, Gh2, Bh1, Bh2;
	double Rhval, Ghval, Bhval;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			Rh1 = -p1[j - 1][i - 1].r - 2 * p1[j - 1][i].r - p1[j - 1][i + 1].r
				+ p1[j + 1][i - 1].r + 2 * p1[j + 1][i].r + p1[j + 1][i + 1].r;
			Rh2 = -p1[j - 1][i - 1].r - 2 * p1[j][i - 1].r - p1[j + 1][i - 1].r
				+ p1[j - 1][i + 1].r + 2 * p1[j][i + 1].r + p1[j + 1][i + 1].r;

			Gh1 = -p1[j - 1][i - 1].g - 2 * p1[j - 1][i].g - p1[j - 1][i + 1].g
				+ p1[j + 1][i - 1].g + 2 * p1[j + 1][i].g + p1[j + 1][i + 1].g;
			Gh2 = -p1[j - 1][i - 1].g - 2 * p1[j][i - 1].g - p1[j + 1][i - 1].g
				+ p1[j - 1][i + 1].g + 2 * p1[j][i + 1].g + p1[j + 1][i + 1].g;

			Bh1 = -p1[j - 1][i - 1].b - 2 * p1[j - 1][i].b - p1[j - 1][i + 1].b
				+ p1[j + 1][i - 1].b + 2 * p1[j + 1][i].b + p1[j + 1][i + 1].b;
			Bh2 = -p1[j - 1][i - 1].b - 2 * p1[j][i - 1].b - p1[j + 1][i - 1].b
				+ p1[j - 1][i + 1].b + 2 * p1[j][i + 1].b + p1[j + 1][i + 1].b;

			Rhval = sqrt(static_cast<double>(Rh1 * Rh1 + Rh2 * Rh2));
			Ghval = sqrt(static_cast<double>(Gh1 * Gh1 + Gh2 * Gh2));
			Bhval = sqrt(static_cast<double>(Bh1 * Bh1 + Bh2 * Bh2));

			p2[j][i].r = static_cast<BYTE>(limit(Rhval + 0.5));
			p2[j][i].g = static_cast<BYTE>(limit(Ghval + 0.5));
			p2[j][i].b = static_cast<BYTE>(limit(Bhval + 0.5));
		}
}

void IppResizeNearest(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	double rx, ry;
	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1); // 확대 비율로 나누어줌
			ry = static_cast<double>(h - 1) * j / (nh - 1);
			x = static_cast<int>(rx + 0.5); // 정수 값 변환 (반올림 하여), 참조할 원본 영상 좌표 
			y = static_cast<int>(ry + 0.5);

			if (x >= w) x = w - 1;
			if (y >= h) y = h - 1;

			pDst[j][i].r = pSrc[y][x].r;
			pDst[j][i].g = pSrc[y][x].g;
			pDst[j][i].b = pSrc[y][x].b;
		}
}

void IppResizeBilinear(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, y1, x2, y2;
	double rx, ry, p, q, Rvalue, Gvalue, Bvalue;

	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1); // 확대 비율로 나누어줌
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x1 = static_cast<int>(rx); // rx, ry 좌표를
			y1 = static_cast<int>(ry);

			x2 = x1 + 1; if (x2 == w) x2 = w - 1; // 같은 경우에는 맨 가장자리의 픽셀 값을 참조하도록 한다.
			y2 = y1 + 1; if (y2 == h) y2 = h - 1; // 둘러싼 4개의 픽셀 좌표를 나타냄

			p = rx - x1;
			q = ry - y1;

			Rvalue = (1. - p) * (1. - q) * pSrc[y1][x1].r + p * (1. - q) * pSrc[y1][x2].r
				+ (1. - p) * q * pSrc[y2][x1].r + p * q * pSrc[y2][x2].r; // 양선형 보간법을 적용하여 픽셀 값을 설정

			Gvalue = (1. - p) * (1. - q) * pSrc[y1][x1].g + p * (1. - q) * pSrc[y1][x2].g
				+ (1. - p) * q * pSrc[y2][x1].g + p * q * pSrc[y2][x2].g; // 양선형 보간법을 적용하여 픽셀 값을 설정

			Bvalue = (1. - p) * (1. - q) * pSrc[y1][x1].b + p * (1. - q) * pSrc[y1][x2].b
				+ (1. - p) * q * pSrc[y2][x1].b + p * q * pSrc[y2][x2].b; // 양선형 보간법을 적용하여 픽셀 값을 설정

			pDst[j][i].r = static_cast<BYTE>(limit(Rvalue + .5)); // 반올림하여 픽셀 값으로 저장
			pDst[j][i].g = static_cast<BYTE>(limit(Gvalue + .5)); // 반올림하여 픽셀 값으로 저장
			pDst[j][i].b = static_cast<BYTE>(limit(Bvalue + .5)); // 반올림하여 픽셀 값으로 저장
		}
}

// 모폴로지 연산
void IppMorphologyColorErosion(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int i, j, m, n, x, y, Rpmin, Gpmin, Bpmin;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	RGBBYTE** pDst = imgDst.GetPixels2D();
	RGBBYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			Rpmin = 255;
			Gpmin = 255;
			Bpmin = 255;

			for (n = -1; n <= 1; n++)
				for (m = -1; m <= 1; m++)
				{
					x = i + m;
					y = j + n;

					if (x >= 0 && x < w && y >= 0 && y < h)
					{
						if (pSrc[y][x].r < Rpmin)
							Rpmin = pSrc[y][x].r;

						if (pSrc[y][x].g < Gpmin)
							Gpmin = pSrc[y][x].g;

						if (pSrc[y][x].b < Bpmin)
							Bpmin = pSrc[y][x].b;
					}
				}

			pDst[j][i].r = Rpmin;
			pDst[j][i].g = Gpmin;
			pDst[j][i].b = Bpmin;
		}
}

// 모폴로지 팽창
void IppMorphologyColorDilation(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	int i, j, m, n, x, y, Rpmax, Gpmax, Bpmax;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	RGBBYTE** pDst = imgDst.GetPixels2D();
	RGBBYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			Rpmax = 0;
			Gpmax = 0;
			Bpmax = 0;

			for (n = -1; n <= 1; n++)
				for (m = -1; m <= 1; m++)
				{
					x = i + m;
					y = j + n;

					if (x >= 0 && x < w && y >= 0 && y < h)
					{
						if (pSrc[y][x].r > Rpmax)
							Rpmax = pSrc[y][x].r;

						if (pSrc[y][x].g > Gpmax)
							Gpmax = pSrc[y][x].g;

						if (pSrc[y][x].b > Bpmax)
							Bpmax = pSrc[y][x].b;
					}
				}

			pDst[j][i].r = Rpmax;
			pDst[j][i].g = Gpmax;
			pDst[j][i].b = Bpmax;
		}
}

// 영상의 열기
void IppMorphologyColorOpening(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	IppRgbImage imgTmp;
	IppMorphologyColorErosion(imgSrc, imgTmp);
	IppMorphologyColorDilation(imgTmp, imgDst);
}

// 영상의 닫기
void IppMorphologyColorClosing(IppRgbImage& imgSrc, IppRgbImage& imgDst)
{
	IppRgbImage imgTmp;
	IppMorphologyColorDilation(imgSrc, imgTmp);
	IppMorphologyColorErosion(imgTmp, imgDst);
}

void IppResizeCubic(IppRgbImage& imgSrc, IppRgbImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, x2, x3, x4, y1, y2, y3, y4; // 각 행의 4개의 점 총 16개의 점의 좌표를 저장할 x1 ~ y4
	double Rv1, Rv2, Rv3, Rv4, Rv;
	double Gv1, Gv2, Gv3, Gv4, Gv;
	double Bv1, Bv2, Bv3, Bv4, Bv;
	double rx, ry, p, q;

	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1); // 확대 비율로 나누어줌
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x2 = static_cast<int>(rx);
			x1 = x2 - 1; if (x1 < 0) x1 = 0;
			x3 = x2 + 1; if (x3 >= w) x3 = w - 1;
			x4 = x2 + 2; if (x4 >= w) x4 = w - 1;
			p = rx - x2;

			y2 = static_cast<int>(ry);
			y1 = y2 - 1; if (y1 < 0) y1 = 0;
			y3 = y2 + 1; if (y3 >= h) y3 = h - 1;
			y4 = y2 + 2; if (y4 >= h) y4 = h - 1;
			q = ry - y2;

			// 각 행의 점의 좌표를 계산하여 한 개의 점 v... 을 설정
			Rv1 = cubic_interpolation(pSrc[y1][x1].r, pSrc[y1][x2].r, pSrc[y1][x3].r, pSrc[y1][x4].r, p);
			Rv2 = cubic_interpolation(pSrc[y2][x1].r, pSrc[y2][x2].r, pSrc[y2][x3].r, pSrc[y2][x4].r, p);
			Rv3 = cubic_interpolation(pSrc[y3][x1].r, pSrc[y3][x2].r, pSrc[y3][x3].r, pSrc[y3][x4].r, p);
			Rv4 = cubic_interpolation(pSrc[y4][x1].r, pSrc[y4][x2].r, pSrc[y4][x3].r, pSrc[y4][x4].r, p);

			Gv1 = cubic_interpolation(pSrc[y1][x1].g, pSrc[y1][x2].g, pSrc[y1][x3].g, pSrc[y1][x4].g, p);
			Gv2 = cubic_interpolation(pSrc[y2][x1].g, pSrc[y2][x2].g, pSrc[y2][x3].g, pSrc[y2][x4].g, p);
			Gv3 = cubic_interpolation(pSrc[y3][x1].g, pSrc[y3][x2].g, pSrc[y3][x3].g, pSrc[y3][x4].g, p);
			Gv4 = cubic_interpolation(pSrc[y4][x1].g, pSrc[y4][x2].g, pSrc[y4][x3].g, pSrc[y4][x4].g, p);

			Bv1 = cubic_interpolation(pSrc[y1][x1].b, pSrc[y1][x2].b, pSrc[y1][x3].b, pSrc[y1][x4].b, p);
			Bv2 = cubic_interpolation(pSrc[y2][x1].b, pSrc[y2][x2].b, pSrc[y2][x3].b, pSrc[y2][x4].b, p);
			Bv3 = cubic_interpolation(pSrc[y3][x1].b, pSrc[y3][x2].b, pSrc[y3][x3].b, pSrc[y3][x4].b, p);
			Bv4 = cubic_interpolation(pSrc[y4][x1].b, pSrc[y4][x2].b, pSrc[y4][x3].b, pSrc[y4][x4].b, p);

			Rv = cubic_interpolation(Rv1, Rv2, Rv3, Rv4, q); // 각 행의 보간된 점들을 다시 보간하여 최종적으로 보간된 값을 구한다.
			Gv = cubic_interpolation(Gv1, Gv2, Gv3, Gv4, q); // 각 행의 보간된 점들을 다시 보간하여 최종적으로 보간된 값을 구한다.
			Bv = cubic_interpolation(Bv1, Bv2, Bv3, Bv4, q); // 각 행의 보간된 점들을 다시 보간하여 최종적으로 보간된 값을 구한다.

			pDst[j][i].r = static_cast<BYTE>(limit(Rv + 0.5)); // 반올림하여 점수형을 변환하여 픽셀값으로 지정
			pDst[j][i].g = static_cast<BYTE>(limit(Gv + 0.5)); // 반올림하여 점수형을 변환하여 픽셀값으로 지정
			pDst[j][i].b = static_cast<BYTE>(limit(Bv + 0.5)); // 반올림하여 점수형을 변환하여 픽셀값으로 지정
		}
}

//-------------------------------------------------------------------------
// R, G, B, H, S, I 의 범위는 [0, 1]
//-------------------------------------------------------------------------
void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I)
{
	I = (R + G + B) / 3;

	if ((R == G) && (G == B)) // 그레이스케일인 경우
	{
		S = 0;
		H = 0;
	}
	else
	{
		double min_rgb;
		double temp;

		min_rgb = min(min(R, G), B);
		S = 1 - (min_rgb / I);
		temp = ((R - G) + (R - B)) / (2 * sqrt((R - G)*(R - G) + (R - B) * (G - B)));

		H = acos(temp) * 180 / PI;

		if (B > G)
			H = 360 - H;

		H /= 360;
	}
}

//-------------------------------------------------------------------------
// R, G, B, H, S, I 의 범위는 [0, 1]
//-------------------------------------------------------------------------
void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B)
{
	if (I == 0.0) // 검정색
	{
		R = 0.0;
		G = 0.0;
		B = 0.0;
		return;
	}

	if (S == 0.0) // 그레이스케일
	{
		R = I;
		G = I;
		B = I;
		return;
	}

	H *= 360; // [0, 1] 범위를 [0, 360] 각도 범위로 변경

	if (H <= 120)
	{
		B = I * (1 - S);
		R = I * (1 + S * cos(H * PI / 180) / cos((60 - H) * PI / 180));
		R = limit(R, 0., 1.);
		G = 3 * I - (R + B);
	}
	else if (H <= 240)
	{
		H -= 120;

		R = I * (1 - S);
		G = I * (1 + S * cos(H * PI / 180) / cos((60 - H) * PI / 180));
		G = limit(G, 0., 1.);
		B = 3 * I - (R + G);
	}
	else
	{
		H -= 240;

		G = I * (1 - S);
		B = I * (1 + S * cos(H * PI / 180) / cos((60 - H) * PI / 180));
		B = limit(B, 0., 1.);
		R = 3 * I - (G + B);
	}


}

//-------------------------------------------------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
//-------------------------------------------------------------------------
void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V)
{
	Y = static_cast<BYTE>(limit(0.299*R + 0.587*G + 0.114*B + 0.5));
	U = static_cast<BYTE>(limit(-0.169*R - 0.331*G + 0.500*B + 128 + 0.5));
	V = static_cast<BYTE>(limit(0.500*R - 0.419*G - 0.081*B + 128 + 0.5));
}

//-------------------------------------------------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
//-------------------------------------------------------------------------
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B)
{
	R = static_cast<BYTE>(limit(Y + 1.4075*(V - 128) + 0.5));
	G = static_cast<BYTE>(limit(Y - 0.3455*(U - 128) - 0.7169*(V - 128) + 0.5));
	B = static_cast<BYTE>(limit(Y + 1.7790*(U - 128) + 0.5));
}

// RGB 색상 평면 나누기
void IppColorSplitRGB(IppRgbImage& imgColor, IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgR.CreateImage(w, h);
	imgG.CreateImage(w, h);
	imgB.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pR = imgR.GetPixels();
	BYTE* pG = imgG.GetPixels();
	BYTE* pB = imgB.GetPixels();

	for (int i = 0; i < size; i++)
	{
		pR[i] = pColor[i].r;
		pG[i] = pColor[i].g;
		pB[i] = pColor[i].b;
	}
}

// HSI 색상 평면 나누기
void IppColorSplitHSI(IppRgbImage& imgColor, IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgH.CreateImage(w, h);
	imgS.CreateImage(w, h);
	imgI.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pH = imgH.GetPixels();
	BYTE* pS = imgS.GetPixels();
	BYTE* pI = imgI.GetPixels();

	double r, g, b, hh, ss, ii;
	for (int i = 0; i < size; i++)
	{
		// HSI 성분의 색생 값은 0, 1 설정되어 있기 때문에 255 나누어준다.
		r = pColor[i].r / 255.;
		g = pColor[i].g / 255.;
		b = pColor[i].b / 255.;

		RGB_TO_HSI(r, g, b, hh, ss, ii);

		pH[i] = static_cast<BYTE>(limit(hh * 255 + 0.5));
		pS[i] = static_cast<BYTE>(limit(ss * 255 + 0.5));
		pI[i] = static_cast<BYTE>(limit(ii * 255 + 0.5));
	}
}

// YUV 색상 평면 나누기
void IppColorSplitYUV(IppRgbImage& imgColor, IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV)
{
	int w = imgColor.GetWidth();
	int h = imgColor.GetHeight();
	int size = imgColor.GetSize();

	imgY.CreateImage(w, h);
	imgU.CreateImage(w, h);
	imgV.CreateImage(w, h);

	RGBBYTE* pColor = imgColor.GetPixels();
	BYTE* pY = imgY.GetPixels();
	BYTE* pU = imgU.GetPixels();
	BYTE* pV = imgV.GetPixels();

	for (int i = 0; i < size; i++)
	{
		RGB_TO_YUV(pColor[i].r, pColor[i].g, pColor[i].b, pY[i], pU[i], pV[i]);
	}
} 

// RGB 색상 평면 합치기
bool IppColorCombineRGB(IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB, IppRgbImage& imgColor)
{
	int w = imgR.GetWidth();
	int h = imgR.GetHeight();
	int size = imgR.GetSize();

	// 같은 그림이 아닐경우 FALSE를 반환
	if (imgG.GetWidth() != w || imgG.GetHeight() != h ||
		imgB.GetWidth() != w || imgB.GetHeight() != h)
		return FALSE;

	imgColor.CreateImage(w, h);

	BYTE* pR = imgR.GetPixels();
	BYTE* pG = imgG.GetPixels();
	BYTE* pB = imgB.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	for (int i = 0; i < size; i++)
	{
		pColor[i].r = pR[i];
		pColor[i].g = pG[i];
		pColor[i].b = pB[i];
	}

	return true;
}

// HSI 색상 평면 합치기
bool IppColorCombineHSI(IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI, IppRgbImage& imgColor)
{
	int w = imgH.GetWidth();
	int h = imgH.GetHeight();
	int size = imgH.GetSize();

	if (imgS.GetWidth() != w || imgS.GetHeight() != h ||
		imgI.GetWidth() != w || imgI.GetHeight() != h)
		return false;

	imgColor.CreateImage(w, h);

	BYTE* pH = imgH.GetPixels();
	BYTE* pS = imgS.GetPixels();
	BYTE* pI = imgI.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	double r, g, b, hh, ss, ii;
	for (int i = 0; i < size; i++)
	{
		hh = pH[i] / 255.;
		ss = pS[i] / 255.;
		ii = pI[i] / 255.;

		HSI_TO_RGB(hh, ss, ii, r, g, b); // 트루 컬러 영상으로 출력해야 하기 때문에 변환해준다.

		pColor[i].r = static_cast<BYTE>(limit(r * 255 + 0.5)); 
		pColor[i].g = static_cast<BYTE>(limit(g * 255 + 0.5));
		pColor[i].b = static_cast<BYTE>(limit(b * 255 + 0.5));
	}

	return true;
}

// YUV 색상 평면 합치기
bool IppColorCombineYUV(IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV, IppRgbImage& imgColor)
{
	int w = imgY.GetWidth();
	int h = imgY.GetHeight();
	int size = imgY.GetSize();

	if (imgU.GetWidth() != w || imgU.GetHeight() != h ||
		imgV.GetWidth() != w || imgV.GetHeight() != h)
		return false;

	imgColor.CreateImage(w, h);

	BYTE* pY = imgY.GetPixels();
	BYTE* pU = imgU.GetPixels();
	BYTE* pV = imgV.GetPixels();
	RGBBYTE* pColor = imgColor.GetPixels();

	for (int i = 0; i < size; i++)
	{
		YUV_TO_RGB(pY[i], pU[i], pV[i], pColor[i].r, pColor[i].g, pColor[i].b);
	}

	return true;
}

void IppColorEdge(IppRgbImage& imgSrc, IppByteImage& imgEdge)
{
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(imgSrc, imgY, imgU, imgV);

	IppByteImage edgeY, edgeU, edgeV;
	// 프리윗 엣지 검출
	IppEdgePrewitt(imgY, edgeY);
	IppEdgePrewitt(imgU, edgeU);
	IppEdgePrewitt(imgV, edgeV);

	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();
	int size = imgSrc.GetSize();

	imgEdge.CreateImage(w, h);
	BYTE* pEdge = imgEdge.GetPixels();

	BYTE* pY = edgeY.GetPixels();
	BYTE* pU = edgeU.GetPixels();
	BYTE* pV = edgeV.GetPixels();

	// 유클라디언 거리 구하기, 0.5의 가중치를 부여
	double dist;
	for (int i = 0; i < size; i++)
	{
		dist = (pY[i] * pY[i]) +
			(0.5 * pU[i]) * (0.5 * pU[i]) +
			(0.5 * pV[i]) * (0.5 * pV[i]);
		pEdge[i] = static_cast<BYTE>(limit(sqrt(dist)));
	}
}