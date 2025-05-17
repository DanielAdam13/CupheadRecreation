#pragma once
#include "Projectile.h"
class Texture;
#include <vector>
class Cuphead;

class PeaShooter final : public Projectile // Player projectiles, using it for special attack since they're very close
{
public:
	explicit PeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, float damage = 1, int colNr = 4, int rowNr = 2);

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) override;
	virtual void Animate(float elapsedSec) override;

	virtual Circlef GetHitbox() const override;
	virtual float Damage() const override;

	virtual bool MarkedForDeletion() const override;

protected:
	virtual Rectf GetBounds() const override;

private:
	const float m_FrameWidth{};
	const float m_FrameHeight{};
};

