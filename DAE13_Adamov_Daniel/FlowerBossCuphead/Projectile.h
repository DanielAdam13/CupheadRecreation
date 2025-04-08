#pragma once
class Texture;
#include "Animator.h"
#include "Matrix2x3.h"

class Projectile // not final since we have the Special attack which will inherit this class
{
public:
	explicit Projectile(const Vector2f& spawnPos, float directionAngle, int damage); // const Vectio2f without & so we get a copy because we don't want the projectile to follow!
	explicit Projectile(const Vector2f& spawnPos, float directinAngle);
	virtual ~Projectile(); // needs a HUGE CHANGE. Maybe introduce a GameState class and check: if(m_GameOver) delete texture...

	virtual void Draw() const;
	virtual void Update(float elapsedSec);
	
	//bool HitEnemy(Enemy* pEnemy); -----> for when we have enemies............bool or void return type function, idk where to handle the damage logic yet
	// -> either in Cuphead or Game...
	// maybe even IF we do it in Cuphead, we can have a Projectile* CollisionAttack() const function
	// same goes for the projectiles of enemies
	// I have to think of a nice logic of where to handle this

protected:
	Vector2f m_Position;
	const Vector2f m_ShootDirection;
	float m_DirectionAngle;

	
	const int m_Damage;
	Animator m_ProjAnimator;

	Rectf GetBounds() const;
	virtual void Animate(float elapsedSec);

private:
	static int m_NumProjectiles;
	//static bool m_GameOver;
	const float m_Speed;
	static Texture* ProjectileSprite(); // no idea if this is the correct way to do it
};

