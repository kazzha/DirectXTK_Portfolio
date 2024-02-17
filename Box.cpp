#include "pch.h"

DX::Box::Box() : color(""), currentGoal(nullptr), alreadyGoal(nullptr), boxIndex(0), StageInfo(nullptr), remainBoxCount(nullptr)
{
}

DX::Box::~Box()
{
}

bool DX::Box::MoveForward()
{
	if ((*StageInfo)[boxIndex + indexRow] == nullptr)
	{
		boxIndex += indexRow;
		(*StageInfo)[boxIndex] = this;
		m_position.y += 64;
		(*StageInfo)[boxIndex - indexRow] = nullptr;
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex + indexRow]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex + indexRow]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex + indexRow]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex += indexRow;
		(*StageInfo)[boxIndex] = this;
		m_position.y += 64;
		(*StageInfo)[boxIndex - indexRow] = nullptr;
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex + indexRow]) != nullptr)
	{
		return false;
	}
	
}

bool DX::Box::MoveBackward()
{
	if ((*StageInfo)[boxIndex - indexRow] == nullptr)
	{
		boxIndex -= indexRow;
		(*StageInfo)[boxIndex] = this;
		m_position.y -= 64;
		(*StageInfo)[boxIndex + indexRow] = nullptr;
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

		return true;
	}
	else if (dynamic_cast<Wall*>((*StageInfo)[boxIndex - indexRow]) != nullptr)
	{
		return false;
	}
	else if (dynamic_cast<Goal*>((*StageInfo)[boxIndex - indexRow]) != nullptr)
	{
		currentGoal = dynamic_cast<Goal*>((*StageInfo)[boxIndex - indexRow]);
		if (currentGoal->color == this->color)
		{
			(*remainBoxCount) -= 1;
		}
		boxIndex -= indexRow;
		(*StageInfo)[boxIndex] = this;
		m_position.y -= 64;
		(*StageInfo)[boxIndex + indexRow] = nullptr;
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex - indexRow]) != nullptr)
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
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

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
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

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
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

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
		if (alreadyGoal != nullptr)
		{
			if (alreadyGoal->color == this->color)
			{
				(*remainBoxCount) += 1;
			}
		}

		return true;
	}
	else if (dynamic_cast<Box*>((*StageInfo)[boxIndex + 1]) != nullptr)
	{
		return false;
	}
}
