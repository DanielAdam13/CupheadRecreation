#pragma once
class Texture;
#include "Animator.h"

class UIManager final
{
public:
	explicit UIManager(const Texture* healthDisplay, const Texture* cards, const Texture* intro, 
		const Texture* death, const Texture* completion, const Texture* pause, const Texture* deathScreen, const Texture* progressMan);

	void Update(float elapsedSec, int cupheadHp, int currentGameStateIdx);
	void Draw(const Rectf& cameraBox) const;

	void ResetUI();
	
	void ChangeCards(int index = 1);
	bool SpecialAttackAllowed() const;

private:
	const Texture* m_HealthTexture;
	const Texture* m_CardsTexture;
	const Texture* m_IntroAnnouncementTexture;
	const Texture* m_DeathAnnouncementTexture;
	const Texture* m_CompletionAnnouncementTexture;
	const Texture* m_PauseScreenTexture;
	const Texture* m_DeathScreenCardTexture;
	const Texture* m_ProgressManTexture;

	float m_CurrentFrameWidth;
	float m_CurrentFrameHeight;

	int m_CurrentFrameHp;
	int m_CardIdx;

	bool m_PlayingIntro;
	bool m_Paused;
	bool m_PlayingDeathAnnouncement;
	bool m_DrawingDeathScreen;
	bool m_PlayingWinAnnounement;

	float m_ScreenAlpha;

	float m_StateAccuSec;
	Animator m_Animator;

	void UpdateCards();
	void UpdateHealth(float elapsedSec, int cupheadHp);

	void DrawCards(const Rectf& cameraBox) const;
	void DrawHealthBar(const Rectf& cameraBox) const;
	void HandleDeathUI(float elapsedSec);
};

