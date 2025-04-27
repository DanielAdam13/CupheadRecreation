#pragma once
#include "Projectile.h"
class Texture;

class SpecialProjectile final : public Projectile
{
public:
	explicit SpecialProjectile(const Vector2f& spawnPos, float directionAngle);
	~SpecialProjectile() override;
	// rule of 3...
	// rule of 5...

	virtual void Draw() const override;
	// we will inherit Animate() as it is 
private:
	static const Texture* GetSpecialAttackSprite();
	const float m_Speed;
};

