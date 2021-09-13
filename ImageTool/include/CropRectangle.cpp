#include "..\\stdafx.h"
#include "CropRectangle.h"

using namespace Figure;
using namespace Gdiplus;
CropRectangle::CropRectangle()
{
	m_pointOfBeginCrop = { 0, 0 };
	m_pointOfEndCrop = { 0, 0 };
	m_pointOfBegin = { 0, 0 };
	m_pointOfEnd = { 0, 0 };
}

CropRectangle::~CropRectangle()
{
}

void CropRectangle::Draw(Gdiplus::Graphics* g, const bool isDrawBorder)
{
	Pen pen(Color(255, 255, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDashDot);

	Rect newRect = Rect(
		min(m_pointOfBegin.X, m_pointOfEnd.X),
		min(m_pointOfBegin.Y, m_pointOfEnd.Y),
		abs(m_pointOfEnd.X - m_pointOfBegin.X),
		abs(m_pointOfEnd.Y - m_pointOfBegin.Y));

	if (isDrawBorder == true)
		g->DrawRectangle(&pen, newRect);
}

void CropRectangle::Draw(Gdiplus::Graphics* g, Gdiplus::Image* image)
{
	SolidBrush brs(Gdiplus::Color::White);

	Rect newRect = Rect(
		min(m_pointOfBeginCrop.X, m_pointOfEndCrop.X),
		min(m_pointOfBeginCrop.Y, m_pointOfEndCrop.Y),
		abs(m_pointOfEndCrop.X - m_pointOfBeginCrop.X),
		abs(m_pointOfEndCrop.Y - m_pointOfBeginCrop.Y));

	g->FillRectangle(&brs, newRect);

	g->DrawImage(image, 
		Gdiplus::Rect(min(m_pointOfBegin.X, m_pointOfEnd.X),
			min(m_pointOfBegin.Y, m_pointOfEnd.Y),
			abs(m_pointOfEnd.X - m_pointOfBegin.X),
			abs(m_pointOfEnd.Y - m_pointOfBegin.Y)),
		min(m_pointOfBeginCrop.X, m_pointOfEndCrop.X),
		min(m_pointOfBeginCrop.Y, m_pointOfEndCrop.Y),
		abs(m_pointOfEndCrop.X - m_pointOfBeginCrop.X),
		abs(m_pointOfEndCrop.Y - m_pointOfBeginCrop.Y),
		Unit::UnitPixel);
}

bool CropRectangle::IsInBound(const unsigned int x, const unsigned int y)
{
	if (x >= min(m_pointOfBegin.X, m_pointOfEnd.X) &&
		x <= max(m_pointOfBegin.X, m_pointOfEnd.X) &&
		y >= min(m_pointOfBegin.Y, m_pointOfEnd.Y) &&
		y <= max(m_pointOfBegin.Y, m_pointOfEnd.Y))
		return true;
	else
		return false;
}

void CropRectangle::Drag(const unsigned int x, const unsigned int y)
{
	m_pointOfBegin.X += x;
	m_pointOfBegin.Y += y;
	m_pointOfEnd.X += x;
	m_pointOfEnd.Y += y;
}

void CropRectangle::SetCropPoints()
{
	m_pointOfBeginCrop = m_pointOfBegin;
	m_pointOfEndCrop = m_pointOfEnd;
}

Gdiplus::Rect CropRectangle::GetSize() const
{
	return Gdiplus::Rect(
		min(m_pointOfBegin.X, m_pointOfEnd.X),
		min(m_pointOfBegin.Y, m_pointOfEnd.Y),
		abs(m_pointOfEnd.X - m_pointOfBegin.X),
		abs(m_pointOfEnd.Y - m_pointOfBegin.Y));
}