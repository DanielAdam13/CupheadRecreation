#include "pch.h"
#include "Mushroom.h"
#include "Texture.h"
#include "utils.h"
#include "Cuphead.h"
#include "BulletManager.h"
#include "MushroomCloud.h"

Mushroom::Mushroom(const Texture* idleTexture, const Texture* boiledTexture, const Texture* attackTexture, const Texture* popTexture, const Texture* deathTexture,
	const Texture* cloudTexture, const Vector2f& pos, int colNr, int rowNr, float range)
	:Enemy(pos),
	m_CurrentTexture{ boiledTexture },
	m_CurrentState{ MushroomState::boil },
	m_LastMushroomState{},
	m_Hp{ 10 },
	m_TextureIdle{ idleTexture },
	m_TextureAttack{ attackTexture },
	m_CurrentSpriteColNr{ colNr },
	m_CurrentSpriteRowNr{ rowNr },
	m_CurrentFrameWidth{ m_CurrentTexture->GetWidth() / m_CurrentSpriteColNr },
	m_CurrentFrameHeight{ m_CurrentTexture->GetHeight() / m_CurrentSpriteRowNr },
	m_Range{ range },
	m_TextureBoiled{ boiledTexture },
	m_TexturePop{ popTexture },
	m_TextureDeath{ deathTexture },
	m_TextureCloud{ cloudTexture },
	m_FacingAngle{ 0.f },
	m_AccuSec{ 0.f },
	m_AllowedAttack{ true },
	m_AttackCooldownTimer{ 0.f },
	m_BoilTimer{ 0.f }
{
}

void Mushroom::Draw() const
{
	if (m_CurrentTexture != nullptr)
	{
		Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_CurrentSpriteColNr) * m_CurrentFrameWidth,
			0.f + (m_Animator.GetCurrentFrameNr() / m_CurrentSpriteColNr) * m_CurrentFrameHeight, m_CurrentFrameWidth, m_CurrentFrameHeight };

		glPushMatrix();
			glTranslatef(m_Positon.x, m_Positon.y, 0);
			glRotatef(m_FacingAngle, 0, 1, 0);
			m_CurrentTexture->Draw(Rectf{ -m_CurrentFrameWidth * 0.75f / 2, 0, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f }, srcRect);
		glPopMatrix();

		// Hitbox
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawRect(GetBounds());
		utils::FillEllipse(m_Positon, 5.f, 5.f);
	}
}

void Mushroom::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead)
{
	UpdateFacingDirection(cuphead.GetPosition());

	int frameToResetAnimation{ 0 };

	m_AccuSec += elapsedSec;
	const float m_AttackCooldown{ 2.f };

	if (m_Hp <= 0 && m_LastMushroomState != MushroomState::death)
	{
		m_AccuSec = 0.f;
		m_CurrentState = MushroomState::death;
	}

	switch (m_CurrentState)
	{
	case Mushroom::MushroomState::idle:
		if (PlayerInRange(cuphead.GetPosition(), m_Range) && m_AllowedAttack)
		{
			int randomNr{ rand() % 2 };
			m_AccuSec = 0.f;
			m_CurrentState = MushroomState::attack;
			bulletManager.AddProjectile(new MushroomCloud(m_TextureCloud, m_Positon, cuphead.GetPosition(), 90.f, 350.f, 1, randomNr));
		}
		else if (!PlayerInRange(cuphead.GetPosition(), m_Range))
		{
			m_CurrentState = MushroomState::idle;
			if (m_AccuSec >= 2.f)
			{
				frameToResetAnimation = 9;
				m_CurrentState = MushroomState::popin;
				m_AccuSec -= 2.f;
			}
		}
		else
		{
			frameToResetAnimation = 9;
			m_AccuSec = 0.f;
			m_CurrentState = MushroomState::popin;
		}
		break;
	case Mushroom::MushroomState::popout:
		if (m_AccuSec >= 0.7f)
		{
			m_CurrentState = MushroomState::idle;
			m_AccuSec -= 0.7f;
		}
		break;
	case Mushroom::MushroomState::attack:
		if (m_AccuSec >= 1.05f)
		{
			m_AccuSec -= 1.05f;
			m_AllowedAttack = false;
			m_CurrentState = MushroomState::idle;
		}
		break;
	case Mushroom::MushroomState::popin:
		if (m_AccuSec >= 0.7f)
		{
			m_CurrentState = MushroomState::boil;
			m_AccuSec -= 0.7f;
		}
		break;
	case Mushroom::MushroomState::boil:
		m_BoilTimer += elapsedSec;

		if (!PlayerInRange(cuphead.GetPosition(), m_Range))
		{
			m_BoilTimer = 0.f;
		}
		else if (PlayerInRange(cuphead.GetPosition(), m_Range) && m_AttackCooldownTimer >= m_AttackCooldown - 0.7f)
		{
			m_BoilTimer = 0.f;
			m_CurrentState = MushroomState::popout;
			m_AccuSec = 0.f;
		}
		else if (m_BoilTimer >= 1.f)
		{
			m_BoilTimer -= 1.f;
			m_CurrentState = MushroomState::popout;
			m_AccuSec = 0.f;
		}
		break;
	case Mushroom::MushroomState::death:
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= 1.f)
		{
			m_DeathMarker = true;
		}
		break;
	}

	if (!m_AllowedAttack)
	{
		m_AttackCooldownTimer += elapsedSec;
		if (m_AttackCooldownTimer >= m_AttackCooldown)
		{
			m_AttackCooldownTimer -= m_AttackCooldown;
			m_AllowedAttack = true;
		}
	}

	ResetAnimator(frameToResetAnimation);
}

