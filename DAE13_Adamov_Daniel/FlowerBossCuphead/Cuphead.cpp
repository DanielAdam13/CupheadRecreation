#include "pch.h"
#include "Cuphead.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"
#include "BulletManager.h"
#include "PeaShooter.h"
#include "UIManager.h"
#include <cassert>
#include "SoundEffect.h"

Cuphead::Cuphead(const Vector2f& position, bool playIntro, int hp, const Texture* peaShooter, const Texture* peaSpecial)
	: m_Position{ position },
	m_HP{ hp },
	m_IsAlive{ true },
	m_FrameWidth{ 0.f },
	m_FrameHeight{ 0.f },
	m_PlayingIntro{ playIntro },
	m_CupheadMovementState{ Movement::idle },
	m_CupheadShootingState{ Shoot::notShooting },
	m_LastMovementState{},
	m_LastShootState{},
	m_KeyPressed{ false },
	m_FrameToResetAnimation{ 0 },
	m_ShootAngle{ 0.f },
	m_IsGrounded{ false },
	m_IsHit{ false },
	m_AnimatingHit{ false },
	m_InvincibilityDuration{ 3.5f },
	m_ClickedDash{ false },
	m_IsDashing{ false },
	m_DashCooldownAcuuSec{ 0.f },
	m_DashAnimAccuSec{ 0.f },
	m_MaxFrameSec{ 0.06f },
	m_CurrentTexture{ nullptr },
	m_CurrentColNr{ 1 },
	m_CurrentRowNr{ 1 },
	m_Velocity{ 0.f, 0.f },
	m_FacingAngle{ 0.f },
	m_SpecialAllowed{ true },
	m_FiringSpecial{ false },
	m_PlaceOfHit{},
	m_TexturePeaShooter{ peaShooter },
	m_TextureSpecialPeaShooter{ peaSpecial },
	m_Animator{}
{
	IntializeTextures();
	InitializeSoundEffects();
}

Cuphead::~Cuphead() noexcept
{
	DeleteTextures();
	DeleteSoundEffects();
}

void Cuphead::Draw() const
{
	Rectf srcRect{ 0.f + (m_Animator.GetCurrentFrameNr() % m_CurrentColNr) * m_FrameWidth,
		0.f + (m_Animator.GetCurrentFrameNr() / m_CurrentColNr) * m_FrameHeight, m_FrameWidth, m_FrameHeight };

	// GetBounds is calculated by the mid point of each texture
	glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, 0); // This reduces the texture quality A LOT for some reason !!!
		glRotatef(m_FacingAngle, 0, 1, 0);
		m_CurrentTexture->Draw(Vector2f{ -GetBounds().width / 2, 0.f }, srcRect);
		
	glPopMatrix();

	// Hitbox
	/*utils::SetColor(Color4f{ 1, 0, 0, 1 });
	utils::DrawRect(GetBounds());
	utils::DrawEllipse(GetHitbox().center, GetHitbox().radius, GetHitbox().radius);
	utils::FillEllipse(m_Position, 5.f, 5.f);*/

	if (m_IsHit && m_IsAlive)
	{
		static float step{ 0.02f };
		static float alpha{ 0.f };

		utils::SetColor(Color4f{ 0.7f, 0.7f, 0.7f, alpha });
		utils::FillRect(GetBounds());

		if (alpha > 0.5f)
		{
			alpha = 0.5f;
			step = -step;
		}
		else if (alpha < 0.f)
		{
			alpha = 0.f;
			step = -step;
		}

		alpha += step;
	}
}

void Cuphead::Update(float elapsedSec, const Uint8* pStates, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, UIManager& uiManager)
{
	// set the m_IsAlive flag
	SetDeath();

	// sets the dashing state from a key event and then processes it in ProcessKeys()...
	Dash(elapsedSec);

	// determines which key is pressed where + combinations and sets the velocity
	ProcessKeys(pStates, uiManager);

	// pushes a new projectile to BulletManager
	CreateProjectiles(elapsedSec, bulletManager, uiManager);

	// sets up the current Texture which is going to be drawn and updates the current framer number via m_Aniamtor
	AnimateCuphead(elapsedSec);

	// updates the postion using m_Velocity and enables collisions from a std::vector<Vector2f>
	HandleRaycast(elapsedSec, vertices);

	// handles invincibility when hit
	TakeDamage(elapsedSec);
}

