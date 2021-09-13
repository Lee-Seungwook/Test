#pragma once

#include "../stdafx.h"
#include "Rectangle.h"

namespace Figure
{
	class CropRectangle : public Rectangle
	{
	public:
		CropRectangle();
		~CropRectangle();

	public:
		virtual bool IsInBound(const unsigned int x, const unsigned int y) override;
		virtual void Drag(const unsigned int x, const unsigned int y) override;
		virtual Gdiplus::Rect GetSize() const override;

		void Draw(Gdiplus::Graphics* g, const bool isDrawBorder);
		void Draw(Gdiplus::Graphics* g, Gdiplus::Image* image);
		void SetCropPoints();

	private:
		Point m_pointOfBeginCrop;
		Point m_pointOfEndCrop;
	};
}