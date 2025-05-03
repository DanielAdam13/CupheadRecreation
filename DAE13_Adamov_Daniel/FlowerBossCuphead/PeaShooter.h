#pragma once
#include "Projectile.h"
class Texture;
#include <vector>

class PeaShooter : public Projectile // Player projectiles, not final since we have a special attack
{
public:
	explicit PeaShooter(const Texture* sprite, const Vector2f& spawnPos, float directionAngle, float speed, int damage = 1);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

protected:

private:

};

