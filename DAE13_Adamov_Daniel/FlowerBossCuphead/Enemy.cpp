#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(const Vector2f& pos)
	: m_Positon{ pos },
	m_Animator{ Animator() },
	m_DeathMarker{ false }
{
}
