#pragma once
#include "Animator.h"
class Texture;
#include "Projectile.h"
class Cuphead;
class SoundEffect;

class Explosion final : public Projectile // Used as Projectile derived class because since it works as a projectile
{
public:
	explicit Explosion(const Texture* sprite, const Vector2f& pos, const SoundEffect* SFX);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override; // it has to announce self deletion to BulletManager
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual float Damage() const override;

	virtual bool MarkedForDeletion() const override;

private:
	virtual Rectf GetBounds() const override;

	const float m_FrameWidth;
	const float m_FrameHeight;

	float m_AccuSec;
	const float m_Duration;

	const SoundEffect* m_SFX;

	Animator m_Animator;
	
};

