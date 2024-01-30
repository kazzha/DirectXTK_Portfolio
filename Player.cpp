#include "pch.h"
#include "Player.h"

using namespace DX;
using namespace DirectX;

Player::Player() : PlayerIndex(0) , hasGoal(nullptr), StageInfo(nullptr)
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

		if (dynamic_cast<Wall*>((*StageInfo)[PlayerIndex - 1]) != nullptr)
		{
			return;
		}
		else if (dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - 1]) != nullptr)
		{
			hasGoal = dynamic_cast<Goal*>((*StageInfo)[PlayerIndex - 1]);
			
			(*StageInfo)[PlayerIndex - 1] = (*StageInfo)[PlayerIndex];
			PlayerIndex -= 1;
			Move(m_position.x - 64, m_position.y);
		}
		
		
	}
	else if (wParam == VK_RIGHT)
	{
		Move(m_position.x + 32, m_position.y);
		AnimatedActor::SetAnimation(L"MoveRight");
		Play();
		AnimatedActor::Update(delta);
	}
	else if (wParam == VK_UP)
	{
		Move(m_position.x, m_position.y - 32 );
		AnimatedActor::SetAnimation(L"MoveBack");
		Play();
		AnimatedActor::Update(delta);
	}
	else if (wParam == VK_DOWN)
	{
		Move(m_position.x, m_position.y + 32 );
		AnimatedActor::SetAnimation(L"MoveForward");
		Play();
		AnimatedActor::Update(delta);
	}
}

void Player::Move(float X, float Y)
{
	if (X >= 64 && Y >= 64)
	{
		m_position.x = X;
		m_position.y = Y;
	}
}
