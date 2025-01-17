#include "pch.h"

const double PI = 3.14159265358979323846;
const float PI_F = 3.14159265358979323846f;

#ifndef _DLL_FEATURE_
#define _DLL_FEATURE_
#endif // !_DLL_FEATURE_
#include "FilterAPI.h"
#include "FeatureAPI.h"

// 로버츠
void FeatureAPI::APIEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = p1[j][i] - p1[j - 1][i - 1];

			h2 = p1[j][i] - p1[j - 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}

// 프리윗
void FeatureAPI::APIEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = -p1[j - 1][i - 1] - p1[j - 1][i] - p1[j - 1][i + 1]
				+ p1[j + 1][i - 1] + p1[j + 1][i] + p1[j + 1][i + 1];

			h2 = -p1[j - 1][i - 1] - p1[j][i - 1] - p1[j + 1][i - 1]
				+ p1[j - 1][i + 1] + p1[j][i + 1] + p1[j + 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}

// 소벨
void FeatureAPI::APIEdgeSobel(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = -p1[j - 1][i - 1] - 2 * p1[j - 1][i] - p1[j - 1][i + 1]
				+ p1[j + 1][i - 1] + 2 * p1[j + 1][i] + p1[j + 1][i + 1];
			h2 = -p1[j - 1][i - 1] - 2 * p1[j][i - 1] - p1[j + 1][i - 1]
				+ p1[j - 1][i + 1] + 2 * p1[j][i + 1] + p1[j + 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}

#define CHECK_WEAK_EDGE(x, y) \
	if (pEdge[y][x] == WEAK_EDGE) { \
		pEdge[y][x] = STRONG_EDGE; \
		strong_edges.push_back(APIPoint(x, y)); \
	}

// 캐니
// igmSrc는 입력 영상, imgEdge는 결과가 저장될 영상, sigam는 가우시안 필터링에서 사용할 표준 편차 값, th _low * th_high는 히스테리시스 엣지 트래킹에서 사용할 임계값
// 히스테리시스 에지 트래킹은 약한 에지를 검사하여 에지인지 아닌지를 판단하는 방식
void FeatureAPI::APIEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high)
{
	register int i, j;

	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	// 1. 가우시안 필터링

	IppFloatImage imgGaussian(w, h); // 가우시안 영상 생성(가우시안 필터를 적용한 영상을 저장하기 위함)
	FilterAPI Filter;
	Filter.APIFilterGaussian(imgSrc, imgGaussian, sigma); // 가우시안 필터 적용, sigma는 표준 편차

	// 2. 그래디언트 구하기 (크기 & 방향)

	IppFloatImage imgGx(w, h); // gradient of x, 수평 방향의 소벨 마스크 연산을 수행한 겨로가를 저장
	IppFloatImage imgGy(w, h); // gradient of y, 수직 방향의 소벨 마스크 연산을 수행한 겨로가를 저장
	IppFloatImage imgMag(w, h); // magnitude of gradient, 그래디언트 크기를 저장하기 위한 용도로 사용

	float** pGauss = imgGaussian.GetPixels2D();
	float** pGx = imgGx.GetPixels2D();
	float** pGy = imgGy.GetPixels2D();
	float** pMag = imgMag.GetPixels2D();

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			pGx[j][i] = -pGauss[j - 1][i - 1] - 2 * pGauss[j][i - 1] - pGauss[j + 1][i - 1]
				+ pGauss[j - 1][i + 1] + 2 * pGauss[j][i + 1] + pGauss[j + 1][i + 1];

			pGy[j][i] = -pGauss[j - 1][i - 1] - 2 * pGauss[j - 1][i] - pGauss[j - 1][i + 1]
				+ pGauss[j + 1][i - 1] + 2 * pGauss[j + 1][i] + pGauss[j + 1][i + 1];

			pMag[j][i] = sqrt(pGx[j][i] * pGx[j][i] + pGy[j][i] * pGy[j][i]);
		}

	// 3. 비최대 억제
	// 국지적 최대를 구함과 동시에 이중 임계값을 적용하여 strong edge와 weak edge를 구한다.
	// th_low보다 큰 픽셀 들에 대해서만 수행

	imgEdge.CreateImage(w, h);
	BYTE** pEdge = imgEdge.GetPixels2D();

	enum DISTRICT { AREA0 = 0, AREA45, AREA90, AREA135, NOAREA };

	const BYTE STRONG_EDGE = 255;
	const BYTE WEAK_EDGE = 128;

	std::vector<APIPoint> strong_edges;

	float ang;
	int district;
	bool local_max;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			// 그래디언트 크기가 th_low보다 큰 픽셀에 대해서만 국지적 최대 검사
			// 국지적 최대인 픽셀에 대해서만 강한 엣지 또는 약한 엣지로 설정

			if (pMag[j][i] > th_low)
			{
				//그래디언트 방향 계산 (4개 구역)
				if (pGx[j][i] != 0.f)
				{
					ang = atan2(pGy[j][i], pGx[j][i]) * 180 / PI_F;

					if (((ang >= -22.5f) && (ang < 22.5f)) || (ang >= 157.5f) || (ang < -157.5f))
						district = AREA0;

					else if (((ang >= 22.5f) && (ang < 67.5f)) || ((ang >= -157.5f) && (ang < -112.5f)))
						district = AREA45;

					else if (((ang >= 67.5) && (ang < 112.5)) || ((ang >= -112.5) && (ang < -67.5)))
						district = AREA90;

					else
						district = AREA135;
				}
				else
					district = AREA90;

				// 국지적 최대 검사
				local_max = false;
				switch (district)
				{
				case AREA0:
					if ((pMag[j][i] >= pMag[j][i - 1]) && (pMag[j][i] > pMag[j][i + 1]))
						local_max = true;
					break;

				case AREA45:
					if ((pMag[j][i] >= pMag[j - 1][i - 1]) && (pMag[j][i] > pMag[j + 1][i + 1]))
						local_max = true;
					break;

				case AREA90:
					if ((pMag[j][i] >= pMag[j - 1][i]) && (pMag[j][i] > pMag[j + 1][i]))
						local_max = true;
					break;

				case AREA135:
				default:
					if ((pMag[j][i] >= pMag[j - 1][i + 1]) && (pMag[j][i] > pMag[j + 1][i - 1]))
						local_max = true;
					break;
				}

				// 강한 엣지와 약한 엣지 구분
				if (local_max)
				{
					if (pMag[j][i] > th_high)
					{
						pEdge[j][i] = STRONG_EDGE;
						strong_edges.push_back(APIPoint(i, j));
					}
					else
						pEdge[j][i] = WEAK_EDGE;
				}
			}
		}

	// 4. 히스테리시스 엣지 트래킹
	// 강한 엣지로 선택된 픽셀들의 주변 8개 픽셀들을 검사하여, 약한 엣지로 설정되어 있는 픽셀을 강한 엣지로 변경한다.
	// 다시 강한 엣지 목록에 추가하여 엣지 트래킹을 재귀적으로 수행

	while (!strong_edges.empty()) // strong_edges가 비어 있지 않으면
	{
		APIPoint p = strong_edges.back(); // 마지막 원소를 참조
		strong_edges.pop_back(); // 마지막 원소를 제거

		int x = p.x, y = p.y;

		// 강한 엣지 주변의 약한 엣지는 최종 엣지(강한 엣지)로 설정
		CHECK_WEAK_EDGE(x + 1, y)
			CHECK_WEAK_EDGE(x + 1, y + 1)
			CHECK_WEAK_EDGE(x, y + 1)
			CHECK_WEAK_EDGE(x - 1, y + 1)
			CHECK_WEAK_EDGE(x - 1, y)
			CHECK_WEAK_EDGE(x - 1, y - 1)
			CHECK_WEAK_EDGE(x, y - 1)
			CHECK_WEAK_EDGE(x + 1, y - 1)
	}

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			// 끝까지 약한 엣지로 남아있는 픽셀은 모두 엣지가 아닌 것으로 판단
			if (pEdge[j][i] == WEAK_EDGE) pEdge[j][i] = 0;
		}

}

