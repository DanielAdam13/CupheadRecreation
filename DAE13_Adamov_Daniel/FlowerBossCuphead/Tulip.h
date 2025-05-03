#pragma once
#include "Enemy.h"
class Texture;
class BulletManager;
class Cuphead;

class Tulip : public Enemy
{
public:
	explicit Tulip(const Texture* idleTexture, const Texture* attackTexture, const Texture* seed, const Vector2f& pos);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;
	virtual bool Parryable() const override;
	virtual Rectf GetParryHitbox() const override;

	virtual void TakeDamage(int damage) override;
	virtual int GetHealth() const override;

protected:
	int m_Hp;
	const Texture* m_CurrentTexture;

	bool PlayerInRange(const Vector2f& playerPos, float range);

private:
	enum class TulipState 
	{
		idle,
		attack
	};

	TulipState m_CurrentState;

	const Texture* m_TextureIdle;
	const Texture* m_TextureAttack;
	
	const Texture* m_TextureSeed;

	int m_CurrentSpriteRowNr;
	int m_CurrentSpriteColNr;

	static float m_CurrentFrameWidth;
	static float m_CurrentFrameHeight;
};

