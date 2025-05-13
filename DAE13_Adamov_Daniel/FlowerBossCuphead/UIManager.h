#pragma once
class Texture;
#include "Animator.h"

class UIManager final
{
public:
	explicit UIManager(const Texture* healthDisplay, const Texture* cards, const Texture* intro, 
		const Texture* death, const Texture* completion, const Texture* pause);

	void Update(float elapsedSec, int cupheadHp, int currentGameStateIdx);
	void Draw(const Rectf& cameraBox) const;
	
	void ChangeCards(int index = 1);
	bool SpecialAttackAllowed() const;

private:
	const Texture* m_HealthTexture;
	const Texture* m_CardsTexture;
	const Texture* m_IntroTexture;
	const Texture* m_DeathTexture;
	const Texture* m_CompletionTexture;
	const Texture* m_PauseScreenTexture;

	int m_CurrentFrameHp;
	int m_CardIdx;

	bool m_PlayingIntro;
	bool m_Paused;
	bool m_PlayingDeathAnnouncment;
	bool m_DrawingDeathScreen;

	float m_StateAccuSec;
	Animator m_Animator;

	void UpdateCards();
	void UpdateHealth(float elapsedSec, int cupheadHp);

	void DrawCards(const Rectf& cameraBox) const;
	void DrawHealthBar(const Rectf& cameraBox) const;
	void HandleDeathUI(float elapsedSec);
};

