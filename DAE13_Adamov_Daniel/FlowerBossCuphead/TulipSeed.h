#pragma once
#include "Projectile.h"
class Texture;

class TulipSeed final: public Projectile // Tulip's attack projectile
{
public:
	explicit TulipSeed(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual bool Parryable() const override;
	virtual int GetDamage() const override;

	virtual bool DissapearOnGroundImpact() override;
	virtual bool MarkedForDeletion() const override;
	
protected:
	float m_AccuSec;


private:
	virtual Rectf GetBounds() const override;
	Vector2f CalculateBezierPoint(float t) const;

	const Vector2f m_InitialPlayerPoint;
	const Vector2f m_StartPoint;
	const Vector2f m_ControlPoint;

	const float m_Duration;

	float m_DelayAccuSec;
};

