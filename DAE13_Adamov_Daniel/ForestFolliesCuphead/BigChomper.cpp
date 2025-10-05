#include "pch.h"
#include "BigChomper.h"
#include "Texture.h"
#include "utils.h"
#include "SoundEffect.h"


BigChomper::BigChomper(const std::unique_ptr<Texture>& spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint,
	float speed, const std::unique_ptr<SoundEffect>& biteSFX1, const std::unique_ptr<SoundEffect>& biteSFX2)
	:Spike(spriteTexture, pos, lowestPoint, highestPoint, speed),
	m_SpriteRowNr{ 4 },
	m_SpriteColNr{ 5 },
	m_ShouldBite{ false },
	m_BiteSFX1{ biteSFX1.get() },
	m_BiteSFX2{ biteSFX2.get() }
{
}

void BigChomper::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_SpriteColNr) * this->GetBounds().width,
	0.f + (m_Animator.GetCurrentFrameNr() / m_SpriteColNr) * this->GetBounds().height, this->GetBounds().width, this->GetBounds().height };

	m_Texture->Draw(Vector2f{ this->GetBounds().left, this->GetBounds().bottom }, srcRect);

	//// Hitbox
	//utils::SetColor(Color4f{ 1,0,0,1 });
	//utils::DrawRect(this->GetBounds());
	//utils::FillEllipse(m_Positon, 5.f, 5.f);

	//// End Points Hitbox
	//utils::SetColor(Color4f{ 0, 0, 1, 1 });
	//utils::FillEllipse(m_LowestPoint, 5.f, 5.f);
	//utils::FillEllipse(m_HighestPoint, 5.f, 5.f);
}

void BigChomper::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager)
{
	Spike::Bounce(elapsedSec);
}

void BigChomper::Animate(float elapsedSec)
{
	if (m_Speed > 0.f)
	{
		if (m_Positon.y <= m_HighestPoint.y - 300.f) // fly up
		{
			m_Animator.AnimateBetweenFrames(elapsedSec, 0, 8, 0.07f);
		}
		else // bite
		{
			if (!m_ShouldBite)
			{
				int randNr{ rand() % 2 + 1 };
				switch (randNr)
				{
				case 1:
					m_BiteSFX1->Play(0);
					break;
				case 2:
					m_BiteSFX2->Play(0);
					break;
				}
				m_ShouldBite = true;
			}
			
			m_Animator.AnimateBetweenFrames(elapsedSec, 9, 11, 0.07f);
		}
	}
	else
	{
		if (m_Positon.y >= m_HighestPoint.y - 200.f) // return bite
		{
			m_Animator.AnimateBetweenFrames(elapsedSec, 12, 15, 0.08f);
			m_ShouldBite = false;
		}
		else // fly down
		{
			m_Animator.AnimateBetweenFrames(elapsedSec, 16, 19, 0.09f);
		}
	}
}

Rectf BigChomper::GetBounds() const
{
	//const Texture* chomperTexture{ GetChomperSprite()};
	const float frameWidth{ this->m_Texture->GetWidth() / m_SpriteColNr };
	const float frameHeight{ this->m_Texture->GetHeight() / m_SpriteRowNr };

	return Rectf(m_Positon.x - frameWidth / 2, m_Positon.y - frameHeight / 2, frameWidth, frameHeight);
}

void BigChomper::TakeDamage(float elapsedSec, float damage, UIManager& uiManager)
{
}

bool BigChomper::MarkedForDeath() const
{
	return m_DeathMarker;
}
