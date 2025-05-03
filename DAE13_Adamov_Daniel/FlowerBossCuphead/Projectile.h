#pragma once
class Texture;
#include "Animator.h"
#include <vector>

class Projectile abstract
{
public:
	explicit Projectile(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage = 1);

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual void Animate(float elapsedSec) = 0;

	virtual Circlef GetHitbox() const = 0;
	virtual bool Parryable() const = 0;
	virtual int GetDamage() const = 0;

	virtual bool DissapearOnGroundImpact() = 0;
	virtual bool MarkedForDeletion() const = 0; // after death animation

protected:
	Vector2f m_Position;
	Vector2f m_ShootDirection;
	float m_DirectionAngle;

	const int m_Damage;
	Animator m_ProjAnimator;

	const Texture* m_Texture;
	const float m_Speed;

	virtual Rectf GetBounds() const = 0;

private:
	
};

