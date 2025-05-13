#include "pch.h"
#include "TulipSeed.h"
#include "utils.h"
#include "Texture.h"
#include "BulletManager.h"
#include "Explosion.h"

TulipSeed::TulipSeed(const Texture* sprite, const Texture* explosion, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, float damage)
	:Projectile::Projectile(sprite, spawnPos, playerPos, directionAngle, speed, damage),
	m_ExplosionTexture{ explosion },
	m_InitialPlayerPoint{ playerPos },
	m_StartPoint{ m_Position },
	m_ControlPoint{ (spawnPos + m_InitialPlayerPoint) / 2 + Vector2f{0.f, 800.f} },
	m_BezierDuration{ 1.5f },
	m_AccuSec{ 0.f },
	m_DelayAccuSec{ 0.f }
{
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

		// Hitbox
		/*utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawEllipse(GetHitbox().center, 30.f, 30.f);
		utils::FillEllipse(m_Position, 5.f, 5.f);*/
	}
}

void TulipSeed::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager) // Moving seed by bezier curve
{
	m_DelayAccuSec += elapsedSec;

	if (m_DelayAccuSec > 0.5f)
	{
		m_AccuSec += elapsedSec;
		const float t{ m_AccuSec / m_BezierDuration };

		m_Position = CalculateBezierPoint(t);
	}

	DissapearOnGroundImpact(vertices, bulletManager);
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

float TulipSeed::Damage() const
{
	return m_Damage;
}

bool TulipSeed::MarkedForDeletion() const
{
	return m_DeleteMarker;
}

Vector2f TulipSeed::CalculateBezierPoint(float t) const // Formula for bezier curve by 3 points!
{
	const float u = 1 - t;
	return Vector2f{ u * u * m_StartPoint.x + 2 * u * t * m_ControlPoint.x + t * t * m_InitialPlayerPoint.x,
	u * u * m_StartPoint.y + 2 * u * t * m_ControlPoint.y + t * t * m_InitialPlayerPoint.y };
}

void TulipSeed::DissapearOnGroundImpact(const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager)
{
	utils::HitInfo hitInfo;
	Vector2f first{ this->GetHitbox().center.x,this->GetHitbox().center.y };
	Vector2f second{ this->GetHitbox().center.x, this->GetHitbox().center.y + this->GetHitbox().radius };

	for (size_t i{}; i < vertices.size(); ++i)
	{
		if (utils::Raycast(vertices[i], first, second, hitInfo))
		{
			bulletManager.AddProjectile(new Explosion(m_ExplosionTexture, hitInfo.intersectPoint));

			m_DeleteMarker = true;
		}
	}
	
}
