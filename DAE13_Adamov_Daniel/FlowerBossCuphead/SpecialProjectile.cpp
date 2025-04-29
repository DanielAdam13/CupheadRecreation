#include "pch.h"
#include "SpecialProjectile.h"
#include "Texture.h"

const Texture* SpecialProjectile::GetSpecialAttackSprite()
{
	static const Texture* specialSprite{ new Texture("Projectile_Special_Loop.png") };
	return specialSprite;
}

SpecialProjectile::SpecialProjectile(Texture* sprite, const Vector2f& spawnPos, float directionAngle)
	:Projectile::Projectile(sprite, spawnPos, directionAngle, 5),
	m_Speed{ 900.f }
{
}

void SpecialProjectile::Draw() const
{
}


