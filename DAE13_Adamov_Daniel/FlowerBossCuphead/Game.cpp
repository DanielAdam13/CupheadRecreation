#include "pch.h"
#include "Game.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"
#include "Camera.h"
#include "Enemy.h"
#include "Spike.h"
#include "BigChomper.h"
#include "Mushroom.h"
#include "Tulip.h"
#include "Acorn.h"
#include "Projectile.h"
#include "SVGParser.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Vertices{},
	m_ForestBackground1{ new Texture("ForestFollies_Background_First_Smaller.png") },
	m_ForestBackground2{ new Texture("ForestFollies_Background_Second_Smaller.png") },
	m_Camera{ GetViewPort().width, GetViewPort().height, (GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth())) * 9.95f
	, (GetViewPort().height / m_ForestBackground1->GetHeight()) * 1.29f },
	m_EnemyManager{},
	m_PlayerBulletManager{},
	m_EnemyBulletManager{},
	m_CurrentGameState{ GameState::intro },
	m_HealthSprite{ new Texture("UI/Health_Stages.png") },
	m_CardsSprite{ new Texture("UI/Cards_Stages.png") },
	m_IntroAnnouncment{ new Texture("UI/Stages_Intro.png") }, // + ~60 mb memory
	m_DeathAnnouncment{ new Texture("UI/YouDied_Stages.png") }, // + ~45 mb memory
	m_BravoAnnouncment{ new Texture("UI/Bravo_Sprite.png") }, // + 200+ mb !!!!!!!!!!!!!!!!!!???
	m_PauseScreen{ new Texture("UI/Pause_Screen.bmp") }, // 20 mb
	m_UIManager{ m_HealthSprite, m_CardsSprite, m_IntroAnnouncment, m_DeathAnnouncment, m_BravoAnnouncment, m_PauseScreen },
	m_PeaShooterSprite{ new Texture("Projectile_Loop.png") },
	m_PeaSpecialSprite{ new Texture("Projectile_Special_Loop.png") },
	m_Cuphead{ Vector2f{ GetViewPort().width / 2, GetViewPort().height * 7 / 36.f}, (m_CurrentGameState == GameState::intro), 3, m_PeaShooterSprite, m_PeaSpecialSprite},
	m_SpikeSprite{ new Texture("Run'N'Gun/Sprite_Spike.png") },
	m_ChomperSprite{ new Texture("Run'N'Gun/Sprite_Chomper.png") },
	m_TulipIdle{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Idle.png") },
	m_TulipAttack{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Attack.png") },
	m_TulipSeed{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Seed.png") },
	m_TulipSeedExplosion{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Explosion.png") }, // + ~25 mb memory
	m_MushroomIdle{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Idle.png") },
	m_MushroomAttack{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Attack.png") },
	m_MushroomPop{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Pop.png") },
	m_MushroomBoil{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Boil.png") },
	m_MushroomDeath{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Death.png") },
	m_MushroomCloud{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Cloud.png") },
	m_AcornIdle{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Fly.png") },
	m_AcornDrop{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Drop.png") },
	m_ScaledLevelWidth{GetViewPort().width * 9.95f},
	m_ScaledLevelHeight{ GetViewPort().height * 1.29f }
{
	Initialize(); 
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 500.f, 250.f }, Vector2f{ 500.f, 150.f }, Vector2f{ 500.f, 350.f }, 250.f));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 1500.f, 250.f }, Vector2f{ 1500.f, 150.f }, Vector2f{ 1500.f, 350.f }, 250.f));

	m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 1200.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 1200.f, 500.f }, 600.f));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 1700.f, 250.f }, Vector2f{ 500.f, 150.f }, Vector2f{ 500.f, 350.f }, 250.f));

	/*m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 2800.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 2800.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 2800.f, 450.f }, 600.f));*/

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 3850.f, 280.f }));
	//m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 1000.f, 170.f }, 5, 4, m_Camera.GetCuurentCameraBounds().width / 2));

	m_EnemyManager.AddEnemy(new Mushroom(m_MushroomIdle, m_MushroomBoil, m_MushroomAttack, m_MushroomPop, m_MushroomDeath, m_MushroomCloud, 
		Vector2f{ 2800, 150.f},
		1, 1, m_Camera.GetCuurentCameraBounds().width / 2.5f));

	SVGParser::GetVerticesFromSvgFile("ForestFollies_Background_Finished_Smaller_3.svg", m_Vertices);
}

