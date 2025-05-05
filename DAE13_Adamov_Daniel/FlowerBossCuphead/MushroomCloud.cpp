#include "pch.h"
#include "MushroomCloud.h"
#include "Texture.h"
#include "utils.h"
#include "Cuphead.h"

MushroomCloud::MushroomCloud(const Texture* sprite, const Vector2f& startPos, const Vector2f& playerPos, float directionAngle, float speed, int damage, bool parryable)
	:Projectile::Projectile(sprite, startPos, playerPos, directionAngle, speed, damage),
	m_ColNr{ 5 },
	m_RowNr{ 4 },
	m_FrameWidth{ sprite->GetWidth() / m_ColNr },
	m_FrameHeight{ sprite->GetHeight() / m_RowNr },
	m_TargetPoint{ playerPos },
	m_StartPoint{ startPos },
	m_Parryable{ parryable }
{
	if (m_Parryable) m_Animator.Reset(10);
}

void MushroomCloud::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 5) * m_FrameWidth,
		0.f + (m_Animator.GetCurrentFrameNr() / 5) * m_FrameHeight, m_FrameWidth, m_FrameHeight };

	m_Texture->Draw(GetBounds(), srcRect);

	// Hitbox
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
	utils::FillEllipse(m_Position, 5.f, 5.f);

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawEllipse(GetHitbox().center.x, GetHitbox().center.y, GetHitbox().radius, GetHitbox().radius);

	if (m_Parryable)
	{
		utils::DrawRect(GetParryHitbox());
	}
	
}

void MushroomCloud::Update(float elapsedSec, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, Cuphead& cuphead)
{
	m_ShootDirection = (m_TargetPoint - m_StartPoint).Normalized();

	m_Position += m_ShootDirection * m_Speed * elapsedSec;

	if (m_Parryable)
	{
		if (cuphead.IsParrying())
		{
			if (utils::IsOverlapping(GetParryHitbox(), cuphead.GetBounds()))
			{
				cuphead.Parry();
				m_DeleteMarker = true;
			}
		}
		
	}
}

void MushroomCloud::Animate(float elapsedSec)
{
	if (!m_Parryable)
	{
		m_Animator.BounceBetween(elapsedSec, 0, 9, 0.07f);
	}
	else m_Animator.BounceBetween(elapsedSec, 10, 19, 0.07f);
}

Circlef MushroomCloud::GetHitbox() const
{
	return Circlef(m_Position.x, m_Position.y + GetBounds().height / 2, 30.f);
}

int MushroomCloud::Damage() const
{
	return m_Damage;
}

bool MushroomCloud::MarkedForDeletion() const
{
	return m_DeleteMarker;
}

Rectf MushroomCloud::GetBounds() const
{
	return Rectf(m_Position.x - m_FrameWidth * 0.75f / 2, m_Position.y, m_FrameWidth * 0.75f, m_FrameHeight * 0.75f);
}

Rectf MushroomCloud::GetParryHitbox() const
{
	return Rectf(GetBounds().left - GetBounds().width / 4, GetBounds().bottom - GetBounds().height / 4, GetBounds().width * 1.5f, GetBounds().height * 1.5f);
}
