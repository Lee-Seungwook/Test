#include "stdafx.h"
#include "IppFeature.h"
#include "IppFilter.h"

const double PI = 3.14159265358979323846;
const float PI_F = 3.14159265358979323846f;



FeatureAPI api;

// 로버츠
void IppEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge)
{
	api.APIEdgeRoberts(img, imgEdge);
}

// 프리윗
void IppEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge)
{
	api.APIEdgePrewitt(img, imgEdge);
}

// 소벨
void IppEdgeSobel(IppByteImage& img, IppByteImage& imgEdge)
{
	api.APIEdgeSobel(img, imgEdge);
}

// 캐니
// igmSrc는 입력 영상, imgEdge는 결과가 저장될 영상, sigam는 가우시안 필터링에서 사용할 표준 편차 값, th _low * th_high는 히스테리시스 엣지 트래킹에서 사용할 임계값
// 히스테리시스 에지 트래킹은 약한 에지를 검사하여 에지인지 아닌지를 판단하는 방식
void IppEdgeCanny(IppByteImage& imgSrc, IppByteImage& imgEdge, float sigma, float th_low, float th_high)
{
	api.APIEdgeCanny(imgSrc, imgEdge, sigma, th_low, th_high);
}

// 허프 변환을 이용하여 직선 검출 (img는 입력 영상, lines는 검출된 직선 성분들의 정보를 저장할 컨테이너, 
// threshold는 직선 성분으로 판단하기 위한 임계값, 축적 배열에 누적된 값이 threshold 값보다 크면 직선으로 간주
void IppHoughLine(IppByteImage& img, std::vector<APILineParam>& lines, int threshold)
{
	api.APIHoughLine(img, lines, threshold);
}

// 직선 그리기 구현 함수
// 직선의 시작 좌표와 끝 좌표를 계산하여 line 정보와 C의 밝기 값을 이용하여 직선을 그린다.
void IppDrawLine(IppByteImage& img, APILineParam line, BYTE c)
{
	api.APIDrawLine(img, line, c);
}

// x좌표가 0인 경우와 w - 1인 경우에 대하여 y좌표의 값을 계산(비트맵 첫번째 좌표에서 마지막 좌표까지 그레이스케일 값 c를 이용하여 직선을 그림
void IppDrawLine(IppByteImage& img, int x1, int y1, int x2, int y2, BYTE c)
{
	api.APIDrawLine(img, x1, y1, x2, y2, c);
}

// 해리스 코너 검출 구현
// img는 입력 영상, corners는 검출된 코너 포인트 저장, th는 해리스 코너 응답 함수에서 코너 여부를 결정할 때 사용할 임계값
void IppHarrisCorner(IppByteImage& img, std::vector<APIPoint>& corners, double th)
{
	api.APIHarrisCorner(img, corners, th);
}
		
