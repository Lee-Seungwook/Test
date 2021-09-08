#pragma once

#include "IppImage/IppImage.h"

// ��� �� ���� ����
void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc ���� 3 x 3 ũ���� ����ũ�� �̿��� ��հ� ���͸��� ����, imgDst�� ����

// ���� ��� �� ���͸� ����
void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst); // imgSrc ���� 3 x 3 ũ���� ����ũ�� �̿��� ���� ��հ� ���͸��� ����, imgDst�� ����

// ����þ� ���� ����
void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma); // imgsrc ���� ǥ�� ������ sigma�� ����þ� ���͸��� ����, imgDst�� ����

// ���ö�þ� ����
void IppFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst);

// ����� ����ũ ����
void IppFilterUnsharpMask(IppByteImage& imgSrc, IppByteImage& imgDst);

// ���̺ν�Ʈ ����
void IppFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha);