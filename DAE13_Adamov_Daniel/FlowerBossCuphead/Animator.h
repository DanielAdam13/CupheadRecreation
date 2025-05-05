#pragma once
class Animator
{
public:
	explicit Animator();

	void PlayAnimation(const float elapsedSec, const float maxFrameSec);
	void PlayAnimation(const float elapsedSec, const float maxFrameSec, const int frameToReset);
	void ReverseAnimate(const float elapsedSec, int lastFrame, float maxFrameSec);
	void AnimateBetweenFrames(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec, const float duration);
	void AnimateBetweenFrames(const float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec);
	void SkipFrame(int frameToSkip);
	void BounceBetween(const float elapsedSec, int firstFrame, int lastFrame, const float maxFrameSec);
	void Reset(int frameToReset = 0);
	void Stop();

	int GetCurrentFrameNr() const;

private:
	int m_FrameNr;
	float m_AccuSec;

	int m_ChangeIdx;
	float m_RewindTime;
};