void Cuphead::ProcessKeys(const Uint8* pStates, UIManager& uiManager)
{
	m_FrameToResetAnimation = 0; // !!!

	if (!m_PlayingIntro)
	{
		if (!m_IsAlive)
		{
			m_Velocity = Vector2f{ 0.f, 150.f };
		}
		else if (m_AnimatingHit)
		{
			if (m_FacingAngle == 0.f)
			{
				m_Velocity = Vector2f{ -100.f, 200.f };
			}
			else
			{
				m_Velocity = Vector2f{ 100.f, 200.f };
			}
		}
		else
		{
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
			else if(!m_FiringSpecial)
			{
				// determines the whole rotation logic + enables dashing in one direction
				UpdateFacingDirection(pStates);
				
				const float movementSpeed{ 400.f };

				// change movement and shooting state, statring with COMBINATIONS
				if (pStates[SDL_SCANCODE_Z])
				{
					// parry
					if (m_CupheadMovementState == Movement::parry)
					{
						if (pStates[SDL_SCANCODE_LEFT])
						{
							m_Velocity.x = -movementSpeed * 0.75f;
						}
						if (pStates[SDL_SCANCODE_RIGHT])
						{
							m_Velocity.x = movementSpeed * 0.75f;
						}

						if (m_IsGrounded)
						{
							m_CupheadMovementState = Movement::idle;
						}
					}
					// jump
					else
					{
						m_CupheadMovementState = Movement::jump;
						m_CupheadShootingState = Shoot::notShooting; // 3

						if (pStates[SDL_SCANCODE_LEFT])
						{
							m_Velocity.x = -movementSpeed * 0.75f;
						}
						if (pStates[SDL_SCANCODE_RIGHT])
						{
							m_Velocity.x = movementSpeed * 0.75f;
						}

						if (m_IsGrounded)
						{
							m_JumpSFX->Play(0);
							m_Velocity = Vector2f{ 0.f, 800.f };
						}
					}
				}
				// special up left
				else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialLeft;

					m_ShootAngle = 135.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}

					m_FrameToResetAnimation = 12;
				}
				// special down left
				else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_DOWN] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialLeft;

					m_ShootAngle = 225.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}

					m_FrameToResetAnimation = 12;
				}
				// special up right
				else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialRight;

					m_ShootAngle = 45.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					m_FrameToResetAnimation = 12;
				}
				// special down right
				else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_DOWN] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialRight;

					m_ShootAngle = 315.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					m_FrameToResetAnimation = 12;
				}
				// special left
				else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialLeft;

					m_ShootAngle = 180.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ 200.f, 300.f };
					}

					m_FrameToResetAnimation = 12;
				}
				// special right
				else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialRight;

					m_ShootAngle = 0.f;
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ -200.f, 300.f };
					}
					m_FrameToResetAnimation = 12;
				}
				// special up
				else if (pStates[SDL_SCANCODE_UP] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialUp;

					m_ShootAngle = 90.f;

					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 0.f, 0.f };
					}
					else
					{
						m_Velocity = Vector2f{ 0.f, 300.f };
					}
					m_FrameToResetAnimation = 0;
				}
				// special down
				else if (pStates[SDL_SCANCODE_DOWN] && pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					m_CupheadShootingState = Shoot::specialDown;

					m_ShootAngle = 270.f;

					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 0.f, 300.f };
					}
					else
					{
						m_Velocity = Vector2f{ 0.f, 400.f };
					}

					m_FrameToResetAnimation = 24;
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
					m_FrameToResetAnimation = 5;
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
					m_FrameToResetAnimation = 5;
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
					m_FrameToResetAnimation = 10;
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
					m_FrameToResetAnimation = 10;
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
					m_FrameToResetAnimation = 5;
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
					m_FrameToResetAnimation = 5;
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
					m_FrameToResetAnimation = 10;
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
					m_FrameToResetAnimation = 10;
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
					m_FrameToResetAnimation = 0;
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
					m_FrameToResetAnimation = 20;
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
				else if (pStates[SDL_SCANCODE_V] && m_SpecialAllowed && uiManager.SpecialAttackAllowed())
				{
					m_CupheadMovementState = Movement::lock;
					
					if (m_FacingAngle == 0.f)
					{
						m_CupheadShootingState = Shoot::specialRight;

						if (m_IsGrounded)
						{
							m_Velocity = Vector2f{ -200.f, 300.f };
						}
						else 
						{
							m_Velocity = Vector2f{ -200.f, 300.f }; 
						}
						
						m_ShootAngle = m_FacingAngle;
					}
					else if (m_FacingAngle == 180.f)
					{
						m_CupheadShootingState = Shoot::specialLeft;

						if (m_IsGrounded)
						{
							m_Velocity = Vector2f{ 200.f, 300.f };
						}
						else
						{
							m_Velocity = Vector2f{ 200.f, 300.f };
						}

						m_ShootAngle = m_FacingAngle;
					}

					m_FrameToResetAnimation = 12;
				}
				else if (pStates[SDL_SCANCODE_X])
				{
					m_CupheadMovementState = Movement::lock;
					
					if (m_IsGrounded)
					{
						m_Velocity = Vector2f{ 0.f, 0.f };
					}

					// important DEFAULT direction!
					if (m_FacingAngle == 0.f)
					{
						m_CupheadShootingState = Shoot::shootRight;
						m_ShootAngle = 0.f;
					}
					else if (m_FacingAngle == 180.f)
					{
						m_CupheadShootingState = Shoot::shootLeft;
						m_ShootAngle = 180.f;
					}

					m_FrameToResetAnimation = 10;
				}

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
	
	ResetAnimation(m_FrameToResetAnimation);
}

