#include "..\\stdafx.h"
#include "Rectangle.h"
#include <math.h>

using namespace Figure;
using namespace Gdiplus;
Rectangle::Rectangle()
{
	m_pointOfBegin = { 0, 0 };
	m_pointOfEnd = { 0, 0 };
}


Rectangle::~Rectangle()
{
}

void Rectangle::SetPoint(const unsigned int index, const int x, const int y)
{
	if (index == 0)
	{
		m_pointOfBegin.X = x;
		m_pointOfBegin.Y = y;
	}
	else if (index == 1)
	{
		m_pointOfEnd.X = x;
		m_pointOfEnd.Y = y;
	}
}

void Rectangle::Draw(Graphics* g)
{
	Pen pen(Color(255, 255, 0, 0), 1);

	Rect newRect = Rect(
		min(m_pointOfBegin.X, m_pointOfEnd.X),
		min(m_pointOfBegin.Y, m_pointOfEnd.Y),
		abs(m_pointOfEnd.X - m_pointOfBegin.X),
		abs(m_pointOfEnd.Y - m_pointOfBegin.Y));

	g->DrawRectangle(&pen, newRect);
}

bool Rectangle::IsInBound(const unsigned int x, const unsigned int y)
{
	if (x >= min(m_pointOfBegin.X, m_pointOfEnd.X) && 
		x <= max(m_pointOfBegin.X, m_pointOfEnd.X) && 
		y >= min(m_pointOfBegin.Y, m_pointOfEnd.Y) &&
		y <= max(m_pointOfBegin.Y, m_pointOfEnd.Y))
		return true;
	else
		return false;
}

void Rectangle::Drag(const unsigned int x, const unsigned int y)
{
	m_pointOfBegin.X += x;
	m_pointOfBegin.Y += y;
	m_pointOfEnd.X += x;
	m_pointOfEnd.Y += y;
}

Rect Rectangle::GetSize() const
{
	return Gdiplus::Rect(
		min(m_pointOfBegin.X, m_pointOfEnd.X),
		min(m_pointOfBegin.Y, m_pointOfEnd.Y),
		abs(m_pointOfEnd.X - m_pointOfBegin.X),
		abs(m_pointOfEnd.Y - m_pointOfBegin.Y));
}