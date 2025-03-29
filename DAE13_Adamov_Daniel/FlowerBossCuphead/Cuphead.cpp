#include "pch.h"
#include "Cuphead.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"

Cuphead::Cuphead(const Vector2f& position)
	: m_Position{ position },
	m_Speed{ 100.f },
	m_HP{ 3 },
	m_PlayingIntro{ true },
	m_CupheadMovementState{ Movement::idle },
	m_CupheadShootingState{ Shoot::idle },
	m_IsHit{ false },
	m_AccuSec{ 0.f },
	m_MaxFrameSec{ 0.07f },
	m_CurrentFrame{ 0 },
	m_Animator{ Animator(m_CurrentFrame) }
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
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::runRight:
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::jump:
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::dash:
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::duck:
			currentTexture = m_TextureIdle;
			break;
		case Cuphead::Movement::parry:
			currentTexture = m_TextureIdle;
			break;
		}
	}

	// we call this regardless of the state
	if (currentTexture != nullptr)
	{
		// regardless of the state, it always gets the right size
		const float frameWidth{ currentTexture->GetWidth() / textureColumns };
		const float frameHeight{ currentTexture->GetHeight() / textureRows };
		// we get GetBounds() by frame sizes which results in a little teleportation
		currentTexture->Draw(GetBounds(frameWidth, frameHeight),
			Rectf{ 0 + (m_CurrentFrame % textureColumns) * frameWidth,
			0 + (m_CurrentFrame / textureColumns) * frameHeight,
			frameWidth, frameHeight });

		// hitbox
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawRect(GetBounds(frameWidth, frameHeight));
	}

	
	
}

void Cuphead::Update(float elapsedSec, const Uint8* pStates)
{
	if (m_PlayingIntro)
	{
		// we need this here
		PlayIntro(elapsedSec);
	}
	else
	{
		// first we check if it's idle
		if (m_CupheadMovementState == Movement::idle)
		{
			m_MaxFrameSec = 0.1f;
			m_AccuSec += elapsedSec;
			if (m_AccuSec > m_MaxFrameSec)
			{
				m_CurrentFrame++;
				m_Animator.ResetAtFrame(5);
				m_AccuSec -= m_MaxFrameSec;
			}
		}

		// secondly, we change the movement and shooting state
		else
		{
			// change movement and shooting state 
		}
	}
}

void Cuphead::PlayIntro(float elapsedSec)
{

	static float m_RewindTime{};

	m_AccuSec += elapsedSec;
	if (m_CurrentFrame > 11 && m_CurrentFrame < 20 && m_RewindTime < 3.f) // Animator.LoopBetween()
	{
		if (m_AccuSec > m_MaxFrameSec)
		{
			m_CurrentFrame++;
			m_RewindTime += m_AccuSec;
			m_AccuSec -= m_MaxFrameSec;
		}
		if (m_CurrentFrame == 20)
		{
			m_CurrentFrame = 11;
		}
	}
	else if (m_CurrentFrame <= 11 || m_RewindTime >= 3.f)
	{
		if (m_AccuSec > m_MaxFrameSec)
		{
			m_CurrentFrame++;
			m_AccuSec -= m_MaxFrameSec;
			if (m_CurrentFrame > 28)
			{
				m_AccuSec = 0.f;
				m_CurrentFrame = 0;
				m_PlayingIntro = false;
			}
		}
	}	
}

Rectf Cuphead::GetBounds(float frameWidth, float frameHeight) const
{
	return Rectf{ m_Position.x, m_Position.y, frameWidth, frameHeight };
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
