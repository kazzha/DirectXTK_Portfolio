#include "pch.h"
#include "Player.h"

using namespace DX;
using namespace DirectX;

Player::Player() : maxSpeed(2.0f)
{
	
}

Player::~Player()
{
}

void Player::Update(double delta, WPARAM wParam)
{
	if (wParam == VK_LEFT)
	{
		Move(m_position.x - 32, m_position.y);
		AnimatedActor::SetAnimation(L"MoveLeft");
		Play();
		AnimatedActor::Update(delta);
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
