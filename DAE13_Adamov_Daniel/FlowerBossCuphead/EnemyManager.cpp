#include "pch.h"
#include "EnemyManager.h"
#include "Texture.h"
#include "Enemy.h"
#include "Spike.h"
#include "BigChomper.h"

int EnemyManager::m_NREnemies{ 0 };

EnemyManager::EnemyManager()
	:m_ChomperSprite{ new Texture("Run'N'Gun/Sprite_Chomper.png") },
	m_SpikeSprite{ new Texture("Run'N'Gun/Sprite_Spike.png") }
{
	IntializeEnemies();
}

EnemyManager::~EnemyManager()
{
	delete m_ChomperSprite;
	m_ChomperSprite = nullptr;

	delete m_SpikeSprite;
	m_SpikeSprite = nullptr;

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

void EnemyManager::DrawEnemies() const
{
	for (Enemy* pEnemy : m_EnemiesVector)
	{
		if (pEnemy != nullptr)
		{
			pEnemy->Draw();
		}
	}
}

void EnemyManager::UpdateEnemies(float elapsedSec)
{
	for (Enemy* pEnemy : m_EnemiesVector)
	{
		if (Spike* spike = dynamic_cast<Spike*>(pEnemy))
		{
			spike->Update(elapsedSec);
		}
	}
}

void EnemyManager::IntializeEnemies()
{
	m_EnemiesVector.reserve(5);

	m_EnemiesVector.push_back(new Spike(m_SpikeSprite, Vector2f{ 500.f, 250.f }, Vector2f{ 500.f, 150.f }, Vector2f{ 500.f, 350.f }, 250.f));

	m_EnemiesVector.push_back(new Spike(m_SpikeSprite, Vector2f{ 1500.f, 250.f }, Vector2f{ 1500.f, 150.f }, Vector2f{ 1500.f, 350.f }, 250.f));

	m_EnemiesVector.push_back(new BigChomper(m_ChomperSprite, Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) }, Vector2f{ 1200.f, 500.f }, 600.f));

	m_EnemiesVector.push_back(new BigChomper(m_ChomperSprite, Vector2f{ 2200.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 2200.f, -(m_ChomperSprite->GetHeight() / 4) }, Vector2f{2200.f, 450.f}, 600.f));
}
