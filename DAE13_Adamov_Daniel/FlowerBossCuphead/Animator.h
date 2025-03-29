#pragma once
class Animator
{
public:
	Animator(int& currentFrameNr);

	void LoopBetween(int firstFrame, int lastFrame, float& duration);
	void SkipFrame(int frameToSkip);
	void Reverse(int firstFrame, int lastFrame);
	void ResetAtFrame(int frameToReset);
private:
	int m_FrameNr;
};

