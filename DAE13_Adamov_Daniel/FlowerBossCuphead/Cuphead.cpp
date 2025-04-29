#include "pch.h"
#include "Cuphead.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"
#include "Matrix2x3.h"
#include "BulletManager.h"

float Cuphead::m_FrameWidth{ 0.f }; // static so we can adapt them in Draw()
float Cuphead::m_FrameHeight{ 0.f };

Cuphead::Cuphead(const Vector2f& position, bool playIntro)
	: m_Position{ position },
	m_HP{ 3 },
	m_PlayingIntro{ playIntro },
	m_CupheadMovementState{ Movement::idle },
	m_CupheadShootingState{ Shoot::notShooting },
	m_KeyPressed{ false },
	m_AccuSecProjectiles{ 0.f },
	m_ShootAngle{ 0.f },
	m_IsGrounded{ false },
	m_IsHit{ false },
	m_DashAccuSec{ 0.f },
	m_ClickedDash{ false },
	m_IsDashing{ false },
	m_DashCooldown{ 0.7f },
	m_DashDuration{ 0.3f },
	m_MaxFrameSec{ 0.07f },
	m_CurrentTexture{ nullptr },
	m_CurrentColNr{ 1 },
	m_CurrentRowNr{ 1 },
	m_Velocity{ 0.f, 0.f },
	m_FacingAngle{ 0.f },
	m_TexturePeaShooter{ new Texture("Projectile_Loop.png") },
	m_TextureSpecialPeaShooter{ new Texture("Projectile_Special_Loop.png") },
	m_Animator{}
{
	IntializeTextures();
}

//Cuphead::Cuphead(const Cuphead& character)
//	:m_Position{ character.m_Position },
//	m_HP{ character.m_HP },
//	m_PlayingIntro{ character.m_PlayingIntro },
//	m_CupheadMovementState{ character.m_CupheadMovementState },
//	m_CupheadShootingState{ character.m_CupheadShootingState },
//	m_KeyPressed{ character.m_KeyPressed },
//	m_AccuSecProjectiles{ character.m_AccuSecProjectiles },
//	m_ShootAngle{ character.m_ShootAngle },
//	m_IsGrounded{ character.m_IsGrounded },
//	m_IsHit{ character.m_IsHit },
//	m_DashAccuSec{ character.m_DashAccuSec },
//	m_ClickedDash{ character.m_ClickedDash },
//	m_IsDashing{ character.m_IsDashing },
//	m_DashCooldown{ character.m_DashCooldown },
//	m_DashDuration{ character.m_DashDuration },
//	m_MaxFrameSec{ character.m_MaxFrameSec },
//	m_CurrentTexture{ character.m_CurrentTexture }, //!
//	m_CurrentColNr{ character.m_CurrentColNr },
//	m_CurrentRowNr{ character.m_CurrentRowNr },
//	m_Velocity{ character.m_Velocity },
//	m_FacingAngle{ character.m_FacingAngle },
//	m_Animator{ character.m_Animator },
//{
//	IntializeTextures();
//}

Cuphead::~Cuphead()
{
	DeleteTextures();
	delete m_TexturePeaShooter;
	m_TexturePeaShooter = nullptr;

	delete m_TextureSpecialPeaShooter;
	m_TextureSpecialPeaShooter = nullptr;
}

void Cuphead::Draw() const
{
	if (m_CurrentTexture != nullptr)
	{
		// regardless of the state, it always gets the right size
		m_FrameWidth = m_CurrentTexture->GetWidth() / m_CurrentColNr;
		m_FrameHeight = m_CurrentTexture->GetHeight() / m_CurrentRowNr;
		Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrame() % m_CurrentColNr) * m_FrameWidth, 0.f + (m_Animator.GetCurrentFrame() / m_CurrentColNr) * m_FrameHeight,
			m_FrameWidth, m_FrameHeight };
		
		// GetBounds is calculated by the mid point of each texture
		glPushMatrix();
			glTranslatef(m_Position.x, m_Position.y, 0); // This reduces the texture quality A LOT for some reason !!!
			glRotatef(m_FacingAngle, 0, 1, 0); 
			m_CurrentTexture->Draw(Vector2f{ -GetBounds().width / 2, 0.f}, srcRect);

		// hitbox
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		//utils::DrawRect(Vector2f{ -GetBounds().width / 2, 0.f }, m_FrameWidth, m_FrameHeight);
		glPopMatrix();

		// Hitbox
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		//utils::DrawRect(GetBounds());
		//utils::FillEllipse(m_Position, 5.f, 5.f);
	}
	
}

