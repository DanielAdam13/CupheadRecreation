#include "pch.h"
#include "SpecialPeaShooter.h"
#include "Texture.h"
#include <iostream>

SpecialPeaShooter::SpecialPeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage)
	:PeaShooter::PeaShooter(sprite, spawnPos, playerPos, directionAngle, speed, damage)
{
	std::cout << "CREATED Special with direction: " << directionAngle << std::endl;
}

void SpecialPeaShooter::Draw() const
{
}

void SpecialPeaShooter::Update(float elapsedSec)
{
}

void SpecialPeaShooter::Animate(float elapsedSec)
{
}

Rectf SpecialPeaShooter::GetBounds() const
{
	return Rectf();
}

Circlef SpecialPeaShooter::GetHitbox() const
{
	return Circlef();
}

bool SpecialPeaShooter::Parryable() const
{
	return false;
}

int SpecialPeaShooter::GetDamage() const
{
	return m_Damage;
}

bool SpecialPeaShooter::DissapearOnGroundImpact()
{
	return false;
}

bool SpecialPeaShooter::MarkedForDeletion() const
{
	return false;
}
