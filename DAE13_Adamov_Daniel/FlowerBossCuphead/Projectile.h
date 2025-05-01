#pragma once
class Texture;
#include "Animator.h"
#include "Matrix2x3.h"

class Projectile // not final since we have the Special attack which will inherit this class
{
public:
	explicit Projectile(Texture* sprite, const Vector2f& spawnPos, float directionAngle, int damage = 1);

	virtual void Draw() const;
	virtual void Update(float elapsedSec);

	Rectf GetBounds() const;

protected:
	Vector2f m_Position;
	const Vector2f m_ShootDirection;
	float m_DirectionAngle;

	const int m_Damage;
	Animator m_ProjAnimator;

	Texture* m_Texture;
	
	void Animate(float elapsedSec);

private:
	const float m_Speed;
};

