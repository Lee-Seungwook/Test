#pragma once

#include "IppImage/IppImage.h"

// �̵� ��ȯ
void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy);
void IppTranslate(IppRgbImage& imgSrc, IppRgbImage& imgDst, int sx, int sy);

