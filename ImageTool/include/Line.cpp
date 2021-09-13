#include "..\\stdafx.h"
#include "Line.h"

using namespace Figure;
using namespace Gdiplus;

Line::Line()
{
	m_pointOfBegin = { 0, 0 };
	m_pointOfEnd = { 0, 0 };
}

Line::~Line()
{
}

void Line::SetPoint(const unsigned int index, const int x, const int y)
{
	if (index == 0)
	{
		m_pointOfBegin.X = x;
		m_pointOfBegin.Y = y;
		m_pointOfEnd.X = x;
		m_pointOfEnd.Y = y;
	}
	else if (index == 1)
	{
		m_pointOfEnd.X = x;
		m_pointOfEnd.Y = y;
	}
}

void Line::Draw(Gdiplus::Graphics* g)
{
	Pen pen(Color(255, 255, 0, 0), 1);

	g->DrawLine(&pen, m_pointOfBegin.X, m_pointOfBegin.Y, m_pointOfEnd.X, m_pointOfEnd.Y);
}

bool Line::IsInBound(const unsigned int x, const unsigned int y)
{
	if (x >= min(m_pointOfBegin.X, m_pointOfEnd.X) &&
		x <= max(m_pointOfBegin.X, m_pointOfEnd.X) &&
		y >= min(m_pointOfBegin.Y, m_pointOfEnd.Y) &&
		y <= max(m_pointOfBegin.Y, m_pointOfEnd.Y))
		return true;
	else
		return false;
}

void Line::Drag(const unsigned int x, const unsigned int y)
{
	m_pointOfBegin.X += x;
	m_pointOfBegin.Y += y;
	m_pointOfEnd.X += x;
	m_pointOfEnd.Y += y;
}

Rect Line::GetSize() const
{
	return Gdiplus::Rect(
		min(m_pointOfBegin.X, m_pointOfEnd.X),
		min(m_pointOfBegin.Y, m_pointOfEnd.Y),
		abs(m_pointOfEnd.X - m_pointOfBegin.X),
		abs(m_pointOfEnd.Y - m_pointOfBegin.Y));
}