void Cuphead::AnimateCuphead(float elapsedSec)
{
	if (m_PlayingIntro)
	{
		m_CurrentTexture = m_TextureIntro;
		m_CurrentColNr = 7;
		m_CurrentRowNr = 4;

		m_Animator.AnimateBetweenFrames(elapsedSec, 12, 19, m_MaxFrameSec, 1.5f);
		
		if (m_Animator.GetCurrentFrameNr() > 27)
		{
			m_Animator.Reset(0);
			m_PlayingIntro = false;
		}
	}
	else if (!m_IsAlive)
	{
		m_CurrentTexture = m_TextureGhost;
		m_CurrentColNr = 6;
		m_CurrentRowNr = 4;

		m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec - 0.01f);
	}
	else if (m_AnimatingHit)
	{
		m_CurrentTexture = m_TextureHit;
		m_CurrentColNr = 3;
		m_CurrentRowNr = 2;

		const float animDuration{ 0.7f };
		static float animAccuSec{ 0.f };
		animAccuSec += elapsedSec;

		m_Animator.BounceBetween(elapsedSec, 0, 5, m_MaxFrameSec + 0.01f);

		if (animAccuSec > animDuration)
		{
			m_AnimatingHit = false;
			animAccuSec -= animDuration;
		}
	}
	else if (m_FiringSpecial)	
	{
		m_CurrentTexture = m_TextureShootSpecial;
		m_CurrentColNr = 6;
		m_CurrentRowNr = 5;

		m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec - 0.01f);
	}
	else
	{
		if (m_CupheadMovementState == Movement::idle)
		{
			// jump animation plays if no button is pressed and in the air
			if (!m_IsGrounded)
			{
				m_CurrentTexture = m_TextureJump;
				m_CurrentColNr = 4;
				m_CurrentRowNr = 2;
				m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec - 0.01f);
			}
			else
			{
				m_CurrentTexture = m_TextureIdle;
				m_CurrentColNr = 5;
				m_CurrentRowNr = 1;

				m_Animator.BounceBetween(elapsedSec, 0, 4, m_MaxFrameSec - 0.01f);
			}
		}
		else if (m_CupheadShootingState == Shoot::notShooting)
		{
			// jumping animation is played if character is moving in the air
			if (!m_IsGrounded && m_CupheadMovementState != Movement::dash && m_CupheadMovementState != Movement::parry)
			{
				m_CurrentTexture = m_TextureJump;
				m_CurrentColNr = 4;
				m_CurrentRowNr = 2;
				m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec - 0.01f);
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
				case Cuphead::Movement::jump: // jumping animation is played when z is pressed
					m_CurrentTexture = m_TextureJump;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 2;

					m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec - 0.01f, 8);
					break;
				case Cuphead::Movement::parry:
					m_CurrentTexture = m_TextureParry;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.AnimateBetweenFrames(elapsedSec, 0, 7, m_MaxFrameSec - 0.01f);
					break;
				case Cuphead::Movement::dash:
					m_CurrentTexture = m_TextureDash;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 2;

					m_Animator.AnimateBetweenFrames(elapsedSec, 2, 6, m_MaxFrameSec);
					break;
				case Cuphead::Movement::duck:
					m_CurrentTexture = m_TextureDuck;
					m_CurrentColNr = 4;
					m_CurrentRowNr = 4;

					m_Animator.AnimateBetweenFrames(elapsedSec, 7, 14, m_MaxFrameSec);
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
			else if (m_CupheadMovementState == Movement::runRight)
			{
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
			else 
			{
				m_CurrentTexture = m_TextureShoot;
				m_CurrentColNr = 5;
				m_CurrentRowNr = 5;

				//m_Animator.PlayAnimation(elapsedSec, m_MaxFrameSec + 0.01f);
				switch (m_CupheadShootingState)
				{
				case Cuphead::Shoot::shootUp:
					m_Animator.BounceBetween(elapsedSec, 0, 4, m_MaxFrameSec + 0.01f);
					break;
				case Cuphead::Shoot::shootDiagonalUpRight:
					m_Animator.BounceBetween(elapsedSec, 5, 9, m_MaxFrameSec + 0.01f);
					break;
				case Cuphead::Shoot::shootRight:
					m_Animator.BounceBetween(elapsedSec, 10, 14, m_MaxFrameSec + 0.01f);
					break;
				case Cuphead::Shoot::shootDiagonalUpLeft:
					m_Animator.BounceBetween(elapsedSec, 5, 9, m_MaxFrameSec + 0.01f);
					break;
				case Cuphead::Shoot::shootLeft:
					m_Animator.BounceBetween(elapsedSec, 10, 14, m_MaxFrameSec + 0.01f);
					break;
				case Cuphead::Shoot::shootDown:
					m_Animator.BounceBetween(elapsedSec, 20, 24, m_MaxFrameSec + 0.01f);
					break;
				}
			}
		}
	}

	assert(("Cuphead Texture not set properly -> resulting in a nullptr.", m_CurrentTexture != nullptr));
	m_FrameWidth = m_CurrentTexture->GetWidth() / m_CurrentColNr;
	m_FrameHeight = m_CurrentTexture->GetHeight() / m_CurrentRowNr;
}

