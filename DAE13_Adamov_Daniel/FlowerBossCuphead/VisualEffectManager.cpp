#include "pch.h"
#include "VisualEffectManager.h"
#include "Effect.h"

VisualEffectManager::VisualEffectManager()
	:m_EffectsVector{}
{
}

VisualEffectManager::~VisualEffectManager() noexcept
{
	for (Effect* pEffect : m_EffectsVector)
	{
		if (pEffect != nullptr)
		{
			delete pEffect;
			pEffect = nullptr;
		}
	}
}

void VisualEffectManager::DrawEffects() const
{
	for (size_t i{}; i < m_EffectsVector.size(); ++i)
	{
		if (m_EffectsVector[i] != nullptr)
		{
			m_EffectsVector[i]->Draw();
		}
	}
}

void VisualEffectManager::UpdateEffects(float elapsedSec)
{
	for (size_t i{}; i < m_EffectsVector.size(); ++i)
	{
		if (m_EffectsVector[i] != nullptr)
		{
			if (!m_EffectsVector[i]->MarkedForDeletion())
			{
				m_EffectsVector[i]->Update(elapsedSec);
			}
			else
			{
				delete m_EffectsVector[i];
				m_EffectsVector[i] = nullptr;
			}
		}
	}
}

void VisualEffectManager::AddEffect(Effect* effect)
{
	m_EffectsVector.push_back(effect);
}
