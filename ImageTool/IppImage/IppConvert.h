#pragma once

#include "IppImage.h"
#include "IppDib.h"

void IppDibToImage(IppDib& dib, IppByteImage& img); // IppDib 클래스를 IppImage 클래스로 변환
void IppDibToImage(IppDib& dib, IppRgbImage& img); // IppDib 객체가 품고 있는 DIB 영상이 그레이스케일 영상이면 IppByte image 사용, 트루컬러이면 IppRgbImage를 사용
void IppImageToDib(IppByteImage& img, IppDib& dib);  // IppImage 클래스를 IppDib 클래스로 변환
void IppImageToDib(IppFloatImage& img, IppDib& dib); // 정밀한 연산을 수행할 필요가 있거나, 실수형 영상으로 생성되는 경우가 있기 때문이다.
void IppImageToDib(IppRgbImage& img, IppDib& dib);
