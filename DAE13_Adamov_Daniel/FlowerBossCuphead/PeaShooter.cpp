#include "pch.h"
#include "PeaShooter.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>

PeaShooter::PeaShooter(const Texture* sprite, const Vector2f& spawnPos, float directionAngle, float speed, int damage)
	: Projectile::Projectile(sprite, spawnPos, directionAngle, speed, damage)
{
	std::cout << "CREATED PeaShooter with direction: " << directionAngle << std::endl;
}

void PeaShooter::Draw() const
{
	Rectf srcRect{ 0.f + (m_ProjAnimator.GetCurrentFrame() % 4) * GetBounds().width,
		0.f + (m_ProjAnimator.GetCurrentFrame() / 4) * GetBounds().height, GetBounds().width, GetBounds().height };

	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0);
	glRotatef(m_DirectionAngle, 0, 0, 1);
	glScalef(0.8f, 0.8f, 0);
	m_Texture->Draw(Vector2f{ -srcRect.width / 2, -srcRect.height / 2 }, srcRect);
	glPopMatrix();
}

void PeaShooter::Update(float elapsedSec, const std::vector<Vector2f>& vertices)
{
	m_Position += m_ShootDirection * m_Speed * elapsedSec;
}

void PeaShooter::Animate(float elapsedSec)
{
	m_ProjAnimator.LoopBetween(elapsedSec, 3, 7, 0.06f);
}

Rectf PeaShooter::GetBounds() const
{
	if (this != nullptr)
	{
		const float frameWidth{ m_Texture->GetWidth() / 4 };
		const float frameHeight{ m_Texture->GetHeight() / 2 };

		return Rectf{ m_Position.x - frameWidth / 2, m_Position.y, frameWidth, frameHeight };
	}
	return Rectf();
}