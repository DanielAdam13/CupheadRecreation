#pragma once
#include "Spike.h"

class BigChomper final : public Spike
{
public:
	explicit BigChomper(const Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(float damage, UIManager& uiManager) override;
	virtual int GetHealth() const override;

	virtual bool MarkedForDeath() const override;

private:
	const int m_SpriteRowNr;
	const int m_SpriteColNr;
};

