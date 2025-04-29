#include "pch.h"
#include "Spike.h"
#include "Texture.h"
#include "utils.h"

Spike::Spike(Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed)
	:Enemy(pos),
	m_LowestPoint{ lowestPoint },
	m_HighestPoint{ highestPoint },
	m_Speed{ speed },
	m_Texture{ spriteTexture },
	m_SpriteRowNr{ 2 },
	m_SpriteColNr{ 5 }
{
}

void Spike::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrame() % m_SpriteColNr) * this->GetBounds().width,
	0.f + (m_Animator.GetCurrentFrame() / m_SpriteColNr) * this->GetBounds().height, this->GetBounds().width, this->GetBounds().height };

	m_Texture->Draw(Vector2f{ this->GetBounds().left, this->GetBounds().bottom}, srcRect);

	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	//utils::DrawRect(this->GetBounds());
	//utils::FillEllipse(m_Positon, 5.f, 5.f);
}

void Spike::Update(float elapsedSec)
{
	m_Animator.ReverseAnimateBetween(elapsedSec, 0, 5, 0.09f);

	Bounce(elapsedSec);
}

Rectf Spike::GetBounds() const
{
	const float frameWidth{ m_Texture->GetWidth() / m_SpriteColNr };
	const float frameHeight{ m_Texture->GetHeight() / m_SpriteRowNr };

	return Rectf(m_Positon.x - frameWidth / 2, m_Positon.y - frameHeight / 2, frameWidth, frameHeight);
}

void Spike::Bounce(float elapsedSec)
{
	const float midPointY{ m_LowestPoint.y + (m_HighestPoint.y - m_LowestPoint.y) / 2 };

	if (m_Positon.y >= m_HighestPoint.y || m_Positon.y <= m_LowestPoint.y)
	{
		m_Speed = -m_Speed;
	}

	m_Positon.y += m_Speed * elapsedSec;
}
