#include "pch.h"
#include "Tulip.h"
#include "Texture.h"
#include "BulletManager.h"
#include "Cuphead.h"
#include "utils.h"
#include <iostream>
#include "TulipSeed.h"

Tulip::Tulip(const Texture* idleTexture, const Texture* attackTexture, const Texture* seed, const Texture* explosion, const Vector2f& pos,
	int colNr, int rowNr, float range)
	:Mushroom::Mushroom(idleTexture, idleTexture, attackTexture, nullptr, nullptr, nullptr, pos, colNr, rowNr, range),
	m_Hp{ 15 },
	m_CurrentState{},
	m_TextureSeed{ seed },
	m_TextureExplosion{ explosion },
	m_AnimAccuSec{ 0.f },
	m_AttackFinished{ false },
	m_CooldownAccuSec{ 0.f }
{
	m_CurrentTexture = idleTexture;
	m_CurrentSpriteColNr = colNr;
	m_CurrentSpriteRowNr = rowNr;
	m_CurrentFrameWidth = m_CurrentTexture->GetWidth() / m_CurrentSpriteColNr;
	m_CurrentFrameHeight = m_CurrentTexture->GetHeight() / m_CurrentSpriteRowNr;
}

void Tulip::Draw() const
{
	Mushroom::Draw();
}

void Tulip::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead)
{
	const float attackAnimDuration{ 1.5f };
	const float attackCooldown{ 3.f };

	if (m_Hp <= 0)
	{
		m_DeathMarker = true;
	}


	if (m_CurrentState == TulipState::idle)
	{
		if (m_AttackFinished)
		{
			m_CooldownAccuSec += elapsedSec;

			if (m_CooldownAccuSec > attackCooldown)
			{
				m_CooldownAccuSec -= attackCooldown;

				m_AttackFinished = false;
			}
		}
		else if (PlayerInRange(cuphead.GetPosition(), m_Range))
		{
			if (m_Animator.GetCurrentFrameNr() > 0)
			{
				m_Animator.Reset(0);
			}
			m_CurrentState = TulipState::attack;

			bulletManager.AddProjectile(new TulipSeed(m_TextureSeed, m_TextureExplosion, Vector2f{ m_Positon.x, m_Positon.y + this->GetBounds().height  }, cuphead.GetPosition(), 0.f, 0.f));
		}
	}
	else if (m_CurrentState == TulipState::attack )
	{
		m_AnimAccuSec += elapsedSec;

		if (m_AnimAccuSec >= attackAnimDuration)
		{
			m_AttackFinished = true;

			m_CurrentState = TulipState::idle;
			m_AnimAccuSec -= attackAnimDuration;
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

		m_Animator.BounceBetween(elapsedSec, 0, 15, 0.06f);
		break;
	case Tulip::TulipState::attack:
		m_CurrentTexture = m_TextureAttack;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 3;

		m_Animator.BounceBetween(elapsedSec, 0, 14, 0.075f);
		break;
	}
	m_CurrentFrameWidth = m_CurrentTexture->GetWidth() / m_CurrentSpriteColNr;
	m_CurrentFrameHeight = m_CurrentTexture->GetHeight() / m_CurrentSpriteRowNr;
}

Rectf Tulip::GetBounds() const
{
	return Rectf(m_Positon.x - (m_CurrentFrameWidth * 0.75f) / 2, m_Positon.y, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f);
}

void Tulip::TakeDamage(int damage)
{
	m_Hp -= damage;
}

int Tulip::GetHealth() const
{
	return m_Hp;
}

bool Tulip::MarkedForDeath() const
{
	return m_DeathMarker;
}

