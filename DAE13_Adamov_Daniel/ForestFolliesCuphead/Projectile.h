#pragma once
class Texture;
#include "Animator.h"
#include <vector>
#include <memory>
class BulletManager;
class Cuphead;
class UIManager;

class Projectile abstract // The projectile abstarct class
{
public:
	explicit Projectile(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, float damage = 1.f);

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) = 0;
	virtual void Animate(float elapsedSec) = 0;

	virtual Circlef GetHitbox() const = 0;
	virtual float Damage() const = 0;

	virtual bool MarkedForDeletion() const = 0; // after death animation

protected:
	Vector2f m_Position;
	Vector2f m_ShootDirection;
	float m_DirectionAngle;

	const float m_Damage;
	Animator m_Animator;

	const Texture* m_Texture;  // reference to unique_ptr
	const float m_Speed;

	virtual Rectf GetBounds() const = 0;
	bool m_DeleteMarker;
};

