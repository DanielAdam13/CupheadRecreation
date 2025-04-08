#pragma once
#include "Projectile.h"
class Texture;

class SpecialProjectile final : public Projectile
{
public:
	explicit SpecialProjectile(const Vector2f& spawnPos, float directionAngle);

	virtual void Draw() const override;
	// we will inherit Animate() as it is 
private:
	static const Texture* m_TextureSpecialAttack;
	const float m_Speed;
};

