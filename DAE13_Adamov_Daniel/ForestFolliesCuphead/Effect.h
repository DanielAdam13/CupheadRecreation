#pragma once
class Texture;
#include "Animator.h"

class Effect final
{
public:
	explicit Effect(const Vector2f& pos, const Texture* texture, int colNr, int rowNr, float scaleIdx = 1);

	void Update(float elapsedDSec);
	void Draw() const;

	bool MarkedForDeletion() const;

private:
	const Vector2f m_Position;

	const Texture* m_Sprite;
	const int m_ColNr;
	const int m_RowNr;
	const float m_FrameWidth;
	const float m_FrameHeight;
	const float m_Scale;

	const float m_LifetimeDuration;
	float m_LifeTimeAccuSec;

	Animator m_Animator;
	bool m_EndMarker;
};

