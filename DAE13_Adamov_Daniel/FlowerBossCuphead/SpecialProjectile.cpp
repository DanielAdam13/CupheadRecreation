#include "pch.h"
#include "SpecialProjectile.h"

SpecialProjectile::SpecialProjectile(const Vector2f& spawnPos, float directionAngle)
	:Projectile::Projectile(spawnPos, directionAngle, 5),
	m_Speed{ 900.f }
{

}

void SpecialProjectile::Draw() const
{
}
