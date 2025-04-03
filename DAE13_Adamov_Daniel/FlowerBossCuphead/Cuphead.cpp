#include "pch.h"
#include "Cuphead.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"

float Cuphead::m_FrameWidth{ 0.f };
float Cuphead::m_FrameHeight{ 0.f };

Cuphead::Cuphead(const Vector2f& position)
	: m_Position{ position },
	m_Speed{ 100.f },
	m_HP{ 3 },
	m_PlayingIntro{ false },
	m_CupheadMovementState{ Movement::idle },
	m_CupheadShootingState{},
	m_IsHit{ false },
	m_AccuSec{ 0.f },
	m_MaxFrameSec{ 0.07f },
	m_CurrentFrame{ 0 },
	m_Velocity{ 0.f, 0.f },
	m_Animator{ m_CurrentFrame, m_AccuSec }
{
	IntializeTextures();
}

Cuphead::~Cuphead()
{
	DeleteTextures();
}

void Cuphead::Draw() const
{
	Texture* currentTexture{};
	int textureColumns{ 1 };
	int textureRows{ 1 };

	if (m_PlayingIntro)
	{
		currentTexture = m_TextureIntro;
		textureColumns = 7;
		textureRows = 4;
	}
	else
	{
		switch (m_CupheadMovementState)
		{
		case Cuphead::Movement::idle:
			currentTexture = m_TextureIdle;
			textureColumns = 5;
			textureRows = 1;
			break;
		case Cuphead::Movement::runLeft:
			currentTexture = m_TextureRun;
			textureColumns = 4;
			textureRows = 4;
			break;
		case Cuphead::Movement::runRight:
			currentTexture = m_TextureRun;
			textureColumns = 4;
			textureRows = 4;
			break;
		case Cuphead::Movement::lock:
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::jump:
			currentTexture = m_TextureJump;
			textureColumns = 4;
			textureRows = 2;
			break;
		case Cuphead::Movement::dash:
			currentTexture = m_TextureDash;
			textureColumns = 4;
			textureRows = 2;
			break;
		case Cuphead::Movement::duck:
			currentTexture = m_TextureDuck;
			textureColumns = 4;
			textureRows = 4;
			break;
		case Cuphead::Movement::parry:
			currentTexture = m_TextureParry;
			break;
		}

		switch (m_CupheadShootingState)
		{
		case Cuphead::Shoot::shootUp:
			break;
		case Cuphead::Shoot::shootDiagonalUpRight:
			break;
		case Cuphead::Shoot::shootRight:
			break;
		case Cuphead::Shoot::shootDiagonalDownRight:
			break;
		case Cuphead::Shoot::shootDown:
			break;
		case Cuphead::Shoot::shootDiagonalUpLeft:
			break;
		case Cuphead::Shoot::shootLeft:
			break;
		case Cuphead::Shoot::shootDiagonalDownLeft:
			break;
		case Cuphead::Shoot::specialLeft:
			break;
		case Cuphead::Shoot::specialUp:
			break;
		case Cuphead::Shoot::specialRight:
			break;
		case Cuphead::Shoot::specialDown:
			break;
		default:
			break;
		}

		//if(...)  // for combined states (run + shoot)
	}

	// we call this regardless of the state
	if (currentTexture != nullptr)
	{
		// regardless of the state, it always gets the right size
		//const float m_FrameWidth{ currentTexture->GetWidth() / textureColumns };
		//const float m_FrameHeight{ currentTexture->GetHeight() / textureRows };
		m_FrameWidth = currentTexture->GetWidth() / textureColumns;
		m_FrameHeight = currentTexture->GetHeight() / textureRows;
		
		// we get GetBounds() by frame sizes which results in a little teleportation
		currentTexture->Draw(GetBounds(),
			Rectf{ 0 + (m_CurrentFrame % textureColumns) * m_FrameWidth,
			0 + (m_CurrentFrame / textureColumns) * m_FrameHeight,
			m_FrameWidth, m_FrameHeight });

		// hitbox
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawRect(GetBounds());
	}

	
}

void Cuphead::Update(float elapsedSec, const Uint8* pStates)
{
	if (m_PlayingIntro)
	{
		PlayIntro(elapsedSec);
	}
	else
	{
		
		// if a key is clicked -> we change state
		if (pStates)
		{
			// change movement and shooting state


			// start with combinations
			// ......

			// then proceed with individual keys
			if (pStates[SDL_SCANCODE_LEFT])
			{
				m_AccuSec += elapsedSec;
				m_CupheadMovementState = Movement::runLeft;
				m_Animator.PlayAnimation(m_MaxFrameSec);
			}
			if (pStates[SDL_SCANCODE_RIGHT])
			{
				m_AccuSec += elapsedSec;
				m_CupheadMovementState = Movement::runRight;
				m_Animator.PlayAnimation(m_MaxFrameSec);
			}
			if (pStates[SDL_SCANCODE_DOWN])
			{
				m_AccuSec += elapsedSec;
				m_CupheadMovementState = Movement::duck;
				

				m_Animator.PlayAnimation(m_MaxFrameSec);
				/*if (m_CurrentFrame > 15)
				{
					m_Animator.ResetAtFrame(15);
				}*/
				
			}
			if (pStates[SDL_SCANCODE_Z])
			{
				m_AccuSec += elapsedSec;
				m_CupheadMovementState = Movement::jump;

				m_Animator.PlayAnimation(m_MaxFrameSec);
				//if (m_CurrentFrame > 5) // play animation once
				//{
				//	m_Animator.Stop();
				//}
			}
			if (pStates[SDL_SCANCODE_LSHIFT])
			{
				m_AccuSec += elapsedSec;
				m_CupheadMovementState = Movement::dash;

				m_Animator.PlayAnimation(m_MaxFrameSec);
				/*if (m_CurrentFrame > 7)
				{
					m_Animator.Stop();
				}*/
			}
			
		}

		// otherwise it's idle
		else
		{
			m_CupheadMovementState = Movement::idle;
		m_AccuSec += elapsedSec;
		m_Animator.ReverseAnimateBetween(0, 5, 0.07f);
		}
	}
}