void Cuphead::ResetAnimation(int frameToReset)
{
	bool stateChanged{ (m_CupheadMovementState != m_LastMovementState) || (m_CupheadShootingState != m_LastShootState) };
	if (stateChanged)
	{
		m_Animator.Reset(frameToReset);
		m_LastMovementState = m_CupheadMovementState;
		m_LastShootState = m_CupheadShootingState;
	}
}

void Cuphead::HandleRaycast(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices)
{
	//std::cout << "Velocity: " << m_Velocity << std::endl;
	if (m_IsAlive)
	{
		m_IsGrounded = false;

		Vector2f firstPointY{ m_Position.x, m_Position.y + GetBounds().height / 5 };
		Vector2f secondPointY{ m_Position.x, m_Position.y - 1.f };

		utils::HitInfo hitInfoY;

		for (size_t i{}; i < vertices.size(); ++i)
		{
			// GROUND collision
			if (utils::Raycast(vertices[i], firstPointY, secondPointY, hitInfoY))
			{
				if (m_Velocity.y <= 0.f) // platform check
				{
					if (!m_KeyPressed)
					{
						m_Velocity = Vector2f{ 0.f, 0.f };
					}

					m_Position.y = hitInfoY.intersectPoint.y;
					m_IsGrounded = true;
				}
			}
			if (!m_IsGrounded)
			{
				m_Velocity += Vector2f{ 0, -9.8f * 20 } * elapsedSec; // if the gravity is only -9.8f, it flies off
			}
		}

		for (size_t i{}; i < vertices.size(); ++i)
		{
			Vector2f firstPointX{ GetBounds().left, m_Position.y + GetBounds().height / 2 };
			Vector2f secondPointX{ GetBounds().left + GetBounds().width, m_Position.y + GetBounds().height / 2 };
			utils::HitInfo hitInfoX;

			// RIGHT collision
			if (utils::Raycast(vertices[i], firstPointX, secondPointX, hitInfoX))
			{
				if (!m_KeyPressed)
				{
					m_Velocity.x = 0.f;
				}

				m_Position.x = hitInfoX.intersectPoint.x - (GetBounds().width / 2 * hitInfoX.normal.x);	
			}

			// LEFT collision
			if (utils::Raycast(vertices[i], secondPointX, firstPointX, hitInfoX))
			{
				if (!m_KeyPressed)
				{
					m_Velocity.x = 0.f;
				}

				m_Position.x = hitInfoX.intersectPoint.x - (GetBounds().width / 2 * hitInfoX.normal.x);
			}
		}
	}

	m_Position += m_Velocity * elapsedSec;
}

