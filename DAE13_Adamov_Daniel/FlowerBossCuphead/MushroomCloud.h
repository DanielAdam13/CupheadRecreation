#pragma once
#include "Projectile.h"
class Cuphead;

class MushroomCloud final : public Projectile
{
public:
	explicit MushroomCloud(const Texture* sprite, const Vector2f& startPos, const Vector2f& playerPos, float directionAngle, float speed, int damage = 1, bool parryable = false);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual float Damage() const override;

	virtual bool MarkedForDeletion() const override;

private:
	virtual Rectf GetBounds() const override;
	Rectf GetParryHitbox() const;

	const int m_ColNr;
	const int m_RowNr;
	const float m_FrameWidth;
	const float m_FrameHeight;

	const Vector2f m_TargetPoint;
	const Vector2f m_StartPoint;

	const bool m_Parryable;
};

