#include "pch.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Cuphead.h"
#include "utils.h"
#include "VisualEffectManager.h"
#include "Effect.h"

EnemyManager::EnemyManager(const Texture* enemyDeathVFX)
	: m_EnemiesVector{ nullptr },
	m_EnemyDeathSpriteVFX{ enemyDeathVFX }
{
	m_EnemiesVector.reserve(20);
}

EnemyManager::~EnemyManager() noexcept
{
	for (size_t i{}; i < m_EnemiesVector.size(); ++i)
	{
		if (m_EnemiesVector[i] != nullptr)
		{
			delete m_EnemiesVector[i];
			m_EnemiesVector[i] = nullptr;
		}
	}
	m_EnemiesVector.clear();
}

void EnemyManager::DrawEnemies(const Rectf& cameraBox) const
{
	for (Enemy* pEnemy : m_EnemiesVector)
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
	for (size_t i{}; i < m_EnemiesVector.size(); ++i)
	{
		if (m_EnemiesVector[i] != nullptr)
		{
			if (m_EnemiesVector[i]->MarkedForDeath())
			{
				vfxManager.AddEffect(new Effect(Vector2f{ m_EnemiesVector[i]->GetBounds().left, m_EnemiesVector[i]->GetBounds().bottom }, 
					m_EnemyDeathSpriteVFX, 10, 1, 0.1f * m_EnemiesVector[i]->GetBounds().width / 15));
				delete m_EnemiesVector[i];
				m_EnemiesVector[i] = nullptr;
			}
			else
			{
				if (std::abs(cameraBox.left - m_EnemiesVector[i]->GetBounds().left) <= cameraBox.width * 1.2f && cameraBox.left < (m_EnemiesVector[i]->GetBounds().left) + cameraBox.width * 0.2f)
				{
					m_EnemiesVector[i]->Update(elapsedSec, bulletManager, cuphead, uiManager);
				}
			}
		}
	}
}

void EnemyManager::AnimateEnemies(float elapsedSec, const Rectf& cameraBox)
{
	for (size_t i{}; i < m_EnemiesVector.size(); ++i)
	{
		if (m_EnemiesVector[i] != nullptr)
		{
			if (std::abs(m_EnemiesVector[i]->GetBounds().left - cameraBox.left) <= cameraBox.width * 1.5f)
			{
				m_EnemiesVector[i]->Animate(elapsedSec);
			}
		}
	}
}

void EnemyManager::DeleteAllEnemies() noexcept
{
	for (size_t i{}; i < m_EnemiesVector.size(); ++i)
	{
		delete m_EnemiesVector[i];
		m_EnemiesVector[i] = nullptr;
	}
}

void EnemyManager::AddEnemy(Enemy* enemy)
{
	m_EnemiesVector.push_back(enemy);
}

Enemy* EnemyManager::operator[](int index) const
{
	return m_EnemiesVector[index];
}

size_t EnemyManager::GetVectorSize() const
{
	return m_EnemiesVector.size();
}
