#include "pch.h"
#include "EnemyManager.h"
#include "Texture.h"
#include "Enemy.h"
#include "Spike.h"
#include "BigChomper.h"
#include "Tulip.h"
#include "Cuphead.h"
#include "utils.h"

int EnemyManager::m_NREnemies{ 0 };

EnemyManager::EnemyManager()
	: m_ChomperSprite{ new Texture("Run'N'Gun/Sprite_Chomper.png") },
	m_SpikeSprite{ new Texture("Run'N'Gun/Sprite_Spike.png") },
	m_TulipIdle{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Idle.png") },
	m_TulipAttack{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Attack.png") }
{
	IntializeEnemies();
	m_EnemiesVector.reserve(20);
}

EnemyManager::~EnemyManager()
{
	delete m_ChomperSprite;
	m_ChomperSprite = nullptr;

	delete m_SpikeSprite;
	m_SpikeSprite = nullptr;

	delete m_TulipIdle;
	m_TulipIdle = nullptr;

	delete m_TulipAttack;
	m_TulipAttack = nullptr;

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
			if (std::abs(m_EnemiesVector[i]->GetBounds().left - cameraBox.left) <= cameraBox.width * 1.5f)
			{
				m_EnemiesVector[i]->Update(elapsedSec, bulletManager, cuphead);
			}

			if (m_EnemiesVector[i]->Parryable())
			{
				if (cuphead.IsParrying())
				{
					if (utils::IsOverlapping(m_EnemiesVector[i]->GetParryHitbox(), cuphead.GetBounds()))
					{
						cuphead.Parry();
						delete m_EnemiesVector[i];
						m_EnemiesVector[i] = nullptr;
					}
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

Enemy* EnemyManager::operator[](int index) const
{
	return m_EnemiesVector[index];
}

size_t EnemyManager::GetVectorSize() const
{
	return m_EnemiesVector.size();
}

void EnemyManager::IntializeEnemies()
{
	m_EnemiesVector.reserve(5);

	m_EnemiesVector.push_back(new Spike(m_SpikeSprite, Vector2f{ 500.f, 250.f }, Vector2f{ 500.f, 150.f }, Vector2f{ 500.f, 350.f }, 250.f));

	//m_EnemiesVector.push_back(new Spike(m_SpikeSprite, Vector2f{ 1500.f, 250.f }, Vector2f{ 1500.f, 150.f }, Vector2f{ 1500.f, 350.f }, 250.f));

	m_EnemiesVector.push_back(new BigChomper(m_ChomperSprite, Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 1200.f, 500.f }, 600.f));

	m_EnemiesVector.push_back(new Spike(m_SpikeSprite, Vector2f{ 1700.f, 250.f }, Vector2f{ 500.f, 150.f }, Vector2f{ 500.f, 350.f }, 250.f));

	m_EnemiesVector.push_back(new BigChomper(m_ChomperSprite, Vector2f{ 2200.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 2200.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{2200.f, 450.f}, 600.f));

	m_EnemiesVector.push_back(new Tulip(m_TulipIdle, m_TulipAttack, Vector2f{ 1000.f, 170.f }));
}
