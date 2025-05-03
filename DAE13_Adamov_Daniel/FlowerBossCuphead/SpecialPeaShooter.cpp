#include "pch.h"
#include "SpecialPeaShooter.h"
#include "Texture.h"
#include <iostream>

SpecialPeaShooter::SpecialPeaShooter(const Texture* sprite, const Vector2f& spawnPos, float directionAngle, float speed, int damage)
	:PeaShooter::PeaShooter(sprite, spawnPos, directionAngle, speed, damage)
{
	std::cout << "CREATED Special with direction: " << directionAngle << std::endl;
}

void SpecialPeaShooter::Draw() const
{
}

void SpecialPeaShooter::Update(float elapsedSec, const std::vector<Vector2f>& vertices)
{
}

void SpecialPeaShooter::Animate(float elapsedSec)
{
}

Rectf SpecialPeaShooter::GetBounds() const
{
	return Rectf();
}
