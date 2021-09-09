#pragma once

#include "IppImage/IppImage.h"

// 이동 변환
void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy);
void IppTranslate(IppRgbImage& imgSrc, IppRgbImage& imgDst, int sx, int sy);

