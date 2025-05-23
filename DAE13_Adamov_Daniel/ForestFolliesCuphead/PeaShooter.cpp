#include "pch.h"
#include "PeaShooter.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>

PeaShooter::PeaShooter(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, float damage, int colNr, int rowNr)
	: Projectile::Projectile(sprite, spawnPos, playerPos, directionAngle, speed, damage),
	m_FrameWidth{m_Texture->GetWidth() / colNr},
	m_FrameHeight{m_Texture->GetHeight() / rowNr}
{
	//std::cout << "CREATED PeaShooter with direction: " << directionAngle << std::endl;
}

void PeaShooter::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 4) * GetBounds().width,
		0.f + (m_Animator.GetCurrentFrameNr() / 4) * GetBounds().height, GetBounds().width, GetBounds().height };

	glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0);
		glRotatef(m_DirectionAngle, 0, 0, 1);
		glScalef(0.8f, 0.8f, 0);
		m_Texture->Draw(Vector2f{ -srcRect.width / 2, -srcRect.height / 2 }, srcRect);
	glPopMatrix();

	// Hitbox
	/*utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawEllipse(GetHitbox().center, GetHitbox().radius, GetHitbox().radius);*/
}

void PeaShooter::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager)
{
	m_Position += m_ShootDirection * m_Speed * elapsedSec;
}

void PeaShooter::Animate(float elapsedSec)
{
	m_Animator.AnimateBetweenFrames(elapsedSec, 3, 7, 0.06f);
}

Rectf PeaShooter::GetBounds() const
{
	return Rectf{ m_Position.x - m_FrameWidth / 2, m_Position.y, m_FrameWidth, m_FrameHeight };
}

Circlef PeaShooter::GetHitbox() const
{
	return Circlef(Vector2f{m_Position.x + m_ShootDirection.x * 25.f, m_Position.y + m_ShootDirection.y * 25.f}, m_FrameWidth / 8);
}

float PeaShooter::Damage() const
{
	return m_Damage;
}

bool PeaShooter::MarkedForDeletion() const
{
	return m_DeleteMarker;
}
