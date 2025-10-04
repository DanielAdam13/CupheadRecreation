#include "pch.h"
#include "CoinManager.h"
#include "Coin.h"
#include "Texture.h"
#include "utils.h"
#include "SoundEffect.h"
#include <iostream>

int CoinManager::coinsTaken{ 0 };

CoinManager::CoinManager(const std::unique_ptr<Texture>& coinTexture, const std::unique_ptr<SoundEffect>& coinPickupSFX)
    :m_CoinVector{},
    m_CoinTexture{ coinTexture.get() },
    m_CoinPickupSFX{ coinPickupSFX.get() }
{
    m_CoinVector.reserve(5);
}

CoinManager::~CoinManager() noexcept
{
    DeleteAllCoins();
}

void CoinManager::AddCoin(const Vector2f& pos)
{
    m_CoinVector.push_back(new Coin(pos, m_CoinTexture));
}

Coin* CoinManager::operator[](int index) const
{
    return m_CoinVector[index];
}

size_t CoinManager::GetVecorSize() const
{
    return m_CoinVector.size();
}

int CoinManager::GetTakenCoinsNumber() const
{
    return coinsTaken;
}

void CoinManager::DrawCoins(const Rectf& cameraBox) const
{
    for (const Coin* pCoin : m_CoinVector)
    {
        if (pCoin != nullptr)
        {
            if (std::abs(cameraBox.left - pCoin->GetBounds().left) <= cameraBox.width * 1.2f && cameraBox.left < (pCoin->GetBounds().left) + cameraBox.width * 0.2f)
            {
                pCoin->Draw();
            }
        }
    }
}

void CoinManager::UpdateCoins(float elapsedSec, const Rectf& cameraBox, const Circlef& playerBounds)
{
    for (size_t i{}; i < m_CoinVector.size(); ++i)
    {
        if (m_CoinVector[i] != nullptr)
        {
            if (!m_CoinVector[i]->EffectEnded())
            {
                if (utils::IsOverlapping(m_CoinVector[i]->GetBounds(), playerBounds) && !m_CoinVector[i]->IsTaken())
                {
                    m_CoinVector[i]->TriggerTaken();
                    m_CoinPickupSFX->Play(0);

                    coinsTaken++;
                    //std::cout << "Coins taken: " << coinsTaken << std::endl;
                }
                
                if (std::abs(cameraBox.left - m_CoinVector[i]->GetBounds().left) <= cameraBox.width * 1.2f && cameraBox.left < (m_CoinVector[i]->GetBounds().left) + cameraBox.width * 0.2f)
                {
                    m_CoinVector[i]->Update(elapsedSec);
                }
            }
            else
            {
                delete m_CoinVector[i];
                m_CoinVector[i] = nullptr;
            }
        }
    }
}

void CoinManager::DeleteAllCoins() noexcept
{
    for (size_t i{}; i < m_CoinVector.size(); ++i)
    {
        if (m_CoinVector[i] != nullptr)
        {
            delete m_CoinVector[i];
            m_CoinVector[i] = nullptr;
        }
    }
    m_CoinVector.clear();

    coinsTaken = 0;

    //std::cout << "Coins taken: " << coinsTaken << std::endl;
}
