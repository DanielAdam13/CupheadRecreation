#include "pch.h"
#include "Acorn.h"
#include "Texture.h"
#include "Cuphead.h"
#include "utils.h"
#include <cassert>

Acorn::Acorn(const Texture* idle, const Texture* drop, const Vector2f& pos, const Vector2f& direction)
	:Enemy::Enemy(pos),
	m_Hp{ 3 },
	m_CurrentState{ AcornState::idle },
	m_TextureIdle{ idle },
	m_TextureDrop{ drop },
	m_CurrentTexture{ idle },
	m_CurrentColNr{ 5 },
	m_CurrentRowNr{ 3 },
	m_CurrentFrameWidth{ m_CurrentTexture->GetWidth() / m_CurrentColNr },
	m_CurrentFrameHeight{ m_CurrentTexture->GetHeight() / m_CurrentRowNr },
	m_Direction{ direction },
	m_FacingAngle{}
{
	if (direction.x < 0)
	{
		m_FacingAngle = 0.f;
	}
	else m_FacingAngle = 180.f;
}

void Acorn::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_CurrentColNr) * m_CurrentFrameWidth,
			0.f + (m_Animator.GetCurrentFrameNr() / m_CurrentColNr) * m_CurrentFrameHeight, m_CurrentFrameWidth, m_CurrentFrameHeight };

	glPushMatrix();
		glTranslatef(m_Positon.x, m_Positon.y, 0);
		glRotatef(m_FacingAngle, 0, 1, 0);
		m_CurrentTexture->Draw(Rectf{ -m_CurrentFrameWidth * 0.75f / 2, -m_CurrentFrameHeight * 0.75f / 2, GetBounds().width, GetBounds().height }, srcRect);
	glPopMatrix();

	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
	utils::FillEllipse(m_Positon, 5.f, 5.f);
}

void Acorn::Update(float elapsedSec, BulletManager& bulletManager, Cuphead& cuphead)
{
	float speed{ 500.f };

	if (m_Hp <= 0 || m_Positon.y <= cuphead.GetPosition().y - m_CurrentFrameWidth * 4)
	{
		m_DeathMarker = true;
	}

	switch (m_CurrentState)
	{
	case Acorn::AcornState::idle:
		if (std::abs(m_Positon.x - cuphead.GetPosition().x) <= 10.f)
		{
			m_CurrentState = AcornState::drop;
		}
		break;
	case Acorn::AcornState::drop:
		m_Direction = Vector2f{ 0, -1 };
		break;
	}

	m_Positon += m_Direction * speed * elapsedSec;
}

void Acorn::Animate(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case Acorn::AcornState::idle:
		m_CurrentTexture = m_TextureIdle;

		m_Animator.BounceBetween(elapsedSec, 0, 10, 0.06f);
		break;
	case Acorn::AcornState::drop:
		m_CurrentTexture = m_TextureDrop;

		m_Animator.PlayAnimation(elapsedSec, 0.06f, 13);
		break;
	}
	m_CurrentFrameWidth = m_CurrentTexture->GetWidth() / m_CurrentColNr;
	m_CurrentFrameHeight = m_CurrentTexture->GetHeight() / m_CurrentRowNr;
}

Rectf Acorn::GetBounds() const
{
	return Rectf(m_Positon.x - m_CurrentFrameWidth / 2, m_Positon.y - m_CurrentFrameHeight / 2, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f);
}

void Acorn::TakeDamage(int damage)
{
	m_Hp -= damage;
}

int Acorn::GetHealth() const
{
	return m_Hp;
}

bool Acorn::MarkedForDeath() const
{
	return m_DeathMarker;
}
