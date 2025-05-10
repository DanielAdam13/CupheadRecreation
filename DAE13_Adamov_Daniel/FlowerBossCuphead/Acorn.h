#pragma once
#include "Enemy.h"
class Texture;
class Cuphead;
class BulletManager;

class Acorn final : public Enemy
{
public:
	explicit Acorn(const Texture* idle, const Texture* drop, const Vector2f& pos, const Vector2f& direction = Vector2f{ -1, 0 });

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead) override;
	virtual void Animate(float elapsedSec) override;

	virtual Rectf GetBounds() const override;

	virtual void TakeDamage(int damage) override;
	virtual int GetHealth() const override;

	virtual bool MarkedForDeath() const override;

private:
	int m_Hp;	

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

};