// 허프 변환을 이용하여 직선 검출 (img는 입력 영상, lines는 검출된 직선 성분들의 정보를 저장할 컨테이너, 
// threshold는 직선 성분으로 판단하기 위한 임계값, 축적 배열에 누적된 값이 threshold 값보다 크면 직선으로 간주
void FeatureAPI::APIHoughLine(IppByteImage& img, std::vector<APILineParam>& lines, int threshold)
{
	register int i, j;

	int w = img.GetWidth();
	int h = img.GetHeight();

	BYTE** ptr = img.GetPixels2D();

	int num_rho = static_cast<int>(sqrt((double)w*w + h * h) * 2);
	int num_ang = 360;

	//-------------------------------------------------------------------------
	// 0 ~ PI 각도에 해당하는 sin, cos 함수의 값을 룩업테이블에 저장
	//-------------------------------------------------------------------------

	float* sin_tbl = new float[num_ang];
	float* cos_tbl = new float[num_ang];

	for (i = 0; i < num_ang; i++)
	{
		sin_tbl[i] = sin(i * PI_F / num_ang);
		cos_tbl[i] = cos(i * PI_F / num_ang);
	}

	//-------------------------------------------------------------------------
	// 축적 배열(Accumulate array) 생성
	//-------------------------------------------------------------------------

	IppIntImage imgAcc(num_ang, num_rho);
	int** pAcc = imgAcc.GetPixels2D();

	int m, n;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			if (ptr[j][i] > 128)
			{
				for (n = 0; n < num_ang; n++)
				{
					m = static_cast<int>(floor(i * sin_tbl[n] + j * cos_tbl[n]));
					m += (num_rho / 2);

					pAcc[m][n]++;
				}
			}
		}

	//-------------------------------------------------------------------------
	// 임계값보다 큰 국지적 최대값을 찾아 직선 성분으로 결정
	//-------------------------------------------------------------------------

	lines.clear();
	int value;
	for (m = 0; m < num_rho; m++)
		for (n = 0; n < num_ang; n++)
		{
			value = pAcc[m][n];
			if (value > 1)
			{
				if (value >= pAcc[m - 1][n] && value >= pAcc[m - 1][n + 1] &&
					value >= pAcc[m][n + 1] && value >= pAcc[m + 1][n + 1] &&
					value >= pAcc[m + 1][n] && value >= pAcc[m + 1][n - 1] &&
					value >= pAcc[m][n - 1] && value >= pAcc[m - 1][n - 1])
				{
					lines.push_back(APILineParam(m - (num_rho / 2), n * PI / num_ang, pAcc[m][n]));
				}
			}
		}

	//-------------------------------------------------------------------------
	// 동적 할당한 메모리 해제
	//-------------------------------------------------------------------------

	delete[] sin_tbl;
	delete[] cos_tbl;
}

