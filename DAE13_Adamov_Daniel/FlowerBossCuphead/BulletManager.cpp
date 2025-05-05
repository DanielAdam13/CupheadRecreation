#include "pch.h"
#include "BulletManager.h"
#include "Projectile.h"
#include "utils.h"
#include <iostream>

BulletManager::BulletManager()
	:m_Projectiles{ nullptr }
{
	m_Projectiles.reserve(50);
}

BulletManager::~BulletManager()
{
	for (Projectile* pProj : m_Projectiles)
	{
		if (pProj != nullptr)
		{
			delete pProj;
			pProj = nullptr;
		}
	}
	m_Projectiles.clear();
}

void BulletManager::DrawActiveBullets() const
{
	for (Projectile* pProj : m_Projectiles)
	{
		if (pProj != nullptr)
		{
			pProj->Draw();
		}
	}
}

void BulletManager::UpdateActiveBullets(float elapsedSec, const Rectf& cameraBox, const std::vector<Vector2f>& vertices, Cuphead& cuphead)
{
	for (size_t i{}; i < m_Projectiles.size(); ++i)
	{
		if (m_Projectiles[i] != nullptr)
		{
			if (!m_Projectiles[i]->MarkedForDeletion())
			{
				if (utils::IsOverlapping(Rectf{ cameraBox.left, cameraBox.bottom, cameraBox.width, cameraBox.height * 1.2f }, m_Projectiles[i]->GetHitbox())) // a bit higher than camera y
				{
					m_Projectiles[i]->Update(elapsedSec, vertices, *this, cuphead);
				}
				else
				{
					delete m_Projectiles[i];
					m_Projectiles[i] = nullptr;
				}
			}
			else
			{
				delete m_Projectiles[i];
				m_Projectiles[i] = nullptr;
			}
		}
	}
}

void BulletManager::AnimateActiveBullets(float elapsedSec)
{
	for (size_t i{}; i < m_Projectiles.size(); ++i)
	{
		if (m_Projectiles[i] != nullptr)
		{
			m_Projectiles[i]->Animate(elapsedSec);
		}
	}
}

void BulletManager::AddProjectile(Projectile* projectile)
{
	m_Projectiles.push_back(projectile);
}

Projectile* BulletManager::operator[](int index) const
{
	return m_Projectiles[index];
}

size_t BulletManager::GetVectorSize() const
{
	return m_Projectiles.size();
}

void BulletManager::RemoveProjectile(int index)
{
	delete m_Projectiles[index];
	m_Projectiles[index] = nullptr;
}
