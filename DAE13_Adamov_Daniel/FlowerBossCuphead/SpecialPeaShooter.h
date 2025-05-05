#pragma once
#include "PeaShooter.h"
#include <vector>
class Cuphead;
class Texture;

class SpecialPeaShooter final : public PeaShooter
{
public:
	explicit SpecialPeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage = 5);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;
	
	virtual Circlef GetHitbox() const override;
	virtual int Damage() const override;

	virtual bool MarkedForDeletion() const override;

private:
	virtual Rectf GetBounds() const override;
};

