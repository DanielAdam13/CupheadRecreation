#include "pch.h"
#include "Explosion.h"
#include "Texture.h"
#include "utils.h"

Explosion::Explosion(const Texture* sprite, const Vector2f& pos)
	:Projectile::Projectile(sprite, pos, pos, 0, 0, 0),
	m_FrameWidth{sprite->GetWidth() / 5},
	m_FrameHeight{sprite->GetHeight() / 4},
	m_AccuSec{ 0.f },
	m_Duration{ 0.95f },
	m_Animator{}
{
}

void Explosion::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 5) * m_FrameWidth,
		0.f + (m_Animator.GetCurrentFrameNr() / 5) * m_FrameHeight, m_FrameWidth, m_FrameHeight };

	m_Texture->Draw(GetBounds(), srcRect);
	
	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
	utils::FillEllipse(m_Position, 15.f, 15.f);

	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawEllipse(Ellipsef{ GetHitbox().center, GetHitbox().radius, GetHitbox().radius});
}

void Explosion::Update(float elapsedSec, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_Duration)
	{
		m_DeleteMarker = true;
		m_AccuSec -= m_Duration;
	}
}

void Explosion::Animate(float elapsedSec)
{
	m_Animator.PlayAnimation(elapsedSec, 0.05f, 19);
}

Rectf Explosion::GetBounds() const
{
	return Rectf(m_Position.x - m_FrameWidth * 0.5f / 2, m_Position.y, m_FrameWidth * 0.5f, m_FrameHeight * 0.5f);
}

Circlef Explosion::GetHitbox() const
{
	return Circlef(m_Position.x, m_Position.y + GetBounds().height / 2, 45.f);
}

float Explosion::Damage() const
{
	return m_Damage;
}

bool Explosion::MarkedForDeletion() const
{
	return m_DeleteMarker;
}
