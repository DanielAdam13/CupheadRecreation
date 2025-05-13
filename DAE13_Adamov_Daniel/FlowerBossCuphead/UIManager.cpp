#include "pch.h"
#include "UIManager.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

UIManager::UIManager(const Texture* healthDisplay, const Texture* cards, const Texture* intro, const Texture* death, const Texture* completion, const Texture* pause)
	:m_HealthTexture{ healthDisplay },
	m_CardsTexture{ cards },
	m_IntroTexture{ intro },
	m_DeathTexture{ death },
	m_CompletionTexture{ completion },
	m_PauseScreenTexture{ pause },
	m_CurrentFrameHp{ 0 },
	m_CardIdx{ 0 },
	m_PlayingIntro{ false },
	m_Paused{ false },
	m_PlayingDeathAnnouncment{ false },
	m_DrawingDeathScreen{ false },
	m_Animator{},
	m_StateAccuSec{ 0.f }
{
}

void UIManager::Update(float elapsedSec, int cupheadHp, int currentGameStateIdx)
{
	UpdateHealth(elapsedSec, cupheadHp);
	UpdateCards();

	if (currentGameStateIdx == 2)
	{
		m_Paused = true;
	}
	else m_Paused = false;

	if (currentGameStateIdx == 0)
	{
		m_PlayingIntro = true;
		m_Animator.PlayAnimation(elapsedSec, 0.06f);
	} 
	else m_PlayingIntro = false;


	if (currentGameStateIdx == 3)
	{
		HandleDeathUI(elapsedSec);
	}
}

void UIManager::Draw(const Rectf& cameraBox) const
{
	DrawHealthBar(cameraBox);
	DrawCards(cameraBox);

	if (m_PlayingIntro)
	{
		const float frameWidth{ m_IntroTexture->GetWidth() / 7 };
		const float frameHeight{ m_IntroTexture->GetHeight() / 7 };
		const Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 7) * frameWidth, 0.f + (m_Animator.GetCurrentFrameNr() / 7) * frameHeight, frameWidth, frameHeight };

		m_IntroTexture->Draw(cameraBox, srcRect);
	}

	if (m_Paused)
	{
		utils::SetColor(Color4f{ 0, 0, 0, 0.5f });
		utils::FillRect(cameraBox);
		m_PauseScreenTexture->Draw(Rectf{cameraBox.left + cameraBox.width * 0.25f, cameraBox.height * 0.25f, cameraBox.width * 0.5f, cameraBox.height * 0.5f});
	}

	if (m_PlayingDeathAnnouncment)
	{
		const float frameWidth{ m_DeathTexture->GetWidth() / 3 };
		const float frameHeight{ m_DeathTexture->GetHeight() / 7 };
		const Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 3) * frameWidth, 0.f + (m_Animator.GetCurrentFrameNr() / 3) * frameHeight, frameWidth, frameHeight };

		m_DeathTexture->Draw(Rectf{cameraBox.left + cameraBox.width * 0.05f, cameraBox.bottom + cameraBox.height * 0.15f, cameraBox.width * 0.9f, cameraBox.height * 0.7f}, srcRect);
	}

	if (m_DrawingDeathScreen)
	{
		utils::SetColor(Color4f{ 0, 0, 0, 0.5f });
		utils::FillRect(cameraBox);
	}
}

void UIManager::ChangeCards(int index)
{
	m_CardIdx += index;
}

bool UIManager::SpecialAttackAllowed() const
{
	return m_CardIdx >= 20;
}

void UIManager::UpdateCards()
{
	if (m_CardIdx > 100)
	{
		m_CardIdx = 100;
		// sound effect?
	}
}

void UIManager::UpdateHealth(float elapsedSec, int cupheadHp)
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
}

void UIManager::DrawCards(const Rectf& cameraBox) const
{
	const float cardTextureWidth{ m_CardsTexture->GetWidth() / 6 };
	const float cardTextureHeight{ m_CardsTexture->GetHeight() };

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
		m_CardsTexture->Draw(dstRect, srcRect); // full cards
	}
}

void UIManager::DrawHealthBar(const Rectf& cameraBox) const
{
	const float healthTextureWidth{ m_HealthTexture->GetWidth() / 6 };
	const float healthTextureHeight{ m_HealthTexture->GetHeight() / 2 };

	m_HealthTexture->Draw(Vector2f{ cameraBox.left + healthTextureWidth * 0.3f, cameraBox.bottom + healthTextureHeight * 1.7f },
		Rectf{ 0.f + m_CurrentFrameHp * healthTextureWidth, 0.f, healthTextureWidth, healthTextureHeight });
}

void UIManager::HandleDeathUI(float elapsedSec)
{
	m_PlayingDeathAnnouncment = true;

	if (m_PlayingDeathAnnouncment)
	{
		m_StateAccuSec += elapsedSec;

		if (m_StateAccuSec >= 1.f)
		{
			m_PlayingDeathAnnouncment = false;
			m_DrawingDeathScreen = true;
			m_Animator.Reset(0);
		}
		else m_Animator.PlayAnimation(elapsedSec, 0.07f);
	}
}
