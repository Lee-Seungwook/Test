#include "stdafx.h"
#include "IppGeometry.h"

// �̵� ��ȯ �Լ� ����
void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		x = i - sx; // �ȼ��� ��ġ�� �����ϱ� ����
		y = j - sy;
		if (x >= 0 && x < w && y >= 0 && y < h)
			pDst[j][i] = pSrc[y][x];
	}
}

void IppTranslate(IppRgbImage& imgSrc, IppRgbImage& imgDst, int sx, int sy)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	RGBBYTE** pSrc = imgSrc.GetPixels2D();
	RGBBYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		x = i - sx; // �ȼ��� ��ġ�� �����ϱ� ����
		y = j - sy;
		if (x >= 0 && x < w && y >= 0 && y < h)
		{
			pDst[j][i].b = pSrc[y][x].b;
			pDst[j][i].g = pSrc[y][x].g;
			pDst[j][i].r = pSrc[y][x].r;
		}
	}
}