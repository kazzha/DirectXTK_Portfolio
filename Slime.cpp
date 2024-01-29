#include "pch.h"

using namespace DX;
using namespace DirectX;


void DX::Slime::Update(double delta)
{
	Move();
}

void Slime::Move()
{
	if (mStep++ > 60)
	{
		mStep = 0;
		SetRotation((1 - rand() % 3) * 45.0f);
	}
}
