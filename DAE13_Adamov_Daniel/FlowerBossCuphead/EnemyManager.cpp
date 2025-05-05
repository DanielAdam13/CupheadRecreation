#include "pch.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Cuphead.h"
#include "utils.h"

EnemyManager::EnemyManager()
	: m_EnemiesVector{ nullptr }
{
	m_EnemiesVector.reserve(20);
}

EnemyManager::~EnemyManager()
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

void EnemyManager::UpdateEnemies(float elapsedSec, const Rectf& cameraBox, BulletManager& bulletManager, Cuphead& cuphead)
{
	for (size_t i{}; i < m_EnemiesVector.size(); ++i)
	{
		if (m_EnemiesVector[i] != nullptr)
		{
			if (m_EnemiesVector[i]->MarkedForDeath())
			{

				delete m_EnemiesVector[i];
				m_EnemiesVector[i] = nullptr;
			}
			else
			{
				if (std::abs(m_EnemiesVector[i]->GetBounds().left - cameraBox.left) <= cameraBox.width * 1.5f)
				{
					m_EnemiesVector[i]->Update(elapsedSec, bulletManager, cuphead);
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
