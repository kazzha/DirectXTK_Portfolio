#include "pch.h"
#include "Box.h"

void DX::Box::Move(float X, float Y)
{
	m_position.x += X;
	m_position.y += Y;
}
