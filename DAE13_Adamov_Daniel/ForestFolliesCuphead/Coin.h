#pragma once
#include "Animator.h"
class Texture;

class Coin final
{
public:
	explicit Coin(const Vector2f& pos, const Texture* sprite);

	void Update(float elapsedSec);
	void Draw() const;

	void TriggerTaken();
	bool EffectEnded() const;

	Rectf GetBounds() const;

	bool IsTaken() const;

private:
	const Vector2f m_Position;

	enum class CoinState
	{
		existing,
		taken
	};

	CoinState m_CurrentState;
	Animator m_Animator;

	const Texture* m_CoinSprite;

	float m_AccSec;

	float m_CurrentFrameWidth;
	float m_CurrentFrameHeight;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	bool m_DeathMarker;
};

