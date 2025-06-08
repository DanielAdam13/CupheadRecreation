#include "pch.h"
#include "Coin.h"
#include "Texture.h"

Coin::Coin(const Vector2f& pos, const Texture* sprite)
	:m_Position{ pos },
	m_CurrentState{},
	m_Animator{},
	m_CoinSprite{ sprite },
	m_AccSec{ 0.f },
	m_CurrentFrameWidth{},
	m_CurrentFrameHeight{},
	m_CurrentColNr{},
	m_CurrentRowNr{},
	m_DeathMarker{ false }
{
}

void Coin::Update(float elapsedSec)
{
	switch (m_CurrentState)
	{
	case Coin::CoinState::existing:
		m_CurrentColNr = 16;
		m_CurrentRowNr = 1;

		m_CurrentFrameWidth = m_CoinSprite->GetWidth() / m_CurrentColNr;
		m_CurrentFrameHeight = 140.f;
		break;
	case Coin::CoinState::taken:
		m_CurrentColNr = 5;
		m_CurrentRowNr = 4;

		m_CurrentFrameWidth = m_CoinSprite->GetWidth() / m_CurrentColNr;
		m_CurrentFrameHeight = (m_CoinSprite->GetHeight() - 175.f) / m_CurrentRowNr;

		m_AccSec += elapsedSec;
		if (m_AccSec >= 1.01f)
		{
			m_DeathMarker = true;
			m_AccSec -= 1.01f;
		}
		break;
	}

	m_Animator.PlayAnimation(elapsedSec, 0.06f);
}

void Coin::Draw() const
{
	Rectf srcRect{};

	switch (m_CurrentState)
	{
	case Coin::CoinState::existing:
		srcRect = Rectf{ 2.f + (m_Animator.GetCurrentFrameNr() % m_CurrentColNr) * m_CurrentFrameWidth, 18.f, m_CurrentFrameWidth - 4.f, m_CurrentFrameHeight };
		break;
	case Coin::CoinState::taken:
		srcRect = Rectf{ 2.f + (m_Animator.GetCurrentFrameNr() % m_CurrentColNr) * m_CurrentFrameWidth, 
			176.f + (m_Animator.GetCurrentFrameNr() / m_CurrentColNr) * m_CurrentFrameHeight, 
			m_CurrentFrameWidth - 10.f, m_CurrentFrameHeight - 4.f};
		break;
	}

	m_CoinSprite->Draw(GetBounds(), srcRect);
}

void Coin::TriggerTaken()
{
	if (m_CurrentState != CoinState::taken)
	{
		m_CurrentState = CoinState::taken;
		m_Animator.Reset(0);
	}
}

bool Coin::EffectEnded() const
{
	return m_DeathMarker;
}

Rectf Coin::GetBounds() const
{
	return Rectf(m_Position.x - m_CurrentFrameWidth * 0.75f / 2, m_Position.y - m_CurrentFrameHeight * 0.75f / 2, m_CurrentFrameWidth * 0.75f, m_CurrentFrameHeight * 0.75f);
}

bool Coin::IsTaken() const
{
	return m_CurrentState == CoinState::taken;
}
