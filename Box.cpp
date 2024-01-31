#include "pch.h"

DX::Box::Box() : color(""), currentGoal(nullptr), alreadyGoal(nullptr), boxIndex(0), StageInfo(nullptr), remainBoxCount(nullptr)
{
}

DX::Box::~Box()
{
}

bool DX::Box::MoveForward()
{
	if ((*StageInfo)[boxIndex + 8] == nullptr)
	{
		boxIndex += 8;
		(*StageInfo)[boxIndex] = this;
		m_position.y += 64;
		(*StageInfo)[boxIndex - 8] = nullptr;

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex + 8]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex + 8]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex + 8]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex += 8;
		(*StageInfo)[boxIndex] = this;
		m_position.y += 64;
		(*StageInfo)[boxIndex - 8] = nullptr;

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex + 8]) != nullptr)
	{
		return false;
	}
}

bool DX::Box::MoveBackward()
{
	if ((*StageInfo)[boxIndex - 8] == nullptr)
	{
		boxIndex -= 8;
		(*StageInfo)[boxIndex] = this;
		m_position.y -= 64;
		(*StageInfo)[boxIndex + 8] = nullptr;

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex - 8]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex - 8]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex - 8]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex -= 8;
		(*StageInfo)[boxIndex] = this;
		m_position.y -= 64;
		(*StageInfo)[boxIndex + 8] = nullptr;

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex - 8]) != nullptr)
	{
		return false;
	}
}

bool DX::Box::MoveLeft()
{
	if ((*StageInfo)[boxIndex - 1] == nullptr)
	{
		boxIndex -= 1;
		(*StageInfo)[boxIndex] = this;
		m_position.x -= 64;
		(*StageInfo)[boxIndex + 1] = nullptr;

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex - 1]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex - 1]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex - 1]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex -= 1;
		(*StageInfo)[boxIndex] = this;
		m_position.x -= 64;
		(*StageInfo)[boxIndex + 1] = nullptr;

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex - 1]) != nullptr)
	{
		return false;
	}
}

bool DX::Box::MoveRight()
{
	if ((*StageInfo)[boxIndex + 1] == nullptr)
	{
		boxIndex += 1;
		(*StageInfo)[boxIndex] = this;
		m_position.x += 64;
		(*StageInfo)[boxIndex - 1] = nullptr;

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex + 1]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex + 1]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex + 1]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex += 1;
		(*StageInfo)[boxIndex] = this;
		m_position.x += 64;
		(*StageInfo)[boxIndex - 1] = nullptr;
		
		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex + 1]) != nullptr)
	{
		return false;
	}
}
