#include "pch.h"
#include "UIManager.h"
#include "Texture.h"
#include <iostream>
#include "utils.h"

UIManager::UIManager(const Texture* healthDisplay, const Texture* cards, const Texture* intro, const Texture* death, 
	const Texture* completion, const Texture* pause, const Texture* deathScreen, const Texture* progressMan)
	:m_HealthTexture{ healthDisplay },
	m_CardsTexture{ cards },
	m_IntroAnnouncementTexture{ intro },
	m_DeathAnnouncementTexture{ death },
	m_CompletionAnnouncementTexture{ completion },
	m_PauseScreenTexture{ pause },
	m_DeathScreenCardTexture{ deathScreen },
	m_ProgressManTexture{ progressMan },
	m_CurrentFrameWidth{},
	m_CurrentFrameHeight{},
	m_CurrentFrameHp{ 0 },
	m_CardIdx{ 0 },
	m_PlayingIntro{ false },
	m_Paused{ false },
	m_PlayingDeathAnnouncement{ false },
	m_DrawingDeathScreen{ false },
	m_PlayingWinAnnounement{ false },
	m_ScreenAlpha{ 0.f },
	m_Animator{},
	m_StateAccuSec{ 0.f }
{
}

void UIManager::Update(float elapsedSec, int cupheadHp, int currentGameStateIdx)
{
	UpdateHealth(elapsedSec, cupheadHp);
	UpdateCards();

	// Pause
	if (currentGameStateIdx == 2)
	{
		m_Paused = true;
	}
	else m_Paused = false;

	// Intro
	if (currentGameStateIdx == 0)
	{
		m_ScreenAlpha = 0.f;
		m_PlayingIntro = true;

		m_CurrentFrameWidth = m_IntroAnnouncementTexture->GetWidth() / 7;
		m_CurrentFrameHeight = m_IntroAnnouncementTexture->GetHeight() / 7;

		m_Animator.PlayAnimation(elapsedSec, 0.06f);
	} 
	else m_PlayingIntro = false;

	// Gameplay
	if (currentGameStateIdx == 1)
	{
		if (m_Animator.GetCurrentFrameNr() != 0)
		{
			this->m_Animator.Reset(0);
		}
	}

	// Death
	if (currentGameStateIdx == 3)
	{
		m_CurrentFrameWidth = m_DeathAnnouncementTexture->GetWidth() / 3;
		m_CurrentFrameHeight = m_DeathAnnouncementTexture->GetHeight() / 7;
		HandleDeathUI(elapsedSec);
	}

	// Win
	if (currentGameStateIdx == 4)
	{
		m_PlayingWinAnnounement = true;

		m_CurrentFrameWidth = m_CompletionAnnouncementTexture->GetWidth() / 3;
		m_CurrentFrameHeight = m_CompletionAnnouncementTexture->GetHeight() / 9;

		m_Animator.PlayAnimation(elapsedSec, 0.06f);

		if (m_Animator.GetCurrentFrameNr() >= 26)
		{
			m_Animator.Stop();
		}

		if (m_ScreenAlpha < 1.f)
		{
			m_StateAccuSec += elapsedSec;

			if (m_StateAccuSec >= 0.1f)
			{
				m_ScreenAlpha += 0.02f;
				m_StateAccuSec -= 0.1f;
			}
		}
	}
	else m_PlayingWinAnnounement = false;
}

void UIManager::Draw(const Rectf& cameraBox) const
{
	DrawHealthBar(cameraBox);
	DrawCards(cameraBox);

	if (m_PlayingIntro)
	{
		const Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 7) * m_CurrentFrameWidth, 
			0.f + (m_Animator.GetCurrentFrameNr() / 7) * m_CurrentFrameHeight, m_CurrentFrameWidth, m_CurrentFrameHeight };

		m_IntroAnnouncementTexture->Draw(cameraBox, srcRect);
	}

	if (m_Paused)
	{
		utils::SetColor(Color4f{ 0, 0, 0, 0.5f });
		utils::FillRect(cameraBox);
		m_PauseScreenTexture->Draw(Rectf{cameraBox.left + cameraBox.width * 0.25f, cameraBox.bottom + cameraBox.height * 0.25f, cameraBox.width * 0.5f, cameraBox.height * 0.5f});
	}

	if (m_PlayingDeathAnnouncement)
	{
		const Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % 3) * m_CurrentFrameWidth, 
			0.f + (m_Animator.GetCurrentFrameNr() / 3) * m_CurrentFrameHeight, m_CurrentFrameWidth, m_CurrentFrameHeight };

		m_DeathAnnouncementTexture->Draw(Rectf{cameraBox.left + cameraBox.width * 0.05f, cameraBox.bottom + cameraBox.height * 0.15f, cameraBox.width * 0.9f, cameraBox.height * 0.7f}, srcRect);
	}

	if (m_DrawingDeathScreen)
	{
		utils::SetColor(Color4f{ 0, 0, 0, 0.5f });
		utils::FillRect(cameraBox);
		m_DeathScreenCardTexture->Draw(Rectf{ cameraBox.left + cameraBox.width * 0.3f, cameraBox.bottom + cameraBox.height * 0.1f, cameraBox.width * 0.4f, cameraBox.width * 0.45f });

		// draw progress man
		const float playerLocationX{ cameraBox.left + cameraBox.width / 2 };
		const Rectf progressDest{ cameraBox.left + cameraBox.width * 0.3f + (playerLocationX / 13500) * cameraBox.width * 0.4f, 
			cameraBox.bottom + cameraBox.height * 0.4f + (playerLocationX / 13500) * sinf(7.f * utils::g_Pi / 180.f) * cameraBox.height * 0.4f,
		m_ProgressManTexture->GetWidth() * 0.6f, m_ProgressManTexture->GetHeight() * 0.6f };

		m_ProgressManTexture->Draw(progressDest);
	}

	if (m_PlayingWinAnnounement)
	{
		const Rectf srcRect{ 5.f + (m_Animator.GetCurrentFrameNr() % 3) * m_CurrentFrameWidth,
			0.f + (m_Animator.GetCurrentFrameNr() / 3) * m_CurrentFrameHeight, m_CurrentFrameWidth - 10.f, m_CurrentFrameHeight };
		
		m_CompletionAnnouncementTexture->Draw(cameraBox, srcRect);

		utils::SetColor(Color4f{ 0, 0, 0, m_ScreenAlpha });
		utils::FillRect(cameraBox);
	}
}

void UIManager::ResetUI()
{
	m_CardIdx = 0;
	m_Animator.Reset(0);
	m_DrawingDeathScreen = false;
	m_PlayingDeathAnnouncement = false;
	m_Paused = false;
	m_PlayingWinAnnounement = false;
	m_StateAccuSec = 0.f;
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
	m_PlayingDeathAnnouncement = true;

	if (m_PlayingDeathAnnouncement)
	{
		m_StateAccuSec += elapsedSec;

		if (m_StateAccuSec >= 1.5f)
		{
			m_PlayingDeathAnnouncement = false;
			m_DrawingDeathScreen = true;
			m_Animator.Reset(0);
		}
		else m_Animator.PlayAnimation(elapsedSec, 0.1f);
	}
}
