#include "pch.h"
#include "BigChomper.h"
#include "Texture.h"
#include "utils.h"


BigChomper::BigChomper(Texture* spriteTexture, const Vector2f& pos, const Vector2f& lowestPoint, const Vector2f& highestPoint, float speed)
	:Spike(spriteTexture, pos, lowestPoint, highestPoint, speed),
	m_SpriteRowNr{ 4 },
	m_SpriteColNr{ 5 }
{
}

void BigChomper::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrame() % m_SpriteColNr) * this->GetBounds().width,
	0.f + (m_Animator.GetCurrentFrame() / m_SpriteColNr) * this->GetBounds().height, this->GetBounds().width, this->GetBounds().height };

	this->m_Texture->Draw(Vector2f{ this->GetBounds().left, this->GetBounds().bottom }, srcRect);

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(this->GetBounds());
	utils::FillEllipse(this->m_Position, 5.f, 5.f);

	utils::SetColor(Color4f{ 0, 0, 1, 1 });
	utils::FillEllipse(m_LowestPoint, 5.f, 5.f);
	utils::FillEllipse(m_HighestPoint, 5.f, 5.f);
}

void BigChomper::Update(float elapsedSec)
{
	Spike::Bounce(elapsedSec);

	if (m_Speed > 0.f)
	{
		if (m_Position.y <= m_HighestPoint.y - 300.f)
		{
			m_Animator.LoopBetween(elapsedSec, 0, 8, 0.07f);
		}
		else
		{
			m_Animator.LoopBetween(elapsedSec, 9, 11, 0.07f);
		}
	}
	else
	{
		if (m_Position.y >= m_HighestPoint.y - 200.f)
		{
			m_Animator.LoopBetween(elapsedSec, 12, 15, 0.08f);
		}
		else
		{
			m_Animator.LoopBetween(elapsedSec, 16, 19, 0.09f);
		}
	}
	
}

Rectf BigChomper::GetBounds() const
{
	//const Texture* chomperTexture{ GetChomperSprite()};
	const float frameWidth{ this->m_Texture->GetWidth() / m_SpriteColNr };
	const float frameHeight{ this->m_Texture->GetHeight() / m_SpriteRowNr };

	return Rectf(this->m_Position.x - frameWidth / 2, this->m_Position.y - frameHeight / 2, frameWidth, frameHeight);
}