// 직선 그리기 구현 함수
// 직선의 시작 좌표와 끝 좌표를 계산하여 line 정보와 C의 밝기 값을 이용하여 직선을 그린다.
void FeatureAPI::APIDrawLine(IppByteImage& img, APILineParam line, BYTE c)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	// (rho, ang) 파라미터를 이용하여 직선의 시작 좌표와 끝 좌표를 계산

	int x1, y1, x2, y2;
	if ((line.ang >= 0 && line.ang < PI / 4) || (line.ang >= 3 * PI / 4 && line.ang < PI))
	{
		x1 = 0;
		y1 = static_cast<int>(floor(line.rho / cos(line.ang) + 0.5));
		x2 = w - 1;
		y2 = static_cast<int>(floor((line.rho - x2 * sin(line.ang)) / cos(line.ang) + 0.5));
	}
	else
	{
		y1 = 0;
		x1 = static_cast<int>(floor(line.rho / sin(line.ang) + 0.5));
		y2 = h - 1;
		x2 = static_cast<int>(floor((line.rho - y2 * cos(line.ang)) / sin(line.ang) + 0.5));
	}

	APIDrawLine(img, x1, y1, x2, y2, c);
}

// x좌표가 0인 경우와 w - 1인 경우에 대하여 y좌표의 값을 계산(비트맵 첫번째 좌표에서 마지막 좌표까지 그레이스케일 값 c를 이용하여 직선을 그림
void FeatureAPI::APIDrawLine(IppByteImage& img, int x1, int y1, int x2, int y2, BYTE c)
{
	int w = img.GetWidth();
	int h = img.GetHeight();
	BYTE** ptr = img.GetPixels2D();

	// 브레제남 알고리즘(Bresenham's Algorithm)에 의한 직선 그리기

	int dx, dy, inc_x, inc_y, fraction;

	dx = x2 - x1;
	inc_x = (dx > 0) ? 1 : -1;
	dx = abs(dx) << 1;

	dy = y2 - y1;
	inc_y = (dy > 0) ? 1 : -1;
	dy = abs(dy) << 1;

	if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
		ptr[y1][x1] = c;

	if (dx >= dy)
	{
		fraction = dy - (dx >> 1);

		while (x1 != x2)
		{
			if ((fraction >= 0) && (fraction || (inc_x > 0)))
			{
				fraction -= dx;
				y1 += inc_y;
			}

			fraction += dy;
			x1 += inc_x;

			if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
				ptr[y1][x1] = c;
		}
	}
	else
	{
		fraction = dx - (dy >> 1);

		while (y1 != y2)
		{
			if ((fraction >= 0) && (fraction || (inc_y > 0)))
			{
				fraction -= dy;
				x1 += inc_x;
			}

			fraction += dx;
			y1 += inc_y;

			if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
				ptr[y1][x1] = c;
		}
	}
}

