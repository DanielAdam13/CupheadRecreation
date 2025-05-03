#pragma once
class Texture;
#include "Animator.h"
#include <vector>

class Projectile abstract
{
public:
	explicit Projectile(const Texture* sprite, const Vector2f& spawnPos, float directionAngle, float speed, int damage = 1);

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices) = 0;
	virtual void Animate(float elapsedSec) = 0;

	virtual Rectf GetBounds() const = 0;

protected:
	Vector2f m_Position;
	const Vector2f m_ShootDirection;
	float m_DirectionAngle;

	const int m_Damage;
	Animator m_ProjAnimator;

	const Texture* m_Texture;
	const float m_Speed;

private:
	
};