void Cuphead::Update(float elapsedSec, const Uint8* pStates, const std::vector<Vector2f>& vertices, BulletManager& bulletManager)
{
	// sets the dashing state from a key event and then processes it in ProcessKeys()...
	Dash(elapsedSec);

	// determines which key is pressed where + combinations and sets the velocity
	ProcessKeys(pStates);

	// determines the current Texture which is going to be used in Draw() + updates the current framer number
	AnimateCuphead(elapsedSec);

	// updates the postion using m_Velocity and enables collisions from a std::vector<Vector2f>
	HandleRaycast(elapsedSec, vertices);

	// pushes a new projectile to BulletManager
	CreateProjectiles(elapsedSec, bulletManager);
}

void Cuphead::ProcessKeys(const Uint8* pStates)
{
	if (!m_PlayingIntro)
	{
		UpdateFacingDirection(pStates);

		m_KeyPressed = true;

		if (m_IsDashing) 
		{

			if (!m_IsGrounded)
			{
				m_Velocity.y = 0.f;

				if (m_FacingAngle == 0.f)
				{
					m_Velocity.x = 600.f;
				}
				else if (m_FacingAngle == 180.f)
				{
					m_Velocity.x = -600.f;
				}
			}
			else
			{
				if (m_FacingAngle == 0.f)
				{
					m_Velocity.x = 800.f;
				}
				else if (m_FacingAngle == 180.f)
				{
					m_Velocity.x = -800.f;
				}
			}
		}
		// determines the whole rotation logic + enables dashing in a direction
		else
		{
			// change movement and shooting state, statring with COMBINATIONS
			const float movementSpeed{ 400.f };

			if (pStates[SDL_SCANCODE_Z])
			{
				if (m_CupheadMovementState != Movement::parry)
				{
					m_CupheadMovementState = Movement::jump;
					m_CupheadShootingState = Shoot::notShooting; // 3

					if (pStates[SDL_SCANCODE_LEFT])
					{
						m_Velocity.x = -movementSpeed;
					}
					if (pStates[SDL_SCANCODE_RIGHT])
					{
						m_Velocity.x = movementSpeed;
					}

					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 0.f, 700.f };
					}
				}
			}
			// shoot Up left
			else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_X] && pStates[SDL_SCANCODE_C])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootDiagonalUpLeft; // 7

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 135.f;
			}
			// shoot Up right
			else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_X] && pStates[SDL_SCANCODE_C])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootDiagonalUpRight; // 6

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 45.f;
			}
			// shoot left
			else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_X] && pStates[SDL_SCANCODE_C])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootLeft; // 9

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 180.f;
			}
			// shoot right
			else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_X] && pStates[SDL_SCANCODE_C])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootRight; // 8

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 0.f;
			}
			// run shoot up right
			else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_X]) // ?? might change
			{
				m_CupheadMovementState = Movement::runRight;
				m_CupheadShootingState = Shoot::shootDiagonalUpRight; // 14

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ movementSpeed, 0.f };
				}
				m_ShootAngle = 45.f;
			}
			// run shoot up left
			else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_X]) // ?? might change
			{
				m_CupheadMovementState = Movement::runLeft;
				m_CupheadShootingState = Shoot::shootDiagonalUpLeft; // 15

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ -movementSpeed, 0.f };
				}
				m_ShootAngle = 135.f;
			}
			// run shoot left
			else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_X])
			{
				m_CupheadMovementState = Movement::runLeft;
				m_CupheadShootingState = Shoot::shootLeft; // 17

				m_Velocity.x = -movementSpeed;

				// Only reset vertical velocity when grounded
				if (m_IsGrounded)
				{
					m_Velocity.y = 0.f;
				}

				m_ShootAngle = 180.f;
			}
			// run shoot right
			else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_X])
			{
				m_CupheadMovementState = Movement::runRight;
				m_CupheadShootingState = Shoot::shootRight; // 16

				m_Velocity.x = movementSpeed;

				// Only reset vertical velocity when grounded
				if (m_IsGrounded)
				{
					m_Velocity.y = 0.f;
				}

				m_ShootAngle = 0.f;
			}
			// shoot up
			else if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_X])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootUp; // 5

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 90.f;
			}
			// shoot down
			else if (pStates[SDL_SCANCODE_DOWN] && pStates[SDL_SCANCODE_X])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootDown; // 12

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
				m_ShootAngle = 270.f;
			}
			// then proceed with individual keys
			else if (pStates[SDL_SCANCODE_LEFT])
			{
				m_CupheadMovementState = Movement::runLeft;
				m_CupheadShootingState = Shoot::notShooting; // 1

				m_Velocity.x = -movementSpeed;

				// Only reset vertical velocity when grounded
				if (m_IsGrounded)
				{
					m_Velocity.y = 0.f;
				}
			}
			else if (pStates[SDL_SCANCODE_RIGHT])
			{
				m_CupheadMovementState = Movement::runRight;
				m_CupheadShootingState = Shoot::notShooting; // 2

				m_Velocity.x = movementSpeed;

				// Only reset vertical velocity when grounded
				if (m_IsGrounded)
				{
					m_Velocity.y = 0.f;
				}
			}
			else if (pStates[SDL_SCANCODE_DOWN])
			{
				m_CupheadMovementState = Movement::duck;
				m_CupheadShootingState = Shoot::notShooting; // 13

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
			}
			else if (pStates[SDL_SCANCODE_X])
			{
				m_CupheadMovementState = Movement::lock;
				m_CupheadShootingState = Shoot::shootRight;

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}

				// important DEFAULT direction!
				if (m_FacingAngle == 0.f)
				{
					m_ShootAngle = 0.f;
				}
				else if (m_FacingAngle == 180.f)
				{
					m_ShootAngle = 180.f;
				}
			}

			// --- I can add whatever key combinations I want from now on, AnimateCuphead will handle it! ---

			// if no key combinations are pressed -> it's idle
			else
			{
				m_KeyPressed = false;
				m_CupheadMovementState = Movement::idle;
				m_CupheadShootingState = Shoot::notShooting;

				if (m_IsGrounded)
				{
					m_Velocity = Vector2f{ 0.f, 0.f };
				}
			}
		}
	}
}

