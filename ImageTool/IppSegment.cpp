#include "stdafx.h"
#include "IppSegment.h"
#include "IppImage/IppEnhance.h"
#include "IppFourier.h"
#include "IppGeometry.h"

SegmentAPI api;

// 영상 이진화 함수
// 입력 영상, 결과 영상, 임계값(임계값 보다 같거나 작으면 0, 크면 255로 픽셀 값 설정)
void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold)
{
	api.APIBinarization(imgSrc, imgDst, threshold);
}

// 임계값 결정 함수
int IppBinarizationIterative(IppByteImage& imgSrc)
{
	return api.APIBinarizationIterative(imgSrc);
}

// 고전적 레이블링 기법을 수행하는 함수
// 입력 영상, 레이블링이 수행된 후 레이블 맵이 저장될 정수형 영상, 검출된 객체들의 레이블 정보를 담을 컨테이너
int IppLabeling(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<APILabelInfo>& labels)
{
	return api.APILabeling(imgSrc, imgDst, labels);
}

//외곽선 검출
// 입력 영상, sx, sy는 외곽선 추적을 시작할 객체의 시작 좌표, 외곽선 픽셀 좌표를 저장할 컨테이너
void IppContourTracing(IppByteImage& imgSrc, int sx, int sy, std::vector<APIPoint>& cp)
{
	api.APIContourTracing(imgSrc, sx, sy, cp);
}

// 모폴로지 침식 연산
void IppMorphologyErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyErosion(imgSrc, imgDst);
}

// 모폴로지 팽창 연산
void IppMorphologyDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyDilation(imgSrc, imgDst);
}

// 모폴로지 열기 연산
void IppMorphologyOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyOpening(imgSrc, imgDst);
}

// 모폴로지 닫기 연산
void IppMorphologyClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyClosing(imgSrc, imgDst);
}

// 그레이 스케일 모폴로지 침식
void IppMorphologyGrayErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayErosion(imgSrc, imgDst);
}

// 그레이 스케일 모폴로지 팽창
void IppMorphologyGrayDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayDilation(imgSrc, imgDst);
}

// 그레이스케일 영상 열기
void IppMorphologyGrayOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayOpening(imgSrc, imgDst);
}

// 그레이스케일 영상 닫기
void IppMorphologyGrayClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayClosing(imgSrc, imgDst);
}

int LabelingDot(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<APILabelInfo>& labels)
{
	return api.APILabelingDot(imgSrc, imgDst, labels);
}

// 푸리에 기술자를 이용한 객체의 경계선 복원 함수
void IppFourierDescriptor(IppByteImage& img, int sx, int sy, int percent, std::vector<APIPoint>& cp)
{
	//외곽선 좌표 구하기
	IppContourTracing(img, sx, sy, cp);

	// 푸리에 기술자 구하기
	int num = cp.size();
	double* x = new double[num];
	double* y = new double[num];

	for (int i = 0; i < num; i++)
	{
		x[i] = static_cast<double>(cp[i].x);
		y[i] = static_cast<double>(cp[i].y);
	}

	DFT1d(x, y, num, 1); // 이산 푸리에 변환

	// 푸리에 기술자의 일부를 이용하여 외곽선 좌표 복원하기
	int p = num * percent / 100;
	for (int i = p; i < num; i++)
	{
		x[i] = 0.;
		y[i] = 0.;
	}

	DFT1d(x, y, num, -1); // 이산 푸리에 역변환

	int w = img.GetWidth();
	int h = img.GetHeight();

	cp.clear();
	APIPoint pt;
	for (int i = 0; i < num; i++)
	{
		pt.x = limit(static_cast<int>(x[i] + 0.5), 0, w - 1);
		pt.y = limit(static_cast<int>(y[i] + 0.5), 0, h - 1);
		cp.push_back(pt);
	}

	// 동적 할당한 메모리 해제
	delete[] x;
	delete[] y;
}

// 입력 영상과 불변 모멘트를 저장할 double형 배열 (7개의 원소를 갖는다.), 
void IppInvariantMoments(IppByteImage& img, double m[7]) 
{
	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	// 기하학적 모멘트 구하기

	g00 = IppGeometricMoment(img, 0, 0);
	g10 = IppGeometricMoment(img, 1, 0);
	g01 = IppGeometricMoment(img, 0, 1);
	g20 = IppGeometricMoment(img, 2, 0);
	g02 = IppGeometricMoment(img, 0, 2);
	g11 = IppGeometricMoment(img, 1, 1);
	g30 = IppGeometricMoment(img, 3, 0);
	g03 = IppGeometricMoment(img, 0, 3);
	g21 = IppGeometricMoment(img, 2, 1);
	g12 = IppGeometricMoment(img, 1, 2);

	// 중심 모멘트 구하기

	double cx = g10 / g00;
	double cy = g01 / g00;

	c00 = g00;
	c20 = g20 - cx * g10;
	c02 = g02 - cy * g01;
	c11 = g11 - cx * g01;
	c30 = g30 - 3 * cx * g20 + 2 * cx * cx * g10;
	c03 = g03 - 3 * cy * g02 + 2 * cy * cy * g01;
	c21 = g21 - 2 * cx * g11 - cy * g20 + 2 * cx * cx * g01;
	c12 = g12 - 2 * cy * g11 - cx * g02 + 2 * cy * cy * g10;

	// 정규화된 중심 모멘트
	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	// 불변 모멘트 구하기

	m[0] = n20 + n02;
	m[1] = (n20 - n02)*(n20 - n02) + 4 * n11*n11;
	m[2] = (n30 - 3 * n12)*(n30 - 3 * n12) + (3 * n21 - n03)*(3 * n21 - n03);
	m[3] = (n30 + n12)*(n30 + n12) + (n21 + n03)*(n21 + n03);
	m[4] = (n30 - 3 * n12)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3 * (n21 + n03)*(n21 + n03))
		+ (3 * n21 - n03)*(n21 + n03)*(3 * (n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
	m[5] = (n20 - n02)*((n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03))
		+ 4 * n11*(n30 + n12)*(n21 + n03);
	m[6] = (3 * n21 - n03)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3 * (n21 + n03)*(n21 + n03))
		+ (3 * n12 - n30)*(n21 + n03)*(3 * (n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));

}

