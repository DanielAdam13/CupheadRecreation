#include "pch.h"
#include "BulletManager.h"
#include "Projectile.h"
#include "Texture.h"
#include "utils.h"
#include "Camera.h"

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

void BulletManager::UpdateActiveBullets(float elapsedSec, const Camera& camera)
{
	for (size_t i{}; i < m_Projectiles.size(); ++i)
	{
		if (m_Projectiles[i] != nullptr)
		{
			if (utils::IsOverlapping(m_Projectiles[i]->GetBounds(), camera.GetCuurentCameraBounds()))
			{
				m_Projectiles[i]->Update(elapsedSec);
			}
			else
			{
				delete m_Projectiles[i];
				m_Projectiles[i] = nullptr;
			}
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