void Cuphead::Dash(float elapsedSec)
{
	const float dashDuration{ 0.3f };
	const float dashCooldown{ 0.3f };

	// dash cooldown going down
	if (m_DashCooldownAcuuSec > 0.f)
	{
		m_DashCooldownAcuuSec -= elapsedSec;

		if (m_DashCooldownAcuuSec < 0.f) // end of coooldown, dash available
		{
			m_DashCooldownAcuuSec = 0.f;
		}
	}

	if (m_ClickedDash && m_DashAnimAccuSec == 0.f) 
	{
		m_IsDashing = true;
		m_ClickedDash = false;
	}

	if (m_IsDashing) 
	{
		m_DashAnimAccuSec += elapsedSec;

		if (m_DashAnimAccuSec > dashDuration) // if dash reaches the end of the animation
		{
			m_IsDashing = false;
			m_DashCooldownAcuuSec = dashCooldown; // -> reset the cooldown timer
			m_DashAnimAccuSec = 0.f;
		}
	}
}

void Cuphead::CreateProjectiles(float elapsedSec, BulletManager& bulletManager, UIManager& uiManager)
{
	static float m_AccuSecProjectiles{ 0.f };
	static float m_AccuSpecial{ 0.f };

	if (!m_SpecialAllowed)
	{
		m_AccuSpecial += elapsedSec;

		if (m_AccuSpecial >= 1.f)
		{
			m_AccuSpecial -= 1.f;
			m_SpecialAllowed = true;
		}
	}

	if (m_CupheadShootingState != Shoot::notShooting)
	{
		if (m_CupheadShootingState == Shoot::shootDiagonalUpLeft || m_CupheadShootingState == Shoot::shootDiagonalUpRight || m_CupheadShootingState == Shoot::shootDown ||
			m_CupheadShootingState == Shoot::shootLeft || m_CupheadShootingState == Shoot::shootRight || m_CupheadShootingState == Shoot::shootUp) // if shooting in any direction
		{
			m_AccuSecProjectiles += elapsedSec;

			if (m_AccuSecProjectiles >= 0.15f)
			{
				int randNr{ rand() % 3 + 1 };
				switch (randNr)
				{
				case 1:
					m_PeaShooterSFX1->Play(0);
					break;
				case 2:
					m_PeaShooterSFX2->Play(0);
					break;
				case 3:
					m_PeaShooterSFX3->Play(0);
					break;
				}

				bulletManager.AddProjectile(new PeaShooter(m_TexturePeaShooter,
					Vector2f{ GetBounds().left + GetBounds().width / 2, GetBounds().bottom + GetBounds().height * 0.4f }, {}, m_ShootAngle, 1300.f, 1.5f));
				m_AccuSecProjectiles -= 0.15f;
			}
		}
		else if (m_SpecialAllowed)
		{
			m_FiringSpecial = true;
			uiManager.ChangeCards(-20);

			m_SpecialPeaSFX1->Play(0);

			bulletManager.AddProjectile(new PeaShooter(m_TextureSpecialPeaShooter,
				Vector2f{ GetBounds().left + GetBounds().width / 2, GetBounds().bottom + GetBounds().height * 0.4f }, {}, m_ShootAngle, 900.f, 0.8f, 4, 2));
			m_SpecialAllowed = false;

		}
	}

	static float specialAnimTimer{ 0.f };
	if (m_FiringSpecial)
	{
		specialAnimTimer += elapsedSec;
		if (specialAnimTimer >= 0.5f)
		{
			specialAnimTimer -= 0.5f;
			m_FiringSpecial = false;
			m_CupheadMovementState = Movement::idle;
		}
	}
}

