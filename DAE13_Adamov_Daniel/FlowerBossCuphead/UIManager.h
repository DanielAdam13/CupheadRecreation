#pragma once
class Texture;

class UIManager final
{
public:
	explicit UIManager(const Texture* healthDisplay, const Texture* cards);

	void Update(float elapsedSec, int cupheadHp);
	void Draw(const Rectf& cameraBox) const;
	
	void ChangeCards(float index = 1.f);
	bool SpecialAttackAllowed() const;

private:
	const Texture* m_TextureHealth;
	const Texture* m_TextureCards;

	int m_CurrentFrameHp;
	int m_CardIdx;
};

