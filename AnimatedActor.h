#pragma once

namespace DX
{
	class AnimatedActor : public Actor
	{
	public:
		AnimatedActor();
		virtual ~AnimatedActor();

		void LoadAnimation(const LPCWSTR jsonFile);

		virtual void Update(double dt) override;

		void SetAnimation(const LPCWSTR aniName);
		void Play() { m_paused = false; }

		void Stop() { m_paused = true; m_frameIndex = 0; m_totalElapsed = 0.f; }
		void Reset() { m_frameIndex = 0; m_totalElapsed = 0.f; }
		void Pause() { m_paused = true; }
		bool IsPaused() { return m_paused; }

	private:
		void SetAnimationIndex(int index);

		rapidjson::WDocument* m_animations;
		rapidjson::WValue* m_animation;

		bool m_paused;
		bool m_isLoop;
		float m_totalElapsed;
		float m_timePerFrame;
		int m_frameIndex;
		int m_frameCount;

	};
}

