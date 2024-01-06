#include "pch.h"

using namespace DX;

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
	ReleaseAll();
}

void ActorManager::OnDeviceLost()
{
	for (auto& pair : m_Actors)
	{
		for (auto& actor : pair.second)
		{
			actor->OnDeviceLost();
		}
	}
}

void ActorManager::OnDeviceRestored()
{
	for (auto& pair : m_Actors)
	{
		for (auto& actor : pair.second)
		{
			actor->OnDeviceRestored();
		}
	}
}

void ActorManager::Initialize()
{
}

void ActorManager::ReleaseAll()
{
	for (auto& pair : m_Actors)
	{
		for (auto& actor : pair.second)
		{
			actor.reset();
		}
		pair.second.clear();
	}

	m_Actors.clear();
}

void ActorManager::Update(double dt)
{
	for (auto& pair : m_Actors)
	{
		for (auto& actor : pair.second)
		{
			actor->Update(dt);
		}
	}
}

void ActorManager::Draw(DirectX::SpriteBatch* pSpriteBatch)
{
	assert(pSpriteBatch != nullptr);

	for (auto& pair : m_Actors)
	{
		for (auto& actor : pair.second)
		{
			actor->Draw(pSpriteBatch);
		}
	}
}

void ActorManager::Delete(int layer, Actor* pActor) // 특정 레이어를 지움
{
	auto i = m_Actors[layer].begin();
	auto e = m_Actors[layer].end();
	while (i != e)
	{
		if (pActor == i->get())
		{
			i = m_Actors[layer].erase(i);
			break;
		}
		else
		{
			++i;
		}
	}
}
