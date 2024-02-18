#include "pch.h"
#include "StageNotify.h"

using namespace DX;

StageNotify::StageNotify() : deltaTime(nullptr)
{
}

StageNotify::~StageNotify()
{
}

void StageNotify::Update(double delta)
{
	(*deltaTime) += delta;
	
}
