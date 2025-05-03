#include "pch.h"
#include "Explosion.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>

Explosion::Explosion(const Texture* sprite, const Vector2f& pos)
	:Projectile::Projectile(sprite, pos, pos, 0, 0, 0),
	m_FrameWidth{sprite->GetWidth() / 5},
	m_FrameHeight{sprite->GetHeight() / 4},
	m_AccuSec{ 0.f },
	m_Duration{ 1.5f },
	m_Animator{},
	m_DeleteMarker{ false }
{
	std::cout << "Created Explosion" << std::endl;
}

void Explosion::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 5) * m_FrameWidth,
		0.f + (m_Animator.GetCurrentFrameNr() / 5) * m_FrameHeight, m_FrameHeight, m_FrameHeight };

	m_Texture->Draw(m_Position, srcRect);
	
	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
}

void Explosion::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_Duration)
	{
		m_DeleteMarker = true;
		std::cout << "Should delete explosion" << std::endl;
		m_AccuSec -= m_Duration;
	}
}

void Explosion::Animate(float elapsedSec)
{
	m_Animator.PlayAnimation(elapsedSec, 0.07f, 19);
}

Rectf Explosion::GetBounds() const
{
	return Rectf(m_Position.x - m_FrameWidth / 2, m_Position.y, m_FrameWidth, m_FrameHeight);
}

Circlef Explosion::GetHitbox() const
{
	return Circlef();
}

bool Explosion::Parryable() const
{
	return false;
}

int Explosion::GetDamage() const
{
	return m_Damage;
}

bool Explosion::DissapearOnGroundImpact()
{
	return false;
}

bool Explosion::MarkedForDeletion() const
{
	return m_DeleteMarker;
}
