#pragma once
#include "Enemy.h"
class Texture;
class BulletManager; // not used
class Cuphead;

class Spike : public Enemy
{
public:
	explicit Spike(const Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(int damage) override;
	virtual int GetHealth() const override;

	virtual bool MarkedForDeath() const override;

protected:
	const Vector2f m_LowestPoint;
	const Vector2f m_HighestPoint;

	float m_Speed;
	const Texture* m_Texture;

	void Bounce(float elapsedSec);
private:

	Rectf GetParryHitbox() const;
	const int m_SpriteRowNr;
	const int m_SpriteColNr;

	const float m_FrameWidth;
	const float m_FrameHeight;
};