void Cuphead::StartDash()
{
	if (!m_PlayingIntro && (m_DashCooldownAcuuSec <= 0.f && m_DashAnimAccuSec == 0.f) && !m_AnimatingHit && !m_FiringSpecial)
	{
		m_ClickedDash = true;
		
		m_DashSFX->Play(0);
		
		m_CupheadMovementState = Movement::dash;
		m_CupheadShootingState = Shoot::notShooting;
	}
}

void Cuphead::ToggleParryState() // toggling the movement state
{
	if (!m_PlayingIntro && !m_IsGrounded && m_CupheadMovementState!= Movement::jump)
	{
		m_CupheadMovementState = Movement::parry;
		m_CupheadShootingState = Shoot::notShooting;
	}
}

void Cuphead::Hit()
{
	if (m_IsHit == false)
	{
		m_PlaceOfHit = m_Position;
		m_AnimatingHit = true;
		m_IsHit = true;
		m_HP--;

		if (m_HP >= 1)
		{
			int randNr{ rand() % 2 + 1 };
			switch (randNr)
			{
			case 1:
				m_HitSFX1->Play(0);
				break;
			case 2:
				m_HitSFX2->Play(0);
				break;
			}
			m_CrackSFX->Play(0);
		}
	}
}

int Cuphead::GetHealth() const // to check if isDead in Game.cpp in future
{
	return m_HP;
}

bool Cuphead::IsParrying() const
{
	return m_CupheadMovementState == Movement::parry;
}

void Cuphead::Parry(UIManager& uiManager)
{
	m_ParrySFX->Play(0);
	m_SlapSFX->Play(0);
	uiManager.ChangeCards(20);
	m_Velocity = Vector2f{ 0.f, 700.f };
}

void Cuphead::ResetPlayer(const Vector2f& pos)
{
	m_IsAlive = true;
	m_FacingAngle = 0.f;
	m_Position = pos;
	m_HP = 3;
	m_PlayingIntro = true;
	m_Velocity = Vector2f{};
	m_CupheadMovementState = Movement::idle;
	m_CupheadShootingState = Shoot::notShooting;
	m_Animator.Reset(0);

	// I'm not resetting the hit animation on purpose = )
}

Vector2f Cuphead::GetPosition() const
{
	return m_Position;
}

Rectf Cuphead::GetBounds() const
{
	return Rectf{ m_Position.x - m_FrameWidth / 2, m_Position.y, m_FrameWidth, m_FrameHeight };
}

Circlef Cuphead::GetHitbox() const
{
	if (!m_IsAlive || m_IsHit)
	{
		return Circlef();
	}
	else if (m_CupheadMovementState == Movement::dash)
	{
		if (m_FacingAngle == 0.f)
		{
			return Circlef(m_Position.x + m_FrameWidth / 4, m_Position.y + m_FrameHeight / 2, m_FrameHeight / 4);
		}
		else return Circlef(m_Position.x - m_FrameWidth / 4, m_Position.y + m_FrameHeight / 2, m_FrameHeight / 4);
	}
	else if (m_CupheadMovementState == Movement::duck && m_IsGrounded)
	{
		return Circlef(m_Position.x, m_Position.y + m_FrameHeight / 4, m_FrameHeight / 3);
	}
	return Circlef(m_Position.x, m_Position.y + m_FrameHeight / 2, m_FrameWidth / 4);
}