void Cuphead::HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices)
{
	Vector2f firstPointY{ m_Position.x, m_Position.y + GetBounds().height };
	Vector2f secondPointY{ m_Position.x, m_Position.y - 1.f };

	utils::HitInfo hitInfo;
	if (utils::Raycast(vertices, firstPointY, secondPointY, hitInfo))
	{
		m_Velocity = Vector2f{ 0.f, 0.f };
		m_Position.y = hitInfo.intersectPoint.y;
	}
	else
	{
		//m_Velocity += Vector2f{ 0, -9.8f };
	}

	if (utils::Raycast(vertices, secondPointY, firstPointY, hitInfo))
	{
		m_Velocity = Vector2f{ 0.f, 0.f };
		//m_Position.y = hitInfo.intersectPoint.y - GetBounds().height;
	}
	else
	{
		//m_Velocity += Vector2f{ 0, 9.8f };
	}

	Vector2f firstPointX{ GetBounds().left - 1.f, m_Position.y + GetBounds().height};
	Vector2f secondPointX{ GetBounds().left + GetBounds().width + 1.f, m_Position.y + GetBounds().height};

	if (utils::Raycast(vertices, firstPointX, secondPointX, hitInfo))
	{
		m_Velocity = Vector2f{ 0.f, 0.f };
		m_Position.x = hitInfo.intersectPoint.x + m_FrameWidth / 2;
	}
	else
	{
		//m_Velocity += Vector2f{ -9.8f, 0.f };
		
	}

	if (utils::Raycast(vertices, secondPointX, firstPointX, hitInfo))
	{
		m_Velocity = Vector2f{ 0.f, 0.f };
		m_Position.x = hitInfo.intersectPoint.x - m_FrameWidth / 2;
	}
	else
	{
		//m_Velocity += Vector2f{ 9.8f,0.f };
	}
	m_Position += m_Velocity * elapsedSec;
}

int Cuphead::GetHealth() const
{
	return m_HP;
}

void Cuphead::PlayIntro(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	m_Animator.LoopBetween(12, 19, 3.f, m_MaxFrameSec);
	if (m_CurrentFrame > 27)
	{
		m_AccuSec = 0.f;
		m_CurrentFrame = 0;
		m_PlayingIntro = false;
	}
}

Rectf Cuphead::GetBounds() const
{
	return Rectf{ m_Position.x - m_FrameWidth / 2, m_Position.y, m_FrameWidth, m_FrameHeight };
}

void Cuphead::IntializeTextures()
{
	m_TextureDash = new Texture("Cuphead_Sprites/Cuphead_Dash.png");
	m_TextureShoot = new Texture("Cuphead_Sprites/Cuphead_Shooting.png");
	m_TextureDeath = new Texture("Cuphead_Sprites/Death.png");
	m_TextureDuck = new Texture("Cuphead_Sprites/Duck.png");
	m_TextureHit = new Texture("Cuphead_Sprites/Hit_Ground.png");
	m_TextureIdle = new Texture("Cuphead_Sprites/Idle.png");
	m_TextureIntro = new Texture("Cuphead_Sprites/Intro.png");
	m_TextureJump = new Texture("Cuphead_Sprites/Jump.png");
	m_TextureParry = new Texture("Cuphead_Sprites/Parry_Combined.png");
	m_TextureRun = new Texture("Cuphead_Sprites/Run.png");
	m_TextureRunShootDiagonal = new Texture("Cuphead_Sprites/Run_Shoot_Diagonal.png");
	m_TextureRunShootStraight = new Texture("Cuphead_Sprites/Run_Shoot_Straight.png");
	m_TextureSpecialAttack = new Texture("Cuphead_Sprites/Special_Attack_Air.png");
}

void Cuphead::DeleteTextures()
{
	delete m_TextureDash;
	m_TextureDash = nullptr;
	delete m_TextureShoot;
	m_TextureShoot = nullptr;
	delete m_TextureDeath;
	m_TextureDeath = nullptr;
	delete m_TextureDuck;
	m_TextureDuck = nullptr;
	delete m_TextureHit;
	m_TextureHit = nullptr;
	delete m_TextureIdle;
	m_TextureIdle = nullptr;
	delete m_TextureIntro;
	m_TextureIntro = nullptr;
	delete m_TextureJump;
	m_TextureJump = nullptr;
	delete m_TextureParry;
	m_TextureParry = nullptr;
	delete m_TextureRun;
	m_TextureRun = nullptr;
	delete m_TextureRunShootDiagonal;
	m_TextureRunShootDiagonal = nullptr;
	delete m_TextureRunShootStraight;
	m_TextureRunShootStraight = nullptr;
	delete m_TextureSpecialAttack;
	m_TextureSpecialAttack = nullptr;
}
