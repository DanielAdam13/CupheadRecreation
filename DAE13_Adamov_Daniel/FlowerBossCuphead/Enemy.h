#pragma once
#include "Animator.h"
class BulletManager;
class Cuphead;

class Enemy abstract
{
public:
	Enemy(const Vector2f& pos);
	virtual ~Enemy() = default;
	// no need for rule of 5 since constructos and operators aren't inherited

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead) = 0;
	virtual void Animate(float elapsedSec) = 0;

	virtual Rectf GetBounds() const = 0;

	virtual void TakeDamage(int damage) = 0;
	virtual int GetHealth() const = 0;

	virtual bool MarkedForDeath() const = 0;

protected:
	Animator m_Animator;
	Vector2f m_Positon;

	bool m_DeathMarker;
};

