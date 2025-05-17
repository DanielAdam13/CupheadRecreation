#pragma once
#include "Projectile.h"
#include <vector>
class Texture;
class Cuphead;
class SoundEffect;

class TulipSeed final: public Projectile // Tulip's attack projectile
{
public:
	explicit TulipSeed(const Texture* sprite, const Texture* explosion, const Vector2f& spawnPos, const Vector2f& playerPos,
		const SoundEffect* explosionSFX, float directionAngle, float speed, float damage = 1);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual float Damage() const override;

	virtual bool MarkedForDeletion() const override;
	
protected:
	float m_AccuSec;

private:
	virtual Rectf GetBounds() const override;
	Vector2f CalculateBezierPoint(float t) const;
	virtual void DissapearOnGroundImpact(const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager);

	const Texture* m_ExplosionTexture;
	const SoundEffect* m_ExplosionSFX;

	const Vector2f m_InitialPlayerPoint;
	const Vector2f m_StartPoint;
	const Vector2f m_ControlPoint;

	const float m_BezierDuration;

	float m_DelayAccuSec;
};

