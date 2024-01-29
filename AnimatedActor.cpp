#include "pch.h"

using namespace DX;
using namespace DirectX;

AnimatedActor::AnimatedActor() : Actor(),
	m_animations{nullptr},
	m_animation{nullptr},
	m_paused{false},
	m_isLoop{false},
	m_totalElapsed{0.0f},
	m_timePerFrame{0.0f},
	m_frameIndex{0},
	m_frameCount{0}
{
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::LoadAnimation(const LPCWSTR jsonFile)
{
	m_animations = JsonManager::Instance().CreateDocument(jsonFile);
}

void AnimatedActor::Update(double dt)
{
	if (m_paused)
	{
		return;
	}

	m_totalElapsed += static_cast<float>(dt);

	int prevFrame = m_frameIndex;

	
	if (m_totalElapsed > m_timePerFrame) // 누적 경과시간이 현재 프레임 시간보다 크면 프레임 정보 갱신
	{
		if (m_isLoop) // 루프모드라면 다시 처음으로 돌아가고, 아니라면 마지막 프레임 유지
		{
			m_frameIndex = (m_frameIndex + 1) % m_frameCount;
		}
		else
		{
			m_frameIndex = std::min(m_frameIndex + 1, m_frameCount - 1);
		}

		if (prevFrame != m_frameIndex) // 프레임이 변경되면 정보 갱신
		{
			m_totalElapsed -= m_timePerFrame; 
			// m_totalElapsed는 애니메이션 경과시간이기에 0.0f로 초기화해도 되지만
			// 정확한 애니메이션 처리를 위해 프레임 유지 시간을 빼기로 함
			SetAnimationIndex(m_frameIndex);
		}
	}

}

void AnimatedActor::SetAnimation(const LPCWSTR aniName)
{
	if (m_animations != nullptr)
	{
		const auto& anims = (*m_animations)[L"animations"].GetArray();
		for (auto& elem : anims)
		{
			if (std::wcscmp(elem[L"name"].GetString(), aniName) == 0)
			{
				m_animation = &elem;
				m_isLoop = elem[L"loop"].GetBool();
				const auto& frames = elem[L"frames"].GetArray();
				m_frameCount = frames.Size();

				Reset();
				SetAnimationIndex(0);
				return;
			}
		}

		Utility::DebugString(L"[AnimatedActor] : Can't find animation %s", aniName);
	}
}

void AnimatedActor::SetAnimationIndex(int index)
{
	if (m_animation != nullptr)
	{
		const auto& frame = (*m_animation)[L"frames"].GetArray()[index];

		SetFrame(frame[L"name"].GetString());
		m_timePerFrame = frame[L"duration"].GetFloat();
	}
}
