#pragma once
#include "PeaShooter.h"

class Texture;

class SpecialPeaShooter final : public PeaShooter
{
public:
	explicit SpecialPeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage = 1);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void Animate(float elapsedSec) override;
	
	virtual Circlef GetHitbox() const override;
	virtual bool Parryable() const override;
	virtual int GetDamage() const override;

	virtual bool DissapearOnGroundImpact() override;
	virtual bool MarkedForDeletion() const override;

private:
	virtual Rectf GetBounds() const override;
};