void Mushroom::Animate(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case Mushroom::MushroomState::idle:
		m_CurrentTexture = m_TextureIdle;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 1;

		m_Animator.BounceBetween(elapsedSec, 0, 4, 0.07f);
		break;
	case Mushroom::MushroomState::popout:
		m_CurrentTexture = m_TexturePop;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 2;

		m_Animator.PlayAnimation(elapsedSec, 0.07f);
		break;
	case Mushroom::MushroomState::attack:
		m_CurrentTexture = m_TextureAttack;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 3;

		m_Animator.PlayAnimation(elapsedSec, 0.07f);
		break;
	case Mushroom::MushroomState::popin:
		m_CurrentTexture = m_TexturePop;
		m_CurrentSpriteColNr = 5;
		m_CurrentSpriteRowNr = 2;

		m_Animator.ReverseAnimate(elapsedSec, 9, 0.07f);
		break;
	case Mushroom::MushroomState::boil:
		m_CurrentTexture = m_TextureBoiled;
		m_CurrentSpriteColNr = 1;
		m_CurrentSpriteRowNr = 1;

		break;
	case Mushroom::MushroomState::death:
		m_CurrentTexture = m_TextureDeath;
		m_CurrentSpriteColNr = 1;
		m_CurrentSpriteRowNr = 1;

		break;
	}
	m_CurrentFrameWidth = m_CurrentTexture->GetWidth() / m_CurrentSpriteColNr;
	m_CurrentFrameHeight = m_CurrentTexture->GetHeight() / m_CurrentSpriteRowNr;
}

Rectf Mushroom::GetBounds() const
{
	return Rectf(m_Positon.x - (m_CurrentFrameWidth * 0.75f) / 2, m_Positon.y, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f);
}

void Mushroom::TakeDamage(int damage)
{
	if (m_CurrentState != MushroomState::boil && m_CurrentState != MushroomState::death)
	{
		m_Hp -= damage;
	}
}

int Mushroom::GetHealth() const
{
	return m_Hp;
}

bool Mushroom::MarkedForDeath() const
{
	return m_DeathMarker;
}

bool Mushroom::PlayerInRange(const Vector2f& playerPos, float range)
{
	return (std::abs(m_Positon.x - playerPos.x)) < range;
}

void Mushroom::ResetAnimator(int frameToReset)
{
	bool stateChanged{ m_CurrentState != m_LastMushroomState };

	if (stateChanged)
	{
		m_Animator.Reset(frameToReset);
		m_LastMushroomState = m_CurrentState;
	}
}

void Mushroom::UpdateFacingDirection(const Vector2f& cupheadPos)
{
	if (cupheadPos.x > m_Positon.x)
	{
		m_FacingAngle = 180.f;
	}
	else m_FacingAngle = 0.f;
}