void Cuphead::AnimateCuphead(float elapsedSec)
{
	if (m_PlayingIntro)
	{
		m_CurrentTexture = m_TextureIntro;
		m_CurrentColNr = 7;
		m_CurrentRowNr = 4;

		m_Animator.LoopBetween(elapsedSec, 12, 19, m_MaxFrameSec, 2.5f);
		
		if (m_Animator.GetCurrentFrame() > 27)
		{
			m_Animator.Reset(0);
			m_PlayingIntro = false;
		}
	}
	else
	{
		// important for resetting animations between each other
		static Movement lastMovementState{};
		static Shoot lastShootState{};

		bool stateChanged{ (m_CupheadMovementState != lastMovementState) ||
			(m_CupheadShootingState != lastShootState) }; // very important!!! --- we use this boolean to reset all animations - we do this through Reset() in Animate.cpp

		
		if (m_CupheadMovementState == Movement::idle)
		{
			// default reseting between animations
			if (stateChanged)
			{
				m_Animator.Reset(0);
				lastMovementState = m_CupheadMovementState;
				lastShootState = m_CupheadShootingState;
			}

			if (!m_IsGrounded)
			{
				m_CurrentTexture = m_TextureJump;
				m_CurrentColNr = 4;
				m_CurrentRowNr = 2;
				m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 8);
			}

			else
			{
				m_CurrentTexture = m_TextureIdle;
				m_CurrentColNr = 5;
				m_CurrentRowNr = 1;

				m_Animator.ReverseAnimateBetween(elapsedSec, 0, 5, m_MaxFrameSec);
			}
			
		}
		else if (m_CupheadShootingState == Shoot::notShooting)
		{
			// default reseting between animations
			if (stateChanged)
			{
				m_Animator.Reset(0);
				lastMovementState = m_CupheadMovementState;
				lastShootState = m_CupheadShootingState;
			}

			if (!m_IsGrounded && m_CupheadMovementState != Movement::jump && m_CupheadMovementState != Movement::dash && m_CupheadMovementState != Movement::parry)
			{
				m_CurrentTexture = m_TextureJump;
				m_CurrentColNr = 4;
				m_CurrentRowNr = 2;
				m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 8);
			}
			else
			{
				switch (m_CupheadMovementState)
				{
				case Cuphead::Movement::runLeft:
					m_CurrentTexture = m_TextureRun;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 16);
					break;
				case Cuphead::Movement::runRight:
					m_CurrentTexture = m_TextureRun;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 16);
					break;
				case Cuphead::Movement::jump:
					m_CurrentTexture = m_TextureJump;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 2;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 8);
					break;
				case Cuphead::Movement::parry:
					m_CurrentTexture = m_TextureParry;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.LoopBetween(elapsedSec, 9, 16, 0.06f);
					break;
				case Cuphead::Movement::dash:
					m_CurrentTexture = m_TextureDash;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 2;

					m_Animator.LoopBetween(elapsedSec, 2, 6, m_MaxFrameSec);
					break;
				case Cuphead::Movement::duck:
					m_CurrentTexture = m_TextureDuck;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.LoopBetween(elapsedSec, 7, 14, m_MaxFrameSec);
					break;
				}
			}
			
		}
		else if (m_CupheadShootingState != Shoot::notShooting)
		{
			if (!m_IsGrounded)
			{
				m_CurrentTexture = m_TextureJump;
				m_CurrentColNr = 4;
				m_CurrentRowNr = 2;
				m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec, 8);
			}
			else if (m_CupheadMovementState == Movement::lock)
			{
				static int currentStartIdx{}; // static so it can track the starting index real time depending on the animation

				m_CurrentTexture = m_TextureShoot;
				m_CurrentColNr = 5;
				m_CurrentRowNr = 5;

				switch (m_CupheadShootingState)
				{
				case Cuphead::Shoot::shootUp:
					currentStartIdx = 0;
					m_Animator.ReverseAnimateBetween(elapsedSec, 0, 4, 0.08f);
					break;
				case Cuphead::Shoot::shootDiagonalUpRight:
					currentStartIdx = 5;
					m_Animator.ReverseAnimateBetween(elapsedSec, 5, 9, 0.08f);
					break;
				case Cuphead::Shoot::shootRight:
					currentStartIdx = 10;
					m_Animator.ReverseAnimateBetween(elapsedSec, 10, 14, 0.08f);
					break;
				case Cuphead::Shoot::shootDiagonalUpLeft:
					currentStartIdx = 5;
					m_Animator.ReverseAnimateBetween(elapsedSec, 5, 9, 0.08f);
					break;
				case Cuphead::Shoot::shootLeft:
					currentStartIdx = 10;
					m_Animator.ReverseAnimateBetween(elapsedSec, 10, 14, 0.08f);
					break;
				case Cuphead::Shoot::shootDown:
					currentStartIdx = 20;
					m_Animator.ReverseAnimateBetween(elapsedSec, 20, 24, 0.08f);
					break;
				}

				// reseting the animations to a unique index each time depending on the Shoot State
				if (stateChanged)
				{
					m_Animator.Reset(currentStartIdx);
					lastMovementState = m_CupheadMovementState;
					lastShootState = m_CupheadShootingState;
				}
			}
			else if (m_CupheadMovementState == Movement::runRight)
			{
				// default reseting
				if (stateChanged)
				{
					m_Animator.Reset(0);
					lastMovementState = m_CupheadMovementState;
					lastShootState = m_CupheadShootingState;
				}

				if (m_CupheadShootingState == Shoot::shootDiagonalUpRight)
				{
					m_CurrentTexture = m_TextureRunShootDiagonal;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;
					
					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec);
				}
				else if (m_CupheadShootingState == Shoot::shootRight)
				{
					m_CurrentTexture = m_TextureRunShootStraight;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec);
				}
			}
			else if (m_CupheadMovementState == Movement::runLeft)
			{
				// default reseting
				if (stateChanged)
				{
					m_Animator.Reset(0);
					lastMovementState = m_CupheadMovementState;
					lastShootState = m_CupheadShootingState;
				}

				if (m_CupheadShootingState == Shoot::shootDiagonalUpLeft)
				{
					m_CurrentTexture = m_TextureRunShootDiagonal;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec);
				}
				else if (m_CupheadShootingState == Shoot::shootLeft)
				{
					m_CurrentTexture = m_TextureRunShootStraight;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec);
				}
			}
		}
	}
}

