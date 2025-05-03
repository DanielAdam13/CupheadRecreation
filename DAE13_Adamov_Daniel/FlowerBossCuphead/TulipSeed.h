#pragma once
#include "Projectile.h"
class Texture;

class TulipSeed : public Projectile // Tulip's attack projectile
{
public:
	explicit TulipSeed(const Texture* sprite, const Vector2f& startPos, float directionAngle, float speed, int damage);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices) override;

	virtual Circlef GetHitbox() const;
	
private:

	void DissapearOnGroundImpact(const std::vector<Vector2f>& vertices);
};

