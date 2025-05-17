#include "pch.h"
#include "Effect.h"
#include "Texture.h"

Effect::Effect(const Vector2f& pos, const Texture* texture, int colNr, int rowNr, float scaleIndex)
	:m_Position{ pos },
	m_Sprite{ texture },
	m_ColNr{ colNr },
	m_RowNr{ rowNr },
	m_FrameWidth{ m_Sprite->GetWidth() / m_ColNr },
	m_FrameHeight{ m_Sprite->GetHeight() / m_RowNr },
	m_Scale{ scaleIndex },
	m_LifetimeDuration{ (colNr * rowNr - 1) * 0.07f },
	m_LifeTimeAccuSec{ 0.f },
	m_Animator{},
	m_EndMarker{ false }
{
}

void Effect::Update(float elapsedDSec)
{
	m_LifeTimeAccuSec += elapsedDSec;

	if (m_LifeTimeAccuSec >= m_LifetimeDuration)
	{
		m_EndMarker = true;
	}

	m_Animator.PlayAnimation(elapsedDSec, 0.07f);
}

void Effect::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_ColNr) * m_FrameWidth,
		0.f + (m_Animator.GetCurrentFrameNr() / m_ColNr) * m_FrameHeight, m_FrameWidth, m_FrameHeight };

	const float worldWidth{ m_FrameWidth * m_Scale };
	const float worldHeight{ m_FrameHeight * m_Scale };
	m_Sprite->Draw(Rectf{m_Position.x - worldWidth / 4, m_Position.y - worldHeight / 4, worldWidth, worldHeight}, srcRect);
}

bool Effect::MarkedForDeletion() const
{
	return m_EndMarker;
}
