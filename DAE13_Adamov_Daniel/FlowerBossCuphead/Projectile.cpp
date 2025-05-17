#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

Projectile::Projectile(const Texture* sprite, const Vector2f& spawnPos, const Vector2f& playerPos, float directionAngle, float speed, float damage)
	:m_Position{ spawnPos },
	m_ShootDirection{ cosf(directionAngle * utils::g_Pi / 180.f), sinf(directionAngle * utils::g_Pi / 180.f) },
	m_DirectionAngle{ directionAngle },
	m_Damage{ damage },
	m_Animator{},
	m_Texture{ sprite },
	m_Speed{ speed },
	m_DeleteMarker{ false }
{
}
