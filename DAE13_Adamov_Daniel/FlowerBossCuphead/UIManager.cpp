#include "pch.h"
#include "UIManager.h"
#include "Texture.h"
#include <iostream>

UIManager::UIManager(const Texture* healthDisplay, const Texture* cards)
	:m_TextureHealth{ healthDisplay },
	m_TextureCards{ cards },
	m_CurrentFrameHp{ 0 },
	m_CardIdx{ 0 }
{
}

void UIManager::Update(float elapsedSec, int cupheadHp)
{
	static float accuSec{ 0.f };
	
	switch (cupheadHp)
	{
	case 3:
		m_CurrentFrameHp = 5;
		break;
	case 2:
		m_CurrentFrameHp = 4;
		break;
	case 1:
		if (m_CurrentFrameHp > 3)
		{
			m_CurrentFrameHp = 1;
		}
		else if (m_CurrentFrameHp < 1)
		{
			m_CurrentFrameHp = 3;
		}

		accuSec += elapsedSec;
		if (accuSec >= 0.2f)
		{
			m_CurrentFrameHp++;
			accuSec -= 0.2f;
		}
		break;
	case 0:
		m_CurrentFrameHp = 0;
		break;
	}

	if (m_CardIdx > 100)
	{
		m_CardIdx = 100;
		// sound effect?
	}
}

void UIManager::Draw(const Rectf& cameraBox) const
{
	const float healthTextureWidth{ m_TextureHealth->GetWidth() / 6 };
	const float healthTextureHeight{ m_TextureHealth->GetHeight() / 2 };

	const float cardTextureWidth{ m_TextureCards->GetWidth() / 6 };
	const float cardTextureHeight{ m_TextureCards->GetHeight() };

	m_TextureHealth->Draw(Vector2f{ cameraBox.left + healthTextureWidth * 0.3f, cameraBox.bottom + healthTextureHeight * 1.7f },
		Rectf{ 0.f + m_CurrentFrameHp * healthTextureWidth, 0.f, healthTextureWidth, healthTextureHeight });

	float cardNr{ m_CardIdx / 20.f };

	for (int i{}; i < ceilf(cardNr); ++i)
	{
		Rectf dstRect{ cameraBox.left + cardTextureWidth * 5.f + i * cardTextureWidth * 0.9f, cameraBox.bottom + cardTextureHeight * 2.f, cardTextureWidth, cardTextureHeight };
		Rectf srcRect{ 0.f + 5 * cardTextureWidth, 0.f, cardTextureWidth, cardTextureHeight };

		if (i == ceilf(cardNr) - 1) // not full card
		{
			if (m_CardIdx % 20 != 0)
			{
				float overflow{ (m_CardIdx % 20) / 20.f };
				dstRect.height *= overflow;
				srcRect = Rectf{ 0.f, 0.f, cardTextureWidth, cardTextureHeight * overflow }; // change the source rect to a smaller card height
			}
		}
		m_TextureCards->Draw(dstRect, srcRect); // full cards
	}
}

void UIManager::ChangeCards(float index)
{
	m_CardIdx += index;
}

bool UIManager::SpecialAttackAllowed() const
{
	return m_CardIdx >= 20;
}