#pragma once
#include "PeaShooter.h"

class Texture;

class SpecialPeaShooter final : public PeaShooter
{
public:
	explicit SpecialPeaShooter(const Texture* sprite, const Vector2f& spawnPos, float directionAngle, float speed, int damage = 1);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices) override;
	virtual void Animate(float elapsedSec) override;
	
	virtual Rectf GetBounds() const override;
private:

};

