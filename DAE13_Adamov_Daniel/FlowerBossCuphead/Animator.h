#pragma once
class Animator
{
public:
	Animator(int& currentFrameNr, float& accumulatedSeconds);

	void PlayAnimation(const float elapsedSec, const float maxFrameSec);
	void PlayAnimation(const float elapsedSec, const float maxFrameSec, const int frameToReset);
	void LoopBetween(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec, const float duration);
	void LoopBetween(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec);
	void SkipFrame(int frameToSkip);
	void ReverseAnimateBetween(const float elapsedSec, int firstFrame, int lastFrame, const float maxFrameSec);
	void Reset(int frameToReset = 0);
	void Stop();

private:
	int& m_FrameNr;
	float m_AccuSec;
};

