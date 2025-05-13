#pragma once
class Animator // Used for changing the current frame number in pretty much all game objects
{
public:
	explicit Animator();

	void PlayAnimation(float elapsedSec, float maxFrameSec);
	void PlayAnimation(float elapsedSec, float maxFrameSec, int frameToReset);
	void ReverseAnimate(float elapsedSec, int lastFrame, float maxFrameSec);
	void AnimateBetweenFrames(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec, float duration);
	void AnimateBetweenFrames(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec);
	void SkipFrame(int frameToSkip);
	void BounceBetween(float elapsedSec, int firstFrame, int lastFrame, float maxFrameSec);
	void Reset(int frameToReset = 0);
	void Stop();

	int GetCurrentFrameNr() const;

private:
	int m_FrameNr;
	float m_AccuSec;

	int m_ChangeIdx;
	float m_RewindTime;
};

