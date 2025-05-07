#pragma once
#include "Projectile.h"
class Texture;
#include <vector>
class Cuphead;

class PeaShooter : public Projectile // Player projectiles, not final since we have a special attack
{
public:
	explicit PeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage = 1, int colNr = 4, int rowNr = 2);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual int Damage() const override;

	virtual bool MarkedForDeletion() const override;

protected:
	virtual Rectf GetBounds() const override;

private:
	const float m_FrameWidth{};
	const float m_FrameHeight{};
};

