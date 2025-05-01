#pragma once
#include "Projectile.h"
class Texture;

class SpecialProjectile final : public Projectile
{
public:
	explicit SpecialProjectile(Texture* sprite, const Vector2f& spawnPos, float directionAngle);

	virtual void Draw() const override;
	// we will inherit Animate() as it is 
private:
	static const Texture* GetSpecialAttackSprite();
	const float m_Speed;
};

