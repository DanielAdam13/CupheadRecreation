#pragma once
class Animator
{
public:
	Animator(int& currentFrameNr, float& accumulatedSeconds);

	void PlayAnimation(const float maxFrameSec);
	void PlayAnimation(const float maxFrameSec, int frameToReset);
	void LoopBetween(int firstFrame, int lastFrame, const float duration, float maxFrameSec);
	void LoopBetween(int firstFrame, int lastFrame, float maxFrameSec);
	void SkipFrame(int frameToSkip);
	void ReverseAnimateBetween(int firstFrame, int lastFrame, const float maxFrameSec);
	void ResetAtFrame(int frameToReset);
	void Stop();

private:
	int& m_FrameNr;
	float& m_AccuSec;
};

