#pragma once

#include <cmath>
#include <cstdint>

namespace DX
{
	class StepTimer
	{
	public:
		StepTimer() noexcept(false) :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw std::exception("QueryPerformanceFrequency");
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw std::exception("queryPerformanceCounter");
			}

			m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10);
		}

		uint64_t GetElapsedTicks() const noexcept
		{
			return m_elapsedTicks;
		} 
		double GetElapsedSeconds() const noexcept
		{
			return TicksToSeconds(m_elapsedTicks);
		}
		uint32_t GetFrameCount() const noexcept
		{
			return m_frameCount;
		}
		uint32_t GetFramesPerSecond() const noexcept
		{
			return m_framesPerSecond;
		}
		void SetFixedTimeStep(bool isFixedTimestep) noexcept // 프레임 간의 시간 간격을 고정/변동 설정
		{
			m_isFixedTimeStep = isFixedTimestep;
		}
		void SetTargetElapsedTicks(uint64_t targetElapsed) noexcept // 프레임 간격을 하드웨어의 타이머 틱 단위로 설정
		{
			m_targetElapsedTicks = targetElapsed;
		}
		void SetTargetElapsedSeconds(double targetElapsed) noexcept // 프레임 간격을 하드웨어의 타이머 초 단위로 설정 - 사용자의 편의에 따라 
		{
			m_targetElapsedTicks = SecondsToTicks(targetElapsed);
		}

		static const uint64_t TicksPerSecond = 10000000;

		static constexpr double TicksToSeconds(uint64_t ticks) noexcept
		{
			return static_cast<double>(ticks) / TicksPerSecond;
		}
		static constexpr uint64_t SecondsToTicks(double seconds) noexcept
		{
			return static_cast<uint64_t>(seconds * TicksPerSecond);
		}

		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw std::exception("QueryPerformanceCounter");
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime)) // 현재 시간을 불러오는 고성능 타이머
			{
				throw std::exception("QueryPerformanceCounter");
			}

			// 현재시간과 이전 시간의 차이. QuadPart는 LARGE_INTEGER 구조체의 맴버 변수로, 고성능 타이머의 현재 시간을 나타냄
			uint64_t timeDelta = static_cast<uint64_t>(
				currentTime.QuadPart - m_qpcLastTime.QuadPart
				);

			m_qpcLastTime = currentTime; // 이전 시간을 현재 시간으로 바꿈
			m_qpcSecondCounter += timeDelta; // 전체 게임 진행 시간에 델타 시간 추가

			if (timeDelta > m_qpcMaxDelta)  // 만약 델타시간이 최대 델타시간보다 크다면 최대 델타시간으로 보정
			{
				timeDelta = m_qpcMaxDelta;
			}

			timeDelta *= TicksPerSecond;  // 게임엔진은 보통 틱 단위로 관리하므로 초 단위인 델타시간을 틱단위로 변환
			timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart); // 고성능 타이머 주파수로 나눠 시간 간격을 정규화시킴

			uint32_t lastFrameCount = m_frameCount; // 프레임 카운트 변화하기 전 현재 카운트 저장

			if (m_isFixedTimeStep)
			{
				if (static_cast<uint64_t>(std::abs(
					static_cast<int64_t>(timeDelta - m_targetElapsedTicks)))
					< TicksPerSecond / 4000) // 델타 시간과 목표 프레임 간격의 차이가 0.25%보다 작으면
				{
					timeDelta = m_targetElapsedTicks; // 델타 시간을 목표 프레임 간격으로 바꿈 
				}
				m_leftOverTicks += timeDelta; // 남은 시간에 이를 담는다

				while(m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks; // 현재 프레임에 사용할 틱 수를 목표 프레임 간격으로 설정
					m_totalTicks += m_targetElapsedTicks; // 현재까지 총 틱 수에 현재 프레임에 사용한 틱 수 더함
					m_leftOverTicks = m_targetElapsedTicks; // 다음 프레임으로 넘어가야 할 남은 시간을 목표 프레임 간격으로 설정
					m_frameCount++;

					update();
				}
			}
			else
			{
				m_elapsedTicks = timeDelta; // 고정 프레임 간격 모드에서는 델타 시간을 그대로 사용(1000만 = 1초) 
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart)) // m_qpcFrequency.QuadPart : 초당 틱 수
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart); // 1초보다 넘어갈 때를 대비해서 1초 안으로 유지
			}
		}
	private:
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64_t m_qpcMaxDelta;

		uint64_t m_elapsedTicks;
		uint64_t m_totalTicks;
		uint64_t m_leftOverTicks;
		uint32_t m_frameCount; 
		uint32_t m_framesPerSecond; // 초당 프레임 수 = 게임의 프레임 속도
		uint32_t m_framesThisSecond;
		uint64_t m_qpcSecondCounter; // 현재까지 측정된 프레임 수를 틱 단위로 저장

		bool m_isFixedTimeStep;
		uint64_t m_targetElapsedTicks;
		
	};
}