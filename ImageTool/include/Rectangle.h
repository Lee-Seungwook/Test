#pragma once

#include "../stdafx.h"
#include "IFigure.h"

namespace Figure
{
	class Rectangle : public IFigure
	{
	public:
		Rectangle();
		~Rectangle();

	public:
		virtual void SetPoint(const unsigned int index, const int x, const int y) override;
		virtual void Draw(Gdiplus::Graphics* g) override;
		virtual bool IsInBound(const unsigned int x, const unsigned int y) override;
		virtual void Drag(const unsigned int x, const unsigned int y) override;
		virtual Gdiplus::Rect GetSize() const override;
	};
}