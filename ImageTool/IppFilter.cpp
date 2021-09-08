#include "pch.h"
#include "IppFilter.h"
#include "stdafx.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <algorithm>

const double PI = 3.14159265358979323846;
const float  PI_F = 3.14159265358979323846f;

// ��հ� ����
void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc; // ���� ������ �����ε����� Ŭ������ ���� ����, �̹��� ����

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1},
	};

	int i, j, m, n, sum;
	for ( j = 1; j < h - 1; j++) // �ֿܰ� �ȼ��� ������� �ʴ´�.
	for ( i = 1; i < w - 1; i++) // ���� �ȼ�
	{
		sum = 0;
		for (m = 0; m < 3; m++) // ����ũ �迭
		for (n = 0; n < 3; n++)
		{
			sum += (pSrc[j - 1 + m][i - 1 + n] * mask[m][n]); // ���� �ȼ��� 1���� �����߱� ������ 1�� ���ش�.
		}

		pDst[j][i] = static_cast<BYTE>(limit(sum / 9. + 0.5)); // ����ũ�� �� ������ �����ش�, 0.5�� �ݿø��ε�
	}
}

// ���� ��հ� ����
void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc; // ���� ������ �����ε����� Ŭ������ ���� ����, �̹��� ����

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1},
	};

	int i, j, m, n, sum;
	for (j = 1; j < h - 1; j++) // �ֿܰ� �ȼ��� ������� �ʴ´�.
	for (i = 1; i < w - 1; i++) // ���� �ȼ�
	{
		sum = 0;
		for (m = 0; m < 3; m++) // ����ũ �迭
		for (n = 0; n < 3; n++)
		{
			sum += (pSrc[j - 1 + m][i - 1 + n] * mask[m][n]); // ���� �ȼ��� 1���� �����߱� ������ 1�� ���ش�.
		}

		pDst[j][i] = static_cast<BYTE>(limit(sum / 16. + 0.5)); // ����ũ�� �� ������ �����ش�, 0.5�� �ݿø��ε�
	}
}


// ����þ� ���� ����
void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma)
{
	register int i, j, k, x;

	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	float** pDst = imgDst.GetPixels2D();

	//-------------------------------------------------------------------------
	// 1���� ����þ� ����ũ & �Ǽ� ������ ���� ���� �̹��� ����
	//-------------------------------------------------------------------------

	int dim = static_cast<int>(2 * 4 * sigma + 1.0); // ����ũ ũ�� ����
	if (dim < 3) dim = 3; // �ּ� ����ũ ũ�⸦ 3���� ����
	if (dim % 2 == 0) dim++; // ¦���̸� Ȧ���� �������
	int dim2 = dim / 2;

	IppFloatImage imgMask(dim, 1); // ����þ� ����ũ�μ� Ȱ���ϱ� ���� �Ǽ��� ������ ���
	float* pMask = imgMask.GetPixels();

	for (i = 0; i < dim; i++)
	{
		x = i - dim2;
		pMask[i] = exp(-(x * x) / (2 * sigma * sigma)) / (sqrt(2 * PI_F) * sigma);
	}

	IppFloatImage imgBuf(w, h);
	float** pBuf = imgBuf.GetPixels2D(); // �ӽ� ������ ���� ����
	float sum1, sum2;

	//-------------------------------------------------------------------------
	// ���� ���� ����ũ ����
	//-------------------------------------------------------------------------

	for ( i = 0; i < w; i++)
	for ( j = 0; j < h; j++)
	{
		sum1 = sum2 = 0.f;

		for (k = 0; k < dim; k++)
		{
			x = k - dim2 + j;

			if (x >= 0 && x < h)
			{
				sum1 += pMask[k];
				sum2 += (pMask[k] * pSrc[x][i]);
			}
		}

		pBuf[j][i] = sum2 / sum1;
	}

	//-------------------------------------------------------------------------
	// ���� ���� ����ũ ����
	//-------------------------------------------------------------------------

	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		sum1 = sum2 = 0.f;

		for (k = 0; k < dim; k++)
		{
			x = k - dim2 + i;

			if (x >= 0 && x < w)
			{
				sum1 += pMask[k];
				sum2 += (pMask[k] * pBuf[j][x]);
			}
		}

		pDst[j][i] = sum2 / sum1; // ��� ���� ����
	}
}


// ���ö�þ� ���� ����
void IppFilterLaplacian(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h); // ��� ������ ���� ���������ν� ��� �ȼ� ���� 0���� �ʱ�ȭ

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, sum;
	for (j = 1; j < h - 1; j++) // 4���⿡ ���� �ȼ� ���� ���� �����Ͽ� ��� �ȼ� ���� ���
	for (i = 1; i < w - 1; i++) // �ֿܰ� �ȼ��� ���꿡�� ����
	{
		sum = pSrc[j - 1][i] + pSrc[j][i - 1] + pSrc[j + 1][i] + pSrc[j][i + 1] - 4 * pSrc[j][i];

		pDst[j][i] = static_cast<BYTE>(limit(sum + 128));
	}
}


 // ����� ����ũ ���͸�
void IppFilterUnsharpMask(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, sum;
	for (j = 1; j < h - 1; j++) // 4���⿡ ���� �ȼ� ���� ���� �����Ͽ� ��� �ȼ� ���� ���
	for (i = 1; i < w - 1; i++)
	{
		sum = 5 * pSrc[j][i] - pSrc[j - 1][i] - pSrc[j][i - 1] - pSrc[j + 1][i] - pSrc[j][i + 1];

		pDst[j][i] = static_cast<BYTE>(limit(sum));
	}
}


// ���̺ν�Ʈ ����
void IppFilterHighboost(IppByteImage& imgSrc, IppByteImage& imgDst, float alpha)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	float sum;

	for (j = 1; j < h - 1; j++) // 4���⿡ ���� �ȼ� ���� ���� �����Ͽ� ��� �ȼ� ���� ���
	for (i = 1; i < w - 1; i++)
	{
		sum = (4 + alpha) * pSrc[j][i] - pSrc[j - 1][i] - pSrc[j][i - 1] - pSrc[j + 1][i] - pSrc[j][i + 1]; // alpha ������ ��Ϻ� �����Ѵ�.
		pDst[j][i] = static_cast<BYTE>(limit(sum + 0.5f)); // �ݿø��Ͽ� ���������� ��ȯ�Ѵ�.
	}
}