#include "pch.h"
#include "TulipSeed.h"
#include "utils.h"
#include <iostream>

TulipSeed::TulipSeed(const Texture* sprite, const Vector2f& startPos, float directionAngle, float speed, int damage)
	:Projectile::Projectile(sprite, startPos, directionAngle, speed, damage)
{
	std::cout << "CREATED Tulip Seed with direction: " << directionAngle << std::endl;
}

void TulipSeed::Draw() const
{
}

void TulipSeed::Update(float elapsedSec, const std::vector<Vector2f>& vertices)
{

	DissapearOnGroundImpact(vertices);
}

Circlef TulipSeed::GetHitbox() const
{
	return Circlef();
}

void TulipSeed::DissapearOnGroundImpact(const std::vector<Vector2f>& vertices)
{
	if (this != nullptr)
	{
		if (utils::IsOverlapping(vertices, GetHitbox()))
		{
			delete this;
		}
	}

}
