#include "pch.h"
#include "Tulip.h"
#include "Texture.h"
#include "BulletManager.h"
#include "Cuphead.h"
#include "utils.h"
#include <iostream>

float Tulip::m_CurrentFrameWidth{ 0.f };
float Tulip::m_CurrentFrameHeight{ 0.f };

Tulip::Tulip(const Texture* idleTexture, const Texture* attackTexture, const Vector2f& pos)
	:Enemy::Enemy(pos),
	m_Hp{ 15 },
	m_CurrentTexture{ nullptr },
	m_CurrentState{},
	m_TextureIdle{ idleTexture },
	m_TextureAttack{ attackTexture },
	//m_TextureSeed{new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Seed.png")},
	//m_TextureExplosion{new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Explosion.png")},
	m_CurrentSpriteColNr{},
	m_CurrentSpriteRowNr{}
{
}

Tulip::~Tulip()
{
	/*delete m_TextureSeed;
	delete m_TextureExplosion;*/
}

void Tulip::Draw() const
{
	if (m_CurrentTexture != nullptr)
	{
		m_CurrentFrameWidth = m_CurrentTexture->GetWidth() / m_CurrentSpriteColNr;
		m_CurrentFrameHeight = m_CurrentTexture->GetHeight() / m_CurrentSpriteRowNr;

		Rectf srcRect{ 0.f + (this->m_Animator.GetCurrentFrame() % m_CurrentSpriteColNr) * m_CurrentFrameWidth,
			0.f + (this->m_Animator.GetCurrentFrame() / m_CurrentSpriteColNr) * m_CurrentFrameHeight, m_CurrentFrameWidth, m_CurrentFrameHeight };

		m_CurrentTexture->Draw(Rectf{ this->GetBounds().left, this->GetBounds().bottom, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f }, srcRect);

		// Hitbox
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawRect(GetBounds());
		utils::FillEllipse(m_Positon, 5.f, 5.f);
	}
}

void Tulip::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead)
{
	static float animAccuSec{ 0.f };
	const float attackAnimDuration{ 1.8f };
	static bool attackFinished{ false };

	static float cooldownAcuuSec{ 0.f };
	const float attackCooldown{ 3.f };

	if (m_CurrentState == TulipState::idle)
	{
		if (attackFinished)
		{
			cooldownAcuuSec += elapsedSec;

			if (cooldownAcuuSec > attackCooldown)
			{
				cooldownAcuuSec -= attackCooldown;

				attackFinished = false;
			}
		}
		else if (PlayerInRange(cuphead.GetPosition(), 500.f))
		{
			if (m_Animator.GetCurrentFrame() > 0)
			{
				m_Animator.Reset(0);
			}
			m_CurrentState = TulipState::attack;
			std::cout << "Switched to attack" << std::endl;
		}
	}
	else if (m_CurrentState == TulipState::attack )
	{
		animAccuSec += elapsedSec;

		if (animAccuSec >= attackAnimDuration)
		{
			std::cout << "Attack finished" << std::endl;
			attackFinished = true;

			m_CurrentState = TulipState::idle;
			animAccuSec -= attackAnimDuration;
		}
	}
}

void Tulip::Animate(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case Tulip::TulipState::idle:
		m_CurrentTexture = m_TextureIdle;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 4;

		m_Animator.ReverseAnimateBetween(elapsedSec, 0, 15, 0.06f);
		break;
	case Tulip::TulipState::attack:
		m_CurrentTexture = m_TextureAttack;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 3;

		m_Animator.ReverseAnimateBetween(elapsedSec, 0, 14, 0.075f);
		break;
	}
}

Rectf Tulip::GetBounds() const
{
	return Rectf(m_Positon.x - (m_CurrentFrameWidth * 0.75f) / 2, m_Positon.y, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f);
}

bool Tulip::Parryable() const
{
	return false;
}

Rectf Tulip::GetParryHitbox() const
{
	return Rectf();
}

bool Tulip::PlayerInRange(const Vector2f& playerPos, float range)
{
	return (std::abs(m_Positon.x - playerPos.x)) < range;
}