// 입력 영상 img와 차수를 나타내는 p와 q를 인자로 전달받고, 그에 해당하는 기하학적 모멘트를 계산하여 반환
double IppGeometricMoment(IppByteImage& img, int p, int q)
{
	int w = img.GetWidth();
	int h = img.GetHeight();
	BYTE** ptr = img.GetPixels2D();

	register int i, j, k;
	double moment = 0, xp, yq;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		xp = yq = 1;
		for (k = 0; k < p; k++) xp *= i;
		for (k = 0; k < q; k++) yq *= j;

		moment += (xp * yq * ptr[j][i]);
	}
	return moment;
}

// img는 저니키 모멘트를 계산할 입력 영상, n과 m은 각각 차수와 반복수, zr과 zi는 계산된 저니키 모멘트의 시룹와 허수부 값이 저장
bool IppZernikeMoments(IppByteImage& img, int n, int m, double& zr, double& zi)
{
	if (n < 0 || ((n - abs(m)) % 2 != 0) || abs(m) > n)
		return false;

	if (n > 8)
		return false;

	const int ZM_RADIUS = 100;
	const int ZM_SIZE = ZM_RADIUS * 2 + 1;
	const int FACT[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320 };

	// 저니키 방사 다항식 계산

	double zm_poly[ZM_RADIUS + 1] = { 0., };

	int sign;
	double rho;

	for (int r = 0; r <= ZM_RADIUS; r++)
	{
		for (int s = 0; s <= ((n - m) / 2); s++)
		{
			sign = (s % 2 == 0) ? 1 : -1;
			rho = static_cast<double>(r) / ZM_RADIUS;

			zm_poly[r] += (sign * FACT[n - s] * pow(rho, (n - 2 * s))) / (FACT[s] * FACT[(n + m) / 2 - s] * FACT[(n - m) / 2 - s]);
		}
	}

	// 저니키 기저 함수 계산

	double zm_basis_real[ZM_SIZE][ZM_SIZE];
	double zm_basis_imag[ZM_SIZE][ZM_SIZE];
	int x, y, dist;
	double theta;

	for (y = 0; y < ZM_SIZE; y++)
	for (x = 0; x < ZM_SIZE; x++)
	{
		dist = static_cast<int> (hypot(y - ZM_RADIUS, x - ZM_RADIUS));

		if (dist <= ZM_RADIUS)
		{
			theta = atan2(y - ZM_RADIUS, x - ZM_RADIUS);
			zm_basis_real[y][x] = zm_poly[dist] * cos(m * theta);
			zm_basis_imag[y][x] = zm_poly[dist] * sin(m*theta);
		}
	}
	zm_basis_real[ZM_RADIUS][ZM_RADIUS] = zm_poly[0];
	zm_basis_imag[ZM_RADIUS][ZM_RADIUS] = 0.0;

	// 저니키 모멘트 계산

	IppByteImage imgDst;
	IppResizeBilinear(img, imgDst, ZM_SIZE, ZM_SIZE);
	BYTE** pDst = imgDst.GetPixels2D();

	zr = zi = 0;
	for (y = 0; y < ZM_SIZE; y++)
	for (x = 0; x < ZM_SIZE; x++)
	{
		dist = static_cast<int>(hypot(y - ZM_RADIUS, x - ZM_RADIUS));

		if (dist <= ZM_RADIUS)
		{
			zr += ((pDst[y][x] / 255.) * zm_basis_real[y][x]);
			zi -= ((pDst[y][x] / 255.) * zm_basis_imag[y][x]); // Complex conjugate
		}
	}

	return true;
}

// img는 입력 영상, 두 번째 인자 imgTmpl은 찾을 대상인 템플릿 영상, imgMap에는 템플릿 매칭을 수행하여 생성된 유사도 맵 영상이 저장
APIPoint IppTemplateMatching(IppByteImage& imgSrc, IppByteImage& imgTmpl, IppIntImage& imgMap)
{
	int sw = imgSrc.GetWidth(); // 입력 영상의 가로 크기
	int sh = imgSrc.GetHeight(); // 입력 영상의 세로 크기
	int tw = imgTmpl.GetWidth(); // 템플릿 영상의 가로 크기
	int th = imgTmpl.GetHeight(); // 템플릿 영상의 세로 크기
	int tw2 = tw / 2;
	int th2 = th / 2;

	imgMap.CreateImage(sw, sh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pTmpl = imgTmpl.GetPixels2D();
	int** pMap = imgMap.GetPixels2D();

	APIPoint dp;

	int i, j, x, y;
	int min_value = 99999;
	int diff, sum_of_diff;
	for (j = th2; j <= sh - th2; j++)
	for (i = tw2; i <= sw - tw2; i++)
	{
		sum_of_diff = 0;
		for (y = 0; y < th; y++)
		for (x = 0; x < tw; x++)
		{
			diff = pSrc[j - th2 + y][i - tw2 + x] - pTmpl[y][x];
			sum_of_diff += (diff * diff);
		}

		pMap[j][i] = sum_of_diff / (tw * th);

		if (pMap[j][i] < min_value)
		{
			min_value = pMap[j][i];
			dp.x = i;
			dp.y = j;
		}
	}

	return dp;
}