void Cuphead::HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices)
{
	Vector2f firstPointY{ m_Position.x, m_Position.y + GetBounds().height };
	Vector2f secondPointY{ m_Position.x, m_Position.y - 1.f };

	utils::HitInfo hitInfo;
	// GROUND collision
	if (utils::Raycast(vertices, firstPointY, secondPointY, hitInfo))
	{
		if (!m_KeyPressed)
		{
			m_Velocity = Vector2f{ 0.f, 0.f };
		}

		m_Position.y = hitInfo.intersectPoint.y;
		m_IsGrounded = true;
	}
	else
	{
		m_IsGrounded = false;
		m_Velocity += Vector2f{ 0, -9.8f };
	}

	Vector2f firstPointX{ GetBounds().left, m_Position.y + GetBounds().height / 2 };
	Vector2f secondPointX{ GetBounds().left  + GetBounds().width, m_Position.y + GetBounds().height / 2 };

	// RIGHT collision
	if (utils::Raycast(vertices, firstPointX, secondPointX, hitInfo))
	{

		if (!m_KeyPressed)
		{
			m_Velocity.x = 0.f;
		}
		//m_Position.x = hitInfo.intersectPoint.x;
		m_Position.x = hitInfo.intersectPoint.x - (GetBounds().width / 2 * hitInfo.normal.x);
	}

	// LEFT collision
	if (utils::Raycast(vertices, secondPointX, firstPointX, hitInfo))
	{
		if (!m_KeyPressed)
		{
			m_Velocity.x = 0.f;
		}
		//m_Position.x = hitInfo.intersectPoint.x + GetBounds().width / 2;
		m_Position.x = hitInfo.intersectPoint.x - (GetBounds().width / 2 * hitInfo.normal.x);
	}

	m_Position += m_Velocity * elapsedSec;
}

