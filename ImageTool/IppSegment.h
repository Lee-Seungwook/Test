#pragma once
#include <vector>
#include "IppImage/IppImage.h"
#include "IppFeature.h"

class IppLabelInfo
{
public:
	std::vector<IppPoint> pixels; // °´Ã¼ ÇÈ¼¿ ÁÂÇ¥ ¸ñ·Ï
	int cx, cy; // °´Ã¼ÀÇ Áß½É ÁÂÇ¥
	int minx, miny, maxx, maxy; // °´Ã¼¸¦ °¨½Î´Â »ç°¢Çü ¿µ¿ªÀÇ ÁÂ»ó±Í, ¿ìÇÏ±Í Á¡ÀÇ ÁÂÇ¥

public:
	IppLabelInfo() : cx(0), cy(0), minx(9999), miny(9999), maxx(0), maxy(0)
	{
		pixels.clear();
	}
};

void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold);
int IppBinarizationIterative(IppByteImage& imgSrc);

int IppLabeling(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<IppLabelInfo>& labels);

void IppContourTracing(IppByteImage& imgSrc, int sx, int sy, std::vector<IppPoint>& cp);