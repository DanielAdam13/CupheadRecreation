#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

int Projectile::m_NumProjectiles{ 0 };
//bool Projectile::m_GameOver{ false };

Texture* Projectile::ProjectileSprite()
{
	static Texture* projectileSprite{ new Texture("Projectile_Loop.png") };
	return projectileSprite;
}

Projectile::Projectile(const Vector2f& spawnPos, float directionAngle, int damage)
	:m_Position{ spawnPos },
	m_Speed{ 1200.f },
	m_ShootDirection{ cosf(directionAngle * utils::g_Pi / 180.f), sinf(directionAngle * utils::g_Pi / 180.f) },
	m_DirectionAngle{ directionAngle },
	m_Damage{ damage },
	m_ProjAnimator{}
{
	m_NumProjectiles++;
	std::cout << "Created projectile with direction: " << directionAngle << std::endl;
}

Projectile::Projectile(const Vector2f& spawnPos, float directionAngle)
	:Projectile::Projectile(spawnPos, directionAngle, 1)
{
}

Projectile::~Projectile()
{
	m_NumProjectiles--;
	if (m_NumProjectiles == 0)
	{
		delete Projectile::ProjectileSprite();
	}
}

void Projectile::Draw() const
{
	Texture* projectileTexture{ ProjectileSprite() };

	/*projectileTexture->Draw(GetBounds(), Rectf{ 0.f + (m_ProjAnimator.GetCurrentFrame() % 4) * GetBounds().width,
		0.f + (m_ProjAnimator.GetCurrentFrame() / 4) * GetBounds().height, GetBounds().width, GetBounds().height });*/

	Rectf srcRect{ 0.f + (m_ProjAnimator.GetCurrentFrame() % 4) * GetBounds().width,
		0.f + (m_ProjAnimator.GetCurrentFrame() / 4) * GetBounds().height, GetBounds().width, GetBounds().height };

	glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0);
		glRotatef(m_DirectionAngle, 0, 0, 1);
		glScalef(0.8f, 0.8f, 0);
		projectileTexture->Draw(Vector2f{ -srcRect.width / 2, -srcRect.height / 2 }, srcRect);
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
	Texture* projectileTexture{ ProjectileSprite() };
	const float frameWidth{ projectileTexture->GetWidth() / 4 };
	const float frameHeight{ projectileTexture->GetHeight() / 2 };

	return Rectf(m_Position.x - frameWidth / 2, m_Position.y, frameWidth, frameHeight);
}
