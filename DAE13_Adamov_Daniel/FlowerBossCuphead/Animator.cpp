#include "pch.h"
#include "Animator.h"

Animator::Animator(int& currentFrameNr, float& accumulatedSec)
	: m_FrameNr{ currentFrameNr },
	m_AccuSec{ accumulatedSec }
{
}

void Animator::PlayAnimation(const float maxFrameSec)
{
	if (m_AccuSec > maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::PlayAnimation(const float maxFrameSec, int frameToReset)
{
	if (m_AccuSec > maxFrameSec)
	{
		m_FrameNr++;
		m_AccuSec -= maxFrameSec;
		ResetAtFrame(frameToReset);
	}
}

void Animator::LoopBetween(int firstFrame, int lastFrame, const float duration, float maxFrameSec)
{
	static float m_RewindTime{ 0.f };
	if (m_FrameNr >= firstFrame && m_FrameNr <= lastFrame && m_RewindTime <= duration)
	{
		if (m_AccuSec > maxFrameSec)
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
		PlayAnimation(maxFrameSec);
		/*if (m_AccuSec > maxFrameSec)
		{
			m_FrameNr++;
			m_AccuSec -= maxFrameSec;
		}*/
	}
}

void Animator::LoopBetween(int firstFrame, int lastFrame, float maxFrameSec)
{
	if (m_FrameNr >= firstFrame && m_FrameNr <= lastFrame)
	{
		PlayAnimation(maxFrameSec);
		if (m_FrameNr > lastFrame)
		{
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

void Animator::ReverseAnimateBetween(int firstFrame, int lastFrame, const float maxFrameSec)
{
	static int changeIdx{ 0 };
	
	if (m_AccuSec > maxFrameSec)
	{
		if (m_FrameNr == firstFrame)
		{
			changeIdx = 1;
		}
		if (m_FrameNr == lastFrame - 1)
		{
			changeIdx = -1;
		}
		m_FrameNr += changeIdx;
		m_AccuSec -= maxFrameSec;
	}
}

void Animator::ResetAtFrame(int frameToReset)
{
	if (m_FrameNr == frameToReset)
	{
		m_FrameNr = 0;
	}
}

void Animator::Stop()
{
	//m_FrameNr = 0;
	m_AccuSec = 0.f;
}
