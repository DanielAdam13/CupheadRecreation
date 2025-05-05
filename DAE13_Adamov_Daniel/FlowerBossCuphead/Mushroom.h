#pragma once
#include "Enemy.h"
class Texture;

class Mushroom : public Enemy
{
public:
	explicit Mushroom(const Texture* idleTexture, const Texture* boiledTexture, const Texture* attackTexture, const Texture* popTexture, const Texture* deathTexture, 
		const Texture* cloudTexture, const Vector2f& pos, int colNr = 1, int rowNr = 1, float range = 400.f);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(int damage) override;
	virtual int GetHealth() const override;

	virtual bool MarkedForDeath() const override;

protected:
	const Texture* m_CurrentTexture;

	const Texture* m_TextureIdle;
	const Texture* m_TextureAttack;

	int m_CurrentSpriteRowNr;
	int m_CurrentSpriteColNr;
	float m_CurrentFrameWidth;
	float m_CurrentFrameHeight;

	const float m_Range;

	virtual bool PlayerInRange(const Vector2f& playerPos, float range);

private:
	int m_Hp;

	enum class MushroomState
	{
		idle,
		popout,
		attack,
		popin,
		boil,
		death
	};

	MushroomState m_CurrentState;
	MushroomState m_LastMushroomState;
	
	const Texture* m_TextureBoiled;
	const Texture* m_TexturePop;
	const Texture* m_TextureDeath;
	const Texture* m_TextureCloud;

	float m_FacingAngle;

	float m_AccuSec;
	bool m_AllowedAttack;
	float m_AttackCooldownTimer;

	float m_BoilTimer;

	void ResetAnimator(int frameToReset);

	void UpdateFacingDirection(const Vector2f& cuphead);

};
