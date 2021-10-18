#include "stdafx.h"
#include "IppSegment.h"
#include "IppImage/IppEnhance.h"
#include "IppFourier.h"
#include "IppGeometry.h"

SegmentAPI api;

// ���� ����ȭ �Լ�
// �Է� ����, ��� ����, �Ӱ谪(�Ӱ谪 ���� ���ų� ������ 0, ũ�� 255�� �ȼ� �� ����)
void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold)
{
	api.APIBinarization(imgSrc, imgDst, threshold);
}

// �Ӱ谪 ���� �Լ�
int IppBinarizationIterative(IppByteImage& imgSrc)
{
	return api.APIBinarizationIterative(imgSrc);
}

// ������ ���̺� ����� �����ϴ� �Լ�
// �Է� ����, ���̺��� ����� �� ���̺� ���� ����� ������ ����, ����� ��ü���� ���̺� ������ ���� �����̳�
int IppLabeling(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<APILabelInfo>& labels)
{
	return api.APILabeling(imgSrc, imgDst, labels);
}

//�ܰ��� ����
// �Է� ����, sx, sy�� �ܰ��� ������ ������ ��ü�� ���� ��ǥ, �ܰ��� �ȼ� ��ǥ�� ������ �����̳�
void IppContourTracing(IppByteImage& imgSrc, int sx, int sy, std::vector<APIPoint>& cp)
{
	api.APIContourTracing(imgSrc, sx, sy, cp);
}

// �������� ħ�� ����
void IppMorphologyErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyErosion(imgSrc, imgDst);
}

// �������� ��â ����
void IppMorphologyDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyDilation(imgSrc, imgDst);
}

// �������� ���� ����
void IppMorphologyOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyOpening(imgSrc, imgDst);
}

// �������� �ݱ� ����
void IppMorphologyClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyClosing(imgSrc, imgDst);
}

// �׷��� ������ �������� ħ��
void IppMorphologyGrayErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayErosion(imgSrc, imgDst);
}

// �׷��� ������ �������� ��â
void IppMorphologyGrayDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayDilation(imgSrc, imgDst);
}

// �׷��̽����� ���� ����
void IppMorphologyGrayOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayOpening(imgSrc, imgDst);
}

// �׷��̽����� ���� �ݱ�
void IppMorphologyGrayClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	api.APIMorphologyGrayClosing(imgSrc, imgDst);
}

int LabelingDot(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<APILabelInfo>& labels)
{
	return api.APILabelingDot(imgSrc, imgDst, labels);
}

// Ǫ���� ����ڸ� �̿��� ��ü�� ��輱 ���� �Լ�
void IppFourierDescriptor(IppByteImage& img, int sx, int sy, int percent, std::vector<APIPoint>& cp)
{
	//�ܰ��� ��ǥ ���ϱ�
	IppContourTracing(img, sx, sy, cp);

	// Ǫ���� ����� ���ϱ�
	int num = cp.size();
	double* x = new double[num];
	double* y = new double[num];

	for (int i = 0; i < num; i++)
	{
		x[i] = static_cast<double>(cp[i].x);
		y[i] = static_cast<double>(cp[i].y);
	}

	DFT1d(x, y, num, 1); // �̻� Ǫ���� ��ȯ

	// Ǫ���� ������� �Ϻθ� �̿��Ͽ� �ܰ��� ��ǥ �����ϱ�
	int p = num * percent / 100;
	for (int i = p; i < num; i++)
	{
		x[i] = 0.;
		y[i] = 0.;
	}

	DFT1d(x, y, num, -1); // �̻� Ǫ���� ����ȯ

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

	// ���� �Ҵ��� �޸� ����
	delete[] x;
	delete[] y;
}

// �Է� ����� �Һ� ���Ʈ�� ������ double�� �迭 (7���� ���Ҹ� ���´�.), 
void IppInvariantMoments(IppByteImage& img, double m[7]) 
{
	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	// �������� ���Ʈ ���ϱ�

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

	// �߽� ���Ʈ ���ϱ�

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

	// ����ȭ�� �߽� ���Ʈ
	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	// �Һ� ���Ʈ ���ϱ�

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

// �Է� ���� img�� ������ ��Ÿ���� p�� q�� ���ڷ� ���޹ް�, �׿� �ش��ϴ� �������� ���Ʈ�� ����Ͽ� ��ȯ
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

// img�� ����Ű ���Ʈ�� ����� �Է� ����, n�� m�� ���� ������ �ݺ���, zr�� zi�� ���� ����Ű ���Ʈ�� �÷�� ����� ���� ����
bool IppZernikeMoments(IppByteImage& img, int n, int m, double& zr, double& zi)
{
	if (n < 0 || ((n - abs(m)) % 2 != 0) || abs(m) > n)
		return false;

	if (n > 8)
		return false;

	const int ZM_RADIUS = 100;
	const int ZM_SIZE = ZM_RADIUS * 2 + 1;
	const int FACT[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320 };

	// ����Ű ��� ���׽� ���

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

	// ����Ű ���� �Լ� ���

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

	// ����Ű ���Ʈ ���

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

// img�� �Է� ����, �� ��° ���� imgTmpl�� ã�� ����� ���ø� ����, imgMap���� ���ø� ��Ī�� �����Ͽ� ������ ���絵 �� ������ ����
APIPoint IppTemplateMatching(IppByteImage& imgSrc, IppByteImage& imgTmpl, IppIntImage& imgMap)
{
	int sw = imgSrc.GetWidth(); // �Է� ������ ���� ũ��
	int sh = imgSrc.GetHeight(); // �Է� ������ ���� ũ��
	int tw = imgTmpl.GetWidth(); // ���ø� ������ ���� ũ��
	int th = imgTmpl.GetHeight(); // ���ø� ������ ���� ũ��
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