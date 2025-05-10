#pragma once
#include "Mushroom.h"
class Texture;

class Tulip final : public Mushroom
{
public:
	explicit Tulip(const Texture* idleTexture, const Texture* attackTexture, const Texture* seed, const Texture* explosion, const Vector2f& pos, int colNr = 5, int rowNr = 4, float range = 600.f);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(float damage, UIManager& uiManager) override;
	virtual int GetHealth() const override;
	virtual bool MarkedForDeath() const override;

private:
	float m_Hp;

	enum class TulipState 
	{
		idle,
		attack
	};
	TulipState m_CurrentState;
	
	const Texture* m_TextureSeed;
	const Texture* m_TextureExplosion;

	float m_AnimAccuSec;
	bool m_AttackFinished;
	float m_CooldownAccuSec;
};