Vector2f Cuphead::GetPlaceOfDeath() const
{
	return m_PlaceOfHit;
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

void Cuphead::TakeDamage(float elapsedSec)
{
	if (m_Position.y <= 0.f)
	{
		if (!m_IsHit)
		{
			Hit();
		}
		m_Velocity.y = 1000.f;
	}

	if (m_IsHit)
	{
		static float invAccuSec{ 0.f };
		invAccuSec += elapsedSec;

		if (invAccuSec >= m_InvincibilityDuration)
		{
			m_IsHit = false;
			invAccuSec -= m_InvincibilityDuration;
		}
		m_CupheadShootingState = Shoot::notShooting;
	}
}

void Cuphead::SetDeath()
{
	static bool canPlayDeath{ false };
	if (m_HP <= 0)
	{
		m_IsAlive = false;
		m_CupheadShootingState = Shoot::notShooting;

		if (!canPlayDeath)
		{
			m_DeathSFX->Play(0);
			canPlayDeath = true;
		}
	}
}

void Cuphead::IntializeTextures()
{
	m_TextureDash = new Texture("Cuphead_Sprites/Cuphead_Dash.png");
	m_TextureShoot = new Texture("Cuphead_Sprites/Cuphead_Shooting.png");
	m_TextureShootSpecial = new Texture("Cuphead_Sprites/Special_Attack_Air.png");
	m_TextureDuck = new Texture("Cuphead_Sprites/Duck.png");
	m_TextureGhost = new Texture("Cuphead_Sprites/Ghost.png");
	m_TextureHit = new Texture("Cuphead_Sprites/Hit_Ground.png");
	m_TextureIdle = new Texture("Cuphead_Sprites/Idle.png");
	m_TextureIntro = new Texture("Cuphead_Sprites/Intro.png");
	m_TextureJump = new Texture("Cuphead_Sprites/Jump.png");
	m_TextureParry = new Texture("Cuphead_Sprites/Parry_Combined.png");
	m_TextureRun = new Texture("Cuphead_Sprites/Run.png");
	m_TextureRunShootDiagonal = new Texture("Cuphead_Sprites/Run_Shoot_Diagonal.png");
	m_TextureRunShootStraight = new Texture("Cuphead_Sprites/Run_Shoot_Straight.png");
}

void Cuphead::InitializeSoundEffects()
{
	m_DashSFX = new SoundEffect("Audio/Cuphead_Player/Dash SFX.wav");
	m_JumpSFX = new SoundEffect("Audio/Cuphead_Player/Jump SFX.wav");
	m_ParrySFX = new SoundEffect("Audio/Cuphead_Player/Parry SFX.wav");
	m_SlapSFX = new SoundEffect("Audio/Cuphead_Player/Slap SFX.wav");
	m_DeathSFX = new SoundEffect("Audio/Cuphead_Player/Death SFX.mp3");
	m_CrackSFX = new SoundEffect("Audio/Cuphead_Player/Crack SFX.wav");
	m_HitSFX1 = new SoundEffect("Audio/Cuphead_Player/Hit 1 SFX.wav");
	m_HitSFX2 = new SoundEffect("Audio/Cuphead_Player/Hit 2 SFX.wav");

	m_PeaShooterSFX1 = new SoundEffect("Audio/PeaShooter/1.wav");
	m_PeaShooterSFX2 = new SoundEffect("Audio/PeaShooter/3.wav");
	m_PeaShooterSFX3 = new SoundEffect("Audio/PeaShooter/6.wav");

	m_SpecialPeaSFX1 = new SoundEffect("Audio/PeaShooter/Special 1 SFX.wav");
}

void Cuphead::DeleteTextures()
{
	delete m_TextureDash;
	m_TextureDash = nullptr;
	delete m_TextureShoot;
	m_TextureShoot = nullptr;
	delete m_TextureShootSpecial;
	m_TextureShootSpecial = nullptr;
	delete m_TextureDuck;
	m_TextureDuck = nullptr;
	delete m_TextureGhost;
	m_TextureGhost = nullptr;
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

void Cuphead::DeleteSoundEffects()
{
	delete m_DashSFX;
	m_DashSFX = nullptr;
	delete m_JumpSFX;
	m_JumpSFX = nullptr;
	delete m_ParrySFX;
	m_ParrySFX = nullptr;
	delete m_SlapSFX;
	m_SlapSFX = nullptr;
	delete m_DeathSFX;
	m_DeathSFX = nullptr;
	delete m_CrackSFX;
	m_CrackSFX = nullptr;
	delete m_HitSFX1;
	m_HitSFX1 = nullptr;
	delete m_HitSFX2;
	m_HitSFX2 = nullptr;

	delete m_PeaShooterSFX1;
	m_PeaShooterSFX1 = nullptr;
	delete m_PeaShooterSFX2;
	m_PeaShooterSFX1 = nullptr;
	delete m_PeaShooterSFX3;
	m_PeaShooterSFX1 = nullptr;

	delete m_SpecialPeaSFX1;
	m_SpecialPeaSFX1 = nullptr;
}