void Game::Cleanup( )
{
	delete m_ForestBackground1;
	m_ForestBackground1 = nullptr;

	delete m_ForestBackground2;
	m_ForestBackground2 = nullptr;

	delete m_HealthSprite;
	m_HealthSprite = nullptr;
	delete m_CardsSprite;
	m_CardsSprite = nullptr;
	delete m_IntroAnnouncment;
	m_IntroAnnouncment = nullptr;
	delete m_DeathAnnouncment;
	m_DeathAnnouncment = nullptr;
	delete m_BravoAnnouncment;
	m_BravoAnnouncment = nullptr;
	delete m_PauseScreen;
	m_PauseScreen = nullptr;

	delete m_PeaShooterSprite;
	m_PeaShooterSprite = nullptr;
	delete m_PeaSpecialSprite;
	m_PeaSpecialSprite = nullptr;

	delete m_SpikeSprite;
	m_SpikeSprite = nullptr;

	delete m_ChomperSprite;
	m_ChomperSprite = nullptr;

	delete m_TulipAttack;
	m_TulipAttack = nullptr;
	delete m_TulipIdle;
	m_TulipIdle = nullptr;
	delete m_TulipSeed;
	m_TulipSeed = nullptr;
	delete m_TulipSeedExplosion;
	m_TulipSeedExplosion = nullptr;

	delete m_MushroomIdle;
	m_MushroomIdle = nullptr;
	delete m_MushroomAttack;
	m_MushroomAttack = nullptr;
	delete m_MushroomPop;
	m_MushroomPop = nullptr;
	delete m_MushroomBoil;
	m_MushroomBoil = nullptr;
	delete m_MushroomDeath;
	m_MushroomDeath = nullptr;
	delete m_MushroomCloud;
	m_MushroomCloud = nullptr;

	delete m_AcornIdle;
	m_AcornIdle = nullptr;
	delete m_AcornDrop;
	m_AcornDrop = nullptr;

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		m_Vertices[i].clear();
	}
	m_Vertices.clear();
}

