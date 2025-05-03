#include "pch.h"
#include "TulipSeed.h"
#include "utils.h"
#include <iostream>
#include "Texture.h"

TulipSeed::TulipSeed(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, int damage)
	:Projectile::Projectile(sprite, spawnPos, playerPos, directionAngle, speed, damage),
	m_InitialPlayerPoint{ playerPos },
	m_StartPoint{ spawnPos },
	m_ControlPoint{ (spawnPos + m_InitialPlayerPoint) / 2 + Vector2f{0.f, 800.f} },
	m_Duration{ 1.5f },
	m_AccuSec{ 0.f },
	m_DelayAccuSec{ 0.f }
{
	std::cout << "CREATED Tulip Seed" << std::endl;
}

void TulipSeed::Draw() const
{
	if (m_DelayAccuSec > 0.5f)
	{
		glPushMatrix();
			glTranslatef(m_Position.x, m_Position.y, 0);
			glRotatef(m_DirectionAngle, 0, 0, 1);
			glScalef(0.8f, 0.8f, 0);
			m_Texture->Draw(Vector2f{ -m_Texture->GetWidth() / 2, -m_Texture->GetHeight() / 2 }, {});
		glPopMatrix();

		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawEllipse(GetHitbox().center, 30.f, 30.f);
		utils::FillEllipse(m_Position, 5.f, 5.f);
	}
}

void TulipSeed::Update(float elapsedSec) // Moving seed by bezier curve
{
	m_DelayAccuSec += elapsedSec;

	if (m_DelayAccuSec > 0.5f)
	{
		m_AccuSec += elapsedSec;
		const float t{ m_AccuSec / m_Duration };

		m_Position = CalculateBezierPoint(t);
	}
}

void TulipSeed::Animate(float elapsedSec)
{
	m_DirectionAngle += 1.8f;

	if (m_DirectionAngle >= 360.f)
	{
		m_DirectionAngle = 0.f;
	}
}

Rectf TulipSeed::GetBounds() const
{
	return Rectf(m_Position.x - m_Texture->GetWidth() / 2, m_Position.y, m_Texture->GetWidth(), m_Texture->GetHeight());
}

Circlef TulipSeed::GetHitbox() const
{
	return Circlef(m_Position, 30.f);
}

bool TulipSeed::Parryable() const
{
	return false;
}

int TulipSeed::GetDamage() const
{
	return m_Damage;
}

bool TulipSeed::DissapearOnGroundImpact()
{
	return true;
}

bool TulipSeed::MarkedForDeletion() const
{
	return false;
}

Vector2f TulipSeed::CalculateBezierPoint(float t) const // Formula for bezier curve by 3 points!
{
	const float u = 1 - t;
	return Vector2f{ u * u * m_StartPoint.x + 2 * u * t * m_ControlPoint.x + t * t * m_InitialPlayerPoint.x,
	u * u * m_StartPoint.y + 2 * u * t * m_ControlPoint.y + t * t * m_InitialPlayerPoint.y };
}
