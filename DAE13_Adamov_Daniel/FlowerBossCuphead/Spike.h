#pragma once
#include "Enemy.h"
class Texture;

class Spike : public Enemy
{
public:
	explicit Spike(Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed);
	// rule of 3...
	// rule of 5...

	virtual void Draw() const override;
	virtual void Update(float elapsedSec);

	virtual Rectf GetBounds() const override;

protected:
	const Vector2f m_LowestPoint;
	const Vector2f m_HighestPoint;

	float m_Speed;
	Texture* m_Texture;

	void Bounce(float elapsedSec);
private:
	
	const int m_SpriteRowNr;
	const int m_SpriteColNr;
};

