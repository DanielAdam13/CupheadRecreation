#include "pch.h"
#include "Animator.h"

Animator::Animator()
	: m_FrameNr{ 0 },
	m_AccuSec{ 0.f },
	m_ChangeIdx{ 0 },
	m_RewindTime{ 0.f }
{
}

void Animator::PlayAnimation(float elapsedSec, float maxFrameSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::PlayAnimation(float elapsedSec, float maxFrameSec, int frameToReset)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
	}

	if (m_FrameNr >= frameToReset)
	{
		m_FrameNr = 0;
	}
}

void Animator::ReverseAnimate(float elapsedSec, int lastFrame, float maxFrameSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= maxFrameSec)
	{
		m_FrameNr--;
		m_AccuSec -= maxFrameSec;
	}

	if (m_FrameNr <= 0)
	{
		m_FrameNr = 0;
	}
}

void Animator::AnimateBetweenFrames(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec, float duration)
{
	m_AccuSec += elapsedSec;

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

void Animator::AnimateBetweenFrames(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec)
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

void Animator::BounceBetween(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec)
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

int Animator::GetCurrentFrameNr() const
{
	return m_FrameNr;
}
