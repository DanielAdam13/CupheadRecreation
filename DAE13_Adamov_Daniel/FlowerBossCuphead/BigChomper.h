#pragma once
#include "Spike.h"
class SoundEffect;

class BigChomper final : public Spike // Simple enemy derived class that plays as an obsticle
{
public:
	explicit BigChomper(const Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, 
		float speed, const SoundEffect* biteSFX1, const SoundEffect* biteSFX2);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(float damage, UIManager& uiManager) override;

	virtual bool MarkedForDeath() const override;

private:
	const int m_SpriteRowNr;
	const int m_SpriteColNr;

	bool m_ShouldBite;

	const SoundEffect* m_BiteSFX1;
	const SoundEffect* m_BiteSFX2;
};

