#include "pch.h"
#include "Spike.h"
#include "Texture.h"
#include "utils.h"
#include "Cuphead.h"
#include "UIManager.h"

Spike::Spike(const Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed)
	:Enemy(pos),
	m_LowestPoint{ lowestPoint },
	m_HighestPoint{ highestPoint },
	m_Speed{ speed },
	m_Texture{ spriteTexture },
	m_SpriteRowNr{ 2 },
	m_SpriteColNr{ 5 },
	m_FrameWidth{ spriteTexture->GetWidth() / m_SpriteColNr },
	m_FrameHeight{ spriteTexture->GetHeight() / m_SpriteRowNr }
{
}

void Spike::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_SpriteColNr) * this->GetBounds().width,
	0.f + (m_Animator.GetCurrentFrameNr() / m_SpriteColNr) * this->GetBounds().height, this->GetBounds().width, this->GetBounds().height };

	m_Texture->Draw(Vector2f{ this->GetBounds().left, this->GetBounds().bottom}, srcRect);

	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(this->GetBounds());
	utils::FillEllipse(m_Positon, 5.f, 5.f);

	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawRect(GetParryHitbox());
}

void Spike::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager)
{
	Bounce(elapsedSec);

	if (cuphead.IsParrying())
	{
		if (utils::IsOverlapping(GetParryHitbox(), cuphead.GetBounds()))
		{
			cuphead.Parry(uiManager);
			m_DeathMarker = true;
		}
	}
}

void Spike::Animate(float elapsedSec)
{
	m_Animator.BounceBetween(elapsedSec, 0, 5, 0.09f);
}

Rectf Spike::GetBounds() const
{
	return Rectf(m_Positon.x - m_FrameWidth / 2, m_Positon.y - m_FrameHeight / 2, m_FrameWidth, m_FrameHeight);
}

Rectf Spike::GetParryHitbox() const
{
	return Rectf{ this->GetBounds().left - m_FrameWidth * 0.75f, this->GetBounds().bottom - m_FrameHeight * 0.75f, m_FrameWidth * 2.5f, m_FrameHeight * 2.5f};
}

void Spike::TakeDamage(float damage, UIManager& uiManager)
{
}

int Spike::GetHealth() const
{
	return 1;
}

bool Spike::MarkedForDeath() const
{
	return m_DeathMarker;
}

void Spike::Bounce(float elapsedSec)
{
	const float midPointY{ m_LowestPoint.y + (m_HighestPoint.y - m_LowestPoint.y) / 2 };

	if (m_Positon.y >= m_HighestPoint.y )
	{
		m_Positon.y = m_HighestPoint.y;
		m_Speed = -m_Speed;
	}
	else if (m_Positon.y <= m_LowestPoint.y)
	{
		m_Positon.y = m_LowestPoint.y;
		m_Speed = -m_Speed;
	}

	m_Positon.y += m_Speed * elapsedSec;
}
