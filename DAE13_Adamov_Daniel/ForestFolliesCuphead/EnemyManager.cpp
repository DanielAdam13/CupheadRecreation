#include "pch.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Cuphead.h"
#include "utils.h"
#include "VisualEffectManager.h"
#include "Effect.h"

EnemyManager::EnemyManager(const Texture* enemyDeathVFX)
	: m_EnemyVector{ nullptr },
	m_EnemyDeathSpriteVFX{ enemyDeathVFX }
{
	m_EnemyVector.reserve(20);
}

EnemyManager::~EnemyManager() noexcept
{
	DeleteAllEnemies();
}

void EnemyManager::DrawEnemies(const Rectf& cameraBox) const
{
	for (const Enemy* pEnemy : m_EnemyVector)
	{
		if (pEnemy != nullptr)
		{
			if (std::abs(pEnemy->GetBounds().left - cameraBox.left) <= cameraBox.width * 1.5f)
			{
				pEnemy->Draw();
			}
		}
	}
}

void EnemyManager::UpdateEnemies(float elapsedSec, const Rectf& cameraBox, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager, VisualEffectManager& vfxManager)
{
	for (size_t i{}; i < m_EnemyVector.size(); ++i)
	{
		if (m_EnemyVector[i] != nullptr)
		{
			if (m_EnemyVector[i]->MarkedForDeath())
			{
				vfxManager.AddEffect(new Effect(Vector2f{ m_EnemyVector[i]->GetBounds().left, m_EnemyVector[i]->GetBounds().bottom }, 
					m_EnemyDeathSpriteVFX, 10, 1, 0.1f * m_EnemyVector[i]->GetBounds().width / 15));
				delete m_EnemyVector[i];
				m_EnemyVector[i] = nullptr;
			}
			else
			{
				if (std::abs(cameraBox.left - m_EnemyVector[i]->GetBounds().left) <= cameraBox.width * 1.2f && cameraBox.left < (m_EnemyVector[i]->GetBounds().left) + cameraBox.width * 0.2f)
				{
					m_EnemyVector[i]->Update(elapsedSec, bulletManager, cuphead, uiManager);
				}
			}
		}
	}
}

void EnemyManager::AnimateEnemies(float elapsedSec, const Rectf& cameraBox)
{
	for (size_t i{}; i < m_EnemyVector.size(); ++i)
	{
		if (m_EnemyVector[i] != nullptr)
		{
			if (std::abs(m_EnemyVector[i]->GetBounds().left - cameraBox.left) <= cameraBox.width * 1.5f)
			{
				m_EnemyVector[i]->Animate(elapsedSec);
			}
		}
	}
}

void EnemyManager::DeleteAllEnemies() noexcept
{
	for (size_t i{}; i < m_EnemyVector.size(); ++i)
	{
		if (m_EnemyVector[i] != nullptr)
		{
			delete m_EnemyVector[i];
			m_EnemyVector[i] = nullptr;
		}
	}
	m_EnemyVector.clear();
}

void EnemyManager::AddEnemy(Enemy* enemy)
{
	m_EnemyVector.push_back(enemy);
}

Enemy* EnemyManager::operator[](int index) const
{
	return m_EnemyVector[index];
}

size_t EnemyManager::GetVectorSize() const
{
	return m_EnemyVector.size();
}