void Game::Update( float elapsedSec )
{
	static float m_AccuSec{ 0.f };

	
	
	switch (m_CurrentGameState)
	{
	case Game::GameState::intro:
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= 2.8f)
		{
			m_CurrentGameState = GameState::gameplay;
			m_AccuSec -= 2.8f;
		}
		break;
	case Game::GameState::death:
		//m_AccuSec += elapsedSec;
		break;
	}

	if (m_CurrentGameState != GameState::pause)
	{
		// Check keyboard state
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		// updates cuphead and pushes projectiles to player BulletManager
		m_Cuphead.Update(elapsedSec, pStates, m_Vertices, m_PlayerBulletManager, m_UIManager);

		// update enemies and enemy bullets if player is alive
		if (m_Cuphead.GetHealth() > 0)
		{
			SpawnAcorns(elapsedSec);

			// update enemies and pushes projectiles to enemy BulletManager
			m_EnemyManager.UpdateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_EnemyBulletManager, m_Cuphead, m_UIManager);

			m_EnemyBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices, m_Cuphead, m_UIManager);

			Vector2f transf{ m_Cuphead.GetPosition().x * (GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth())) * 9.95f,
			   m_Cuphead.GetPosition().y * (GetViewPort().height / m_ForestBackground1->GetHeight()) * 1.29f };
			// follow player if alive
			m_Camera.Aim(m_ScaledLevelWidth, m_ScaledLevelHeight, transf);
		}
		else
		{
			m_CurrentGameState = GameState::death;
			// unfollow if player dead
			Vector2f transf{ m_Cuphead.GetPlaceOfDeath().x * (GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth())) * 9.95f,
			   m_Cuphead.GetPlaceOfDeath().y * (GetViewPort().height / m_ForestBackground1->GetHeight()) * 1.29f };
			m_Camera.Aim(m_ScaledLevelWidth, m_ScaledLevelHeight, transf);
		}

		// animate enemy bullets even if player is dead
		m_EnemyBulletManager.AnimateActiveBullets(elapsedSec);

		// animate enemies even if player is dead
		m_EnemyManager.AnimateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds());

		// update and animate even if player is dead
		m_PlayerBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices, m_Cuphead, m_UIManager);

		m_PlayerBulletManager.AnimateActiveBullets(elapsedSec);

		// delete player bullets if they collide with enemy and deal damage
		ManagePlayerProjectiles();

		ManageTakingDamageCuphead();
	}
	else
	{
		// follow player if paused
		Vector2f transf{ m_Cuphead.GetPosition().x * (GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth())) * 9.95f,
			   m_Cuphead.GetPosition().y * (GetViewPort().height / m_ForestBackground1->GetHeight()) * 1.29f };
		m_Camera.Aim(m_ScaledLevelWidth, m_ScaledLevelHeight, transf);
	}

	m_UIManager.Update(elapsedSec, m_Cuphead.GetHealth(), int(m_CurrentGameState));
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	m_Camera.Apply();
	
	DrawScaledObjects();

	m_Camera.DrawBorderOverlay(GetViewPort().height / 20);
	m_Camera.Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	switch (e.keysym.sym)
	{
	case SDLK_LSHIFT:
		m_Cuphead.StartDash();
		break;
	case SDLK_z:
		m_Cuphead.ToggleParryState();
		break;
	case SDLK_ESCAPE:
		if (m_CurrentGameState == GameState::gameplay)
		{
			m_CurrentGameState = GameState::pause;
		}
		else if (m_CurrentGameState == GameState::pause)
		{
			m_CurrentGameState = GameState::gameplay;
		}
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ManagePlayerProjectiles()
{
	for (int j{}; j < m_EnemyManager.GetVectorSize(); ++j)
	{
		if (m_EnemyManager[j] != nullptr)
		{
			for (int i{}; i < m_PlayerBulletManager.GetVectorSize(); ++i)
			{
				if (m_PlayerBulletManager[i] != nullptr)
				{
					if (utils::IsOverlapping(m_EnemyManager[j]->GetBounds(), m_PlayerBulletManager[i]->GetHitbox()))
					{
						m_EnemyManager[j]->TakeDamage(m_PlayerBulletManager[i]->Damage(), m_UIManager);

						if (m_PlayerBulletManager[i]->Damage() - 0.8f > 0.001f)
						{
							m_PlayerBulletManager.RemoveProjectile(i);
						}
					}
				}
			}
		}
	}
}

void Game::ManageTakingDamageCuphead()
{
	// deal damage to cuphead by enemy collisions
	for (int i{}; i < m_EnemyManager.GetVectorSize(); ++i)
	{
		if (m_EnemyManager[i] != nullptr)
		{
			if (utils::IsOverlapping(m_EnemyManager[i]->GetBounds(), m_Cuphead.GetHitbox()) && m_Cuphead.GetHealth() > 0)
			{
				m_Cuphead.Hit();
			}
		}
	}

	// deal damage to cuphead by projectiles
	for (int i{}; i < m_EnemyBulletManager.GetVectorSize(); ++i)
	{
		if (m_EnemyBulletManager[i] != nullptr)
		{
			if (utils::IsOverlapping(m_Cuphead.GetHitbox(), m_EnemyBulletManager[i]->GetHitbox()))
			{
				m_Cuphead.Hit();
			}
		}
	}
}

void Game::DrawScaledObjects() const
{
	glPushMatrix();

	glScalef((GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth())) * 9.95f, // x ratio
		(GetViewPort().height / m_ForestBackground1->GetHeight()) * 1.29f, // y ratio
		0.f);

	m_ForestBackground1->Draw();
	m_ForestBackground2->Draw(Rectf{ m_ForestBackground1->GetWidth(), 0.f, m_ForestBackground2->GetWidth(), m_ForestBackground2->GetHeight() });
	m_Cuphead.Draw();

	// Hitbox
	/*utils::SetColor(Color4f{ 1,0,1,1 });
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		utils::DrawPolygon(m_Vertices[i], true, 2.f);
	}*/

	m_PlayerBulletManager.DrawActiveBullets();
	m_EnemyBulletManager.DrawActiveBullets();

	m_EnemyManager.DrawEnemies(m_Camera.GetCuurentCameraBounds());
	m_UIManager.Draw(m_Camera.GetCuurentCameraBounds());


	glPopMatrix();
}

void Game::SpawnAcorns(float elapsedSec)
{
	static bool allowedSpawn{ false };

	if (!allowedSpawn && m_Cuphead.GetPosition().x >= (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth()) / 3)
	{
		allowedSpawn = true; // stays on
	}

	if (allowedSpawn)
	{
		static float accuSec{ 5.f };
		accuSec += elapsedSec;

		if (accuSec >= 5.f)
		{
			int randNr{ rand() % 2 };

			if (randNr == 0) // from left
			{
				m_EnemyManager.AddEnemy(new Acorn(m_AcornIdle, m_AcornDrop,
					Vector2f{ m_Camera.GetCuurentCameraBounds().left - m_Camera.GetCuurentCameraBounds().width * 0.05f, 
					m_Camera.GetCuurentCameraBounds().bottom + m_Camera.GetCuurentCameraBounds().height * 0.9f},
					Vector2f{ 1, 0 }));
			}
			else // from right
			{
				m_EnemyManager.AddEnemy(new Acorn(m_AcornIdle, m_AcornDrop,
					Vector2f{ m_Camera.GetCuurentCameraBounds().left + m_Camera.GetCuurentCameraBounds().width * 1.05f, 
					m_Camera.GetCuurentCameraBounds().bottom + m_Camera.GetCuurentCameraBounds().height * 0.9f},
					Vector2f{ -1, 0 }));
			}

			accuSec -= 5.f;
		}
	}
}
