#include "pch.h"

using namespace DX;
using namespace DirectX;

Player::Player() : PlayerIndex(0), currentGoal(nullptr), alreadyGoal(nullptr), StageInfo(nullptr), walkCount(nullptr)
{

}

Player::~Player()
{
}

void Player::Update(double delta, WPARAM wParam)
{
	if (wParam == VK_LEFT)
	{	
		AnimatedActor::SetAnimation(L"MoveLeft");
		Play();
		AnimatedActor::Update(delta);

		if (PlayerIndex -1 < 0) return;

		if (dynamic_cast<Wall*>((*StageInfo)[PlayerIndex - 1]) != nullptr)
		{
			return;
		}
		else if ((*StageInfo)[PlayerIndex - 1] == nullptr)
		{
			(*StageInfo)[PlayerIndex - 1] = this;
			PlayerIndex -= 1;
			(*StageInfo)[PlayerIndex + 1] = nullptr;
			Move(m_position.x - 64, m_position.y);
			(*walkCount)++;
			if (alreadyGoal != nullptr)
			{
				(*StageInfo)[PlayerIndex + 1] = alreadyGoal;
				alreadyGoal = nullptr;
			}
		}
		else if (dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - 1]) != nullptr)
		{
			if (currentGoal == nullptr)
			{
				currentGoal = dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - 1]);
				(*StageInfo)[PlayerIndex - 1] = this;
				PlayerIndex -= 1;
				Move(m_position.x - 64, m_position.y);
				(*StageInfo)[PlayerIndex + 1] = alreadyGoal;
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
			}
		}
		else if (dynamic_cast<Box*>((*StageInfo)[PlayerIndex - 1]) != nullptr)
		{
			Box* boxPointer = dynamic_cast<Box*>((*StageInfo)[PlayerIndex - 1]);
			if (boxPointer->MoveLeft())
			{
				if (boxPointer->alreadyGoal != nullptr)
				{
					currentGoal = boxPointer->alreadyGoal;
					boxPointer->alreadyGoal = nullptr;
					if (boxPointer->currentGoal != nullptr)
					{
						boxPointer->alreadyGoal = boxPointer->currentGoal;
						boxPointer->currentGoal = nullptr;
					}
				}
				PlayerIndex -= 1;
				(*StageInfo)[PlayerIndex] = this;
				(*StageInfo)[PlayerIndex + 1] = nullptr;
				Move(m_position.x - 64, m_position.y);
				if (alreadyGoal != nullptr)
				{
					(*StageInfo)[PlayerIndex + 1] = alreadyGoal;
					alreadyGoal = nullptr;
				}
				else
				{
				}
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
				if (boxPointer->currentGoal != nullptr)
				{
					boxPointer->alreadyGoal = boxPointer->currentGoal;
					boxPointer->currentGoal = nullptr;
				}


			}
		}

		
	}
	else if (wParam == VK_RIGHT)
	{
		AnimatedActor::SetAnimation(L"MoveRight");
		Play();
		AnimatedActor::Update(delta);

		if (PlayerIndex + 1 > 54) return;

		if (dynamic_cast<Wall*>((*StageInfo)[PlayerIndex + 1]) != nullptr)
		{
			return;
		}
		else if ((*StageInfo)[PlayerIndex + 1] == nullptr)
		{
			(*StageInfo)[PlayerIndex + 1] = this;
			PlayerIndex += 1;
			(*StageInfo)[PlayerIndex - 1] = nullptr;
			Move(m_position.x + 64, m_position.y);
			(*walkCount)++;
			if (alreadyGoal != nullptr)
			{
				(*StageInfo)[PlayerIndex - 1] = alreadyGoal;
				alreadyGoal = nullptr;
			}
		}
		else if (dynamic_cast<Goal*>((*StageInfo)[PlayerIndex + 1]) != nullptr)
		{
			if (currentGoal == nullptr)
			{
				currentGoal = dynamic_cast<Goal*>((*StageInfo)[PlayerIndex + 1]);
				(*StageInfo)[PlayerIndex + 1] = this;
				PlayerIndex += 1;
				Move(m_position.x + 64, m_position.y);
				(*StageInfo)[PlayerIndex - 1] = alreadyGoal;
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
			}
		}
		else if (dynamic_cast<Box*>((*StageInfo)[PlayerIndex + 1]) != nullptr)
		{
			Box* boxPointer = dynamic_cast<Box*>((*StageInfo)[PlayerIndex + 1]);
			if (boxPointer->MoveRight())
			{
				if (boxPointer->alreadyGoal != nullptr)
				{
					currentGoal = boxPointer->alreadyGoal;
					boxPointer->alreadyGoal = nullptr;
					if (boxPointer->currentGoal != nullptr)
					{
						boxPointer->alreadyGoal = boxPointer->currentGoal;
						boxPointer->currentGoal = nullptr;
					}
				}
				PlayerIndex += 1;
				(*StageInfo)[PlayerIndex] = this;
				(*StageInfo)[PlayerIndex - 1] = nullptr;
				Move(m_position.x + 64, m_position.y);
				if (alreadyGoal != nullptr)
				{
					(*StageInfo)[PlayerIndex - 1] = alreadyGoal;
					alreadyGoal = nullptr;
				}
				else
				{
				}
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
				if (boxPointer->currentGoal != nullptr)
				{
					boxPointer->alreadyGoal = boxPointer->currentGoal;
					boxPointer->currentGoal = nullptr;
				}
			}
		}

	}
	else if (wParam == VK_UP)
	{
		AnimatedActor::SetAnimation(L"MoveBack");
		Play();
		AnimatedActor::Update(delta);

		if (PlayerIndex - indexRow < 0) return;

		if (dynamic_cast<Wall*>((*StageInfo)[PlayerIndex - indexRow]) != nullptr)
		{
			return;
		}
		else if ((*StageInfo)[PlayerIndex - indexRow] == nullptr)
		{
			(*StageInfo)[PlayerIndex - indexRow] = this;
			PlayerIndex -= indexRow;
			(*StageInfo)[PlayerIndex + indexRow] = nullptr;
			Move(m_position.x, m_position.y - 64);
			(*walkCount)++;
			if (alreadyGoal != nullptr)
			{
				(*StageInfo)[PlayerIndex + 8] = alreadyGoal;
				alreadyGoal = nullptr;
			}
		}
		else if (dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - indexRow]) != nullptr)
		{
			if (currentGoal == nullptr)
			{
				currentGoal = dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - indexRow]);
				(*StageInfo)[PlayerIndex - indexRow] = this;
				PlayerIndex -= indexRow;
				Move(m_position.x, m_position.y - 64);
				(*StageInfo)[PlayerIndex + indexRow] = alreadyGoal;
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
			}
		}
		else if (dynamic_cast<Box*>((*StageInfo)[PlayerIndex - indexRow]) != nullptr)
		{
			Box* boxPointer = dynamic_cast<Box*>((*StageInfo)[PlayerIndex - indexRow]);
			if (boxPointer->MoveBackward())
			{
				if (boxPointer->alreadyGoal != nullptr)
				{
					currentGoal = boxPointer->alreadyGoal;
					boxPointer->alreadyGoal = nullptr;
					if (boxPointer->currentGoal != nullptr)
					{
						boxPointer->alreadyGoal = boxPointer->currentGoal;
						boxPointer->currentGoal = nullptr;
					}
				}
				PlayerIndex -= indexRow;
				(*StageInfo)[PlayerIndex] = this;
				(*StageInfo)[PlayerIndex + indexRow] = nullptr;
				Move(m_position.x, m_position.y - 64);
				if (alreadyGoal != nullptr)
				{
					(*StageInfo)[PlayerIndex + indexRow] = alreadyGoal;
					alreadyGoal = nullptr;
				}
				else
				{
				}
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				(*walkCount)++;
				if (boxPointer->currentGoal != nullptr)
				{
					boxPointer->alreadyGoal = boxPointer->currentGoal;
					boxPointer->currentGoal = nullptr;
				}
			}
		}
	}
	else if (wParam == VK_DOWN)
	{
		AnimatedActor::SetAnimation(L"MoveForward");
		Play();
		AnimatedActor::Update(delta);

		if (PlayerIndex + indexRow > 54) return;

		if (dynamic_cast<Wall*>((*StageInfo)[PlayerIndex + indexRow]) != nullptr)
		{
			return;
		}
		else if ((*StageInfo)[PlayerIndex + indexRow] == nullptr)
		{
			(*StageInfo)[PlayerIndex + indexRow] = (*StageInfo)[PlayerIndex];
			PlayerIndex += indexRow;
			(*StageInfo)[PlayerIndex - indexRow] = nullptr;
			Move(m_position.x, m_position.y + 64);
			(*walkCount)++;
			if (alreadyGoal != nullptr)
			{
				(*StageInfo)[PlayerIndex - indexRow] = alreadyGoal;
				alreadyGoal = nullptr;
			}
		}
		else if (dynamic_cast<Goal*>((*StageInfo)[PlayerIndex + indexRow]) != nullptr)
		{
			if (currentGoal == nullptr)
			{
				currentGoal = dynamic_cast<Goal*>((*StageInfo)[PlayerIndex + indexRow]);
				(*StageInfo)[PlayerIndex + indexRow] = this;
				PlayerIndex += indexRow;
				Move(m_position.x, m_position.y + 64);
				(*walkCount)++;
				(*StageInfo)[PlayerIndex - indexRow] = alreadyGoal;
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
			}
		}
		else if (dynamic_cast<Box*>((*StageInfo)[PlayerIndex + indexRow]) != nullptr)
		{
			Box* boxPointer = dynamic_cast<Box*>((*StageInfo)[PlayerIndex + indexRow]);
			if (boxPointer->MoveForward())
			{
				if (boxPointer->alreadyGoal != nullptr)
				{
					currentGoal = boxPointer->alreadyGoal;
					boxPointer->alreadyGoal = nullptr;
					if (boxPointer->currentGoal != nullptr)
					{
						boxPointer->alreadyGoal = boxPointer->currentGoal;
						boxPointer->currentGoal = nullptr;
					}
				}
				PlayerIndex += indexRow;
				(*StageInfo)[PlayerIndex] = this;
				(*StageInfo)[PlayerIndex - indexRow] = nullptr;
				Move(m_position.x, m_position.y + 64);
				(*walkCount)++;
				if (alreadyGoal != nullptr)
				{
					(*StageInfo)[PlayerIndex - indexRow] = alreadyGoal;
					alreadyGoal = nullptr;
				}
				else
				{
				}
				alreadyGoal = currentGoal;
				currentGoal = nullptr;
				if (boxPointer->currentGoal != nullptr)
				{
					boxPointer->alreadyGoal = boxPointer->currentGoal;
					boxPointer->currentGoal = nullptr;
				}
			}
		}
	}
}

void Player::Move(float X, float Y)
{
		m_position.x = X;
		m_position.y = Y;
	
}
