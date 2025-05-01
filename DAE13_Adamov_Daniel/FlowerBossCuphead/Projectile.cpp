#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

Projectile::Projectile(Texture* sprite, const Vector2f& spawnPos, float directionAngle, int damage)
	:m_Position{ spawnPos },
	m_Speed{ 1200.f },
	m_ShootDirection{ cosf(directionAngle * utils::g_Pi / 180.f), sinf(directionAngle * utils::g_Pi / 180.f) },
	m_DirectionAngle{ directionAngle },
	m_Damage{ damage },
	m_ProjAnimator{},
	m_Texture{ sprite }
{
	std::cout << "CREATED projectile with direction: " << directionAngle << std::endl;
}

void Projectile::Draw() const
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

void Projectile::Update(float elapsedSec)
{
	m_Position += m_ShootDirection * m_Speed * elapsedSec;
	Animate(elapsedSec);
}

void Projectile::Animate(float elapsedSec)
{
	m_ProjAnimator.LoopBetween(elapsedSec, 3, 7, 0.06f);
}

Rectf Projectile::GetBounds() const
{
	if (this != nullptr)
	{
		const float frameWidth{ m_Texture->GetWidth() / 4 };
		const float frameHeight{ m_Texture->GetHeight() / 2 };

		return Rectf{ m_Position.x - frameWidth / 2, m_Position.y, frameWidth, frameHeight };
	}
	return Rectf();
}
