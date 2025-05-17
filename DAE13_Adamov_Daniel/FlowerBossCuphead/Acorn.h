#pragma once
#include "Enemy.h"
class Texture;
class SoundEffect;

class Acorn final : public Enemy // Enemy derived class that falls down from above
{
public:
	explicit Acorn(const Texture* idle, const Texture* drop, const Vector2f& pos, const Vector2f& direction, SoundEffect* idleSFX, SoundEffect* fallSFX);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(float damage, UIManager& uiManager) override;

	virtual bool MarkedForDeath() const override;

private:
	float m_Hp;	

	enum class AcornState
	{
		idle,
		drop
	};
	AcornState m_CurrentState;

	const Texture* m_TextureIdle;
	const Texture* m_TextureDrop;

	const Texture* m_CurrentTexture;

	const int m_CurrentColNr;
	const int m_CurrentRowNr;
	float m_CurrentFrameWidth;
	float m_CurrentFrameHeight;

	Vector2f m_Direction;
	float m_FacingAngle;

	SoundEffect* m_IdleSFX;
	SoundEffect* m_DropSFX;
};

