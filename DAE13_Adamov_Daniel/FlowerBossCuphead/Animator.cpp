#include "pch.h"
#include "Animator.h"

Animator::Animator(int& currentFrameNr)
	: m_FrameNr{ currentFrameNr }
{
}

void Animator::ResetAtFrame(int frameToReset)
{
	if (m_FrameNr == frameToReset)
	{
		m_FrameNr = 0;
	}
}