// 해리스 코너 검출 구현
// img는 입력 영상, corners는 검출된 코너 포인트 저장, th는 해리스 코너 응답 함수에서 코너 여부를 결정할 때 사용할 임계값
void FeatureAPI::APIHarrisCorner(IppByteImage& img, std::vector<APIPoint>& corners, double th)
{
	register int i, j, x, y;

	int w = img.GetWidth();
	int h = img.GetHeight();

	BYTE** ptr = img.GetPixels2D();

	//-------------------------------------------------------------------------
	// 1. (fx)*(fx), (fx)*(fy), (fy)*(fy) 계산
	//-------------------------------------------------------------------------

	IppFloatImage imgDx2(w, h);
	IppFloatImage imgDy2(w, h);
	IppFloatImage imgDxy(w, h);

	float** dx2 = imgDx2.GetPixels2D();
	float** dy2 = imgDy2.GetPixels2D();
	float** dxy = imgDxy.GetPixels2D();

	float tx, ty;
	for (j = 1; j < h - 1; j++)
	for (i = 1; i < w - 1; i++)
	{
		tx = (ptr[j - 1][i + 1] + ptr[j][i + 1] + ptr[j + 1][i + 1]
			- ptr[j - 1][i - 1] - ptr[j][i - 1] - ptr[j + 1][i - 1]) / 6.f;
		ty = (ptr[j + 1][i - 1] + ptr[j + 1][i] + ptr[j + 1][i + 1]
			- ptr[j - 1][i - 1] - ptr[j - 1][i] - ptr[j - 1][i + 1]) / 6.f;

		dx2[j][i] = tx * tx;
		dy2[j][i] = ty * ty;
		dxy[j][i] = tx * ty;
	}

	//-------------------------------------------------------------------------
	// 2. 가우시안 필터링
	//-------------------------------------------------------------------------

	IppFloatImage imgGdx2(w, h);
	IppFloatImage imgGdy2(w, h);
	IppFloatImage imgGdxy(w, h);

	float** gdx2 = imgGdx2.GetPixels2D();
	float** gdy2 = imgGdy2.GetPixels2D();
	float** gdxy = imgGdxy.GetPixels2D();

	float g[5][5] = { {1, 4, 6, 4, 1}, {4, 16, 24, 16, 4}, {6, 24, 36, 24, 6}, {4, 16, 24, 16, 4},{1, 4, 6, 4, 1} }; // 가우시안 마스크 

	for (y = 0; y < 5; y++)
	for (x = 0; x < 5; x++)
	{
		g[y][x] /= 256.f;
	}

	float tx2, ty2, txy;
	for (j = 2; j < h - 2; j++)
	for (i = 2; i < w - 2; i++)
	{
		tx2 = ty2 = txy = 0;
		for (y = 0; y < 5; y++)
		for (x = 0; x < 5; x++)
		{
			tx2 += (dx2[j + y - 2][i + x - 2] * g[y][x]);
			ty2 += (dy2[j + y - 2][i + x - 2] * g[y][x]);
			txy += (dxy[j + y - 2][i + x - 2] * g[y][x]);
		}
		
		gdx2[j][i] = tx2;
		gdy2[j][i] = ty2;
		gdxy[j][i] = txy;
	}

	//-------------------------------------------------------------------------
	// 3. 코너 응답 함수 생성
	//-------------------------------------------------------------------------

	IppFloatImage imgCrf(w, h);
	float** crf = imgCrf.GetPixels2D();

	float k = 0.04f;
	for (j = 2; j < h - 2; j++)
	for (i = 2; i < w - 2; i++)
	{
		crf[j][i] = (gdx2[j][i] * gdy2[j][i] - gdxy[j][i] * gdxy[j][i])
			- k * (gdx2[j][i] + gdy2[j][i])*(gdx2[j][i] + gdy2[j][i]);
	}

	//-------------------------------------------------------------------------
	// 4. 임계값보다 큰 국지적 최대값을 찾아 코너 포인트로 결정
	//-------------------------------------------------------------------------

	corners.clear(); // size를 초기화 한다. capacity는 변함 없음
	float cvf_value;

	for (j = 2; j < h - 2; j++)
	for (i = 2; i < w - 2; i++)
	{
		cvf_value = crf[j][i];
		if (cvf_value > th) // 임계값보다 크면
		{
			if (cvf_value > crf[j - 1][i] && cvf_value > crf[j - 1][i + 1] &&
				cvf_value > crf[j][i + 1] && cvf_value > crf[j + 1][i + 1] &&
				cvf_value > crf[j + 1][i] && cvf_value > crf[j + 1][i - 1] &&
				cvf_value > crf[j][i - 1] && cvf_value > crf[j - 1][i - 1]) // 국지적 최댓값을 찾는다
			{
			corners.push_back(APIPoint(i, j)); // 코너 포인트로 기록
			}
		}
	}
}


