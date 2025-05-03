#include "pch.h"
#include "Animator.h"

Animator::Animator()
	: m_FrameNr{ 0 },
	m_AccuSec{ 0.f },
	m_ChangeIdx{ 0 }
{
}

void Animator::PlayAnimation(const float elapsedSec, const float maxFrameSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec > maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::PlayAnimation(const float elapsedSec, const float maxFrameSec, const int frameToReset)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec > maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::LoopBetween(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec, const float duration)
{
	m_AccuSec += elapsedSec;

	static float m_RewindTime{ 0.f };
	if (m_FrameNr >= firstFrame && m_FrameNr <= lastFrame && m_RewindTime <= duration)
	{
		if (m_AccuSec >= maxFrameSec)
		{
			m_FrameNr++;
			m_RewindTime += maxFrameSec;
			m_AccuSec -= maxFrameSec;
		}
		if (m_FrameNr > lastFrame)
		{
			m_FrameNr = firstFrame;
		}
	}
	else if (m_FrameNr < firstFrame || m_RewindTime > duration)
	{
		if (m_AccuSec >= maxFrameSec)
		{
			m_FrameNr++;
			m_AccuSec -= maxFrameSec;
		}
	}
}

void Animator::LoopBetween(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec > maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;

		if (m_FrameNr > lastFrame)
		{
			m_AccuSec = 0.f;
			m_FrameNr = firstFrame;
		}
	}
}

void Animator::SkipFrame(int frameToSkip)
{
	if (m_FrameNr == frameToSkip)
	{
		m_FrameNr += 2;
	}
}

void Animator::ReverseAnimateBetween(const float elapsedSec, int firstFrame, int lastFrame, const float maxFrameSec)
{
	m_AccuSec += elapsedSec;
	
	if (m_AccuSec >= maxFrameSec)
	{
		if (m_FrameNr == firstFrame)
		{
			m_ChangeIdx = 1;
		}
		if (m_FrameNr == lastFrame)
		{
			m_ChangeIdx = -1;
		}

		m_FrameNr += m_ChangeIdx;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::Reset(int frameToReset)
{
	m_FrameNr = frameToReset;
	m_AccuSec = 0.f; // full reset
}

void Animator::Stop()
{
	//m_FrameNr = 0;
	m_AccuSec = 0.f;
}

int Animator::GetCurrentFrame() const
{
	return m_FrameNr;
}
