#pragma once
#include "Spike.h"

class BigChomper final : public Spike
{
public:
	explicit BigChomper(Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed);
	// rule of 3...
	// rule of 5...

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

private:
	const int m_SpriteRowNr;
	const int m_SpriteColNr;
};