void Cuphead::Dash(float elapsedSec)
{
	if (m_ClickedDash && m_DashAccuSec == 0.f)
	{
		m_IsDashing = true;
		m_ClickedDash = false;
	}

	if (m_IsDashing)
	{
		m_DashAccuSec += elapsedSec;

		if (m_DashAccuSec > m_DashDuration)
		{
			m_IsDashing = false;
		}
	}

	if (m_DashAccuSec >= m_DashDuration && m_IsGrounded)
	{
		m_DashAccuSec = 0.f;
	}
	
}

void Cuphead::CreateProjectiles(float elapsedSec, BulletManager& bulletManager)
{
	const float projectileCooldown{ 0.2f };

	if (m_CupheadShootingState != Shoot::notShooting) // if shooting in any direction
	{
		m_AccuSecProjectiles += elapsedSec;

		if (m_AccuSecProjectiles > projectileCooldown)
		{
			bulletManager.AddProjectile(new Projectile(m_TexturePeaShooter, 
				Vector2f{ GetBounds().left + GetBounds().width / 2, GetBounds().bottom + GetBounds().height * 0.4f }, m_ShootAngle));
			m_AccuSecProjectiles -= projectileCooldown;
		}
	}
}

void Cuphead::StartDash()
{
	if (!m_PlayingIntro)
	{
		m_ClickedDash = true;
		m_CupheadMovementState = Movement::dash;
		m_CupheadShootingState = Shoot::notShooting;
	}
}

void Cuphead::Parry()
{
	if (!m_PlayingIntro && !m_IsGrounded)
	{
		m_CupheadMovementState = Movement::parry;
		m_CupheadShootingState = Shoot::notShooting;
	}
}

int Cuphead::GetHealth() const // to check if isDead in Game.cpp in future
{
	return m_HP;
}

Vector2f Cuphead::GetPosition() const
{
	return m_Position;
}

bool Cuphead::IsShooting() const
{
	if (m_CupheadShootingState != Shoot::notShooting)
	{
		return true;
	}
	return false;
}

Rectf Cuphead::GetBounds() const
{
	return Rectf{ m_Position.x - m_FrameWidth / 2, m_Position.y, m_FrameWidth, m_FrameHeight };
}

void Cuphead::UpdateFacingDirection(const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_FacingAngle = 0.f ;
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		m_FacingAngle = 180.f;
	}
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
}
