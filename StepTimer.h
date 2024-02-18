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
		void SetFixedTimeStep(bool isFixedTimestep) noexcept // ������ ���� �ð� ������ ����/���� ����
		{
			m_isFixedTimeStep = isFixedTimestep;
		}
		void SetTargetElapsedTicks(uint64_t targetElapsed) noexcept // ������ ������ �ϵ������ Ÿ�̸� ƽ ������ ����
		{
			m_targetElapsedTicks = targetElapsed;
		}
		void SetTargetElapsedSeconds(double targetElapsed) noexcept // ������ ������ �ϵ������ Ÿ�̸� �� ������ ���� - ������� ���ǿ� ���� 
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

			if (!QueryPerformanceCounter(&currentTime)) // ���� �ð��� �ҷ����� ���� Ÿ�̸�
			{
				throw std::exception("QueryPerformanceCounter");
			}

			// ����ð��� ���� �ð��� ����. QuadPart�� LARGE_INTEGER ����ü�� �ɹ� ������, ���� Ÿ�̸��� ���� �ð��� ��Ÿ��
			uint64_t timeDelta = static_cast<uint64_t>(
				currentTime.QuadPart - m_qpcLastTime.QuadPart
				);

			m_qpcLastTime = currentTime; // ���� �ð��� ���� �ð����� �ٲ�
			m_qpcSecondCounter += timeDelta; // ��ü ���� ���� �ð��� ��Ÿ �ð� �߰�

			if (timeDelta > m_qpcMaxDelta)  // ���� ��Ÿ�ð��� �ִ� ��Ÿ�ð����� ũ�ٸ� �ִ� ��Ÿ�ð����� ����
			{
				timeDelta = m_qpcMaxDelta;
			}

			timeDelta *= TicksPerSecond;  // ���ӿ����� ���� ƽ ������ �����ϹǷ� �� ������ ��Ÿ�ð��� ƽ������ ��ȯ
			timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart); // ���� Ÿ�̸� ���ļ��� ���� �ð� ������ ����ȭ��Ŵ

			uint32_t lastFrameCount = m_frameCount; // ������ ī��Ʈ ��ȭ�ϱ� �� ���� ī��Ʈ ����

			if (m_isFixedTimeStep)
			{
				if (static_cast<uint64_t>(std::abs(
					static_cast<int64_t>(timeDelta - m_targetElapsedTicks)))
					< TicksPerSecond / 4000) // ��Ÿ �ð��� ��ǥ ������ ������ ���̰� 0.25%���� ������
				{
					timeDelta = m_targetElapsedTicks; // ��Ÿ �ð��� ��ǥ ������ �������� �ٲ� 
				}
				m_leftOverTicks += timeDelta; // ���� �ð��� �̸� ��´�

				while(m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks; // ���� �����ӿ� ����� ƽ ���� ��ǥ ������ �������� ����
					m_totalTicks += m_targetElapsedTicks; // ������� �� ƽ ���� ���� �����ӿ� ����� ƽ �� ����
					m_leftOverTicks = m_targetElapsedTicks; // ���� ���������� �Ѿ�� �� ���� �ð��� ��ǥ ������ �������� ����
					m_frameCount++;

					update();
				}
			}
			else
			{
				m_elapsedTicks = timeDelta; // ���� ������ ���� ��忡���� ��Ÿ �ð��� �״�� ���(1000�� = 1��) 
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart)) // m_qpcFrequency.QuadPart : �ʴ� ƽ ��
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart); // 1�ʺ��� �Ѿ ���� ����ؼ� 1�� ������ ����
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
		uint32_t m_framesPerSecond; // �ʴ� ������ �� = ������ ������ �ӵ�
		uint32_t m_framesThisSecond;
		uint64_t m_qpcSecondCounter; // ������� ������ ������ ���� ƽ ������ ����

		bool m_isFixedTimeStep;
		uint64_t m_targetElapsedTicks;
		
	};
}