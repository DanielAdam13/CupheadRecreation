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

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Vertices{},
	m_ForestBackground1{ new Texture("ForestFollies_Background_First.png") },
	m_ForectBackground2{ new Texture("ForestFollies_Background_Second.png") },
	m_Camera{ GetViewPort().width, GetViewPort().height },
	m_EnemyManager{},
	m_PlayerBulletManager{},
	m_EnemyBulletManager{},
	m_HealthSprite{ new Texture("UI/Health_Stages.png") },
	m_CardsSprite{ new Texture("UI/Cards_Stages.png") },
	m_UIManager{ m_HealthSprite, m_CardsSprite },
	m_PeaShooterSprite{ new Texture("Projectile_Loop.png") },
	m_PeaSpecialSprite{ new Texture("Projectile_Special_Loop.png") },
	m_Cuphead{ Vector2f{GetViewPort().width / 2, 100.f}, true, 3, m_PeaShooterSprite, m_PeaSpecialSprite },
	m_SpikeSprite{ new Texture("Run'N'Gun/Sprite_Spike.png") },
	m_ChomperSprite{ new Texture("Run'N'Gun/Sprite_Chomper.png") },
	m_TulipIdle{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Idle.png") },
	m_TulipAttack{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Attack.png") },
	m_TulipSeed{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Seed.png") },
	m_TulipSeedExplosion{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Explosion.png") }, // ~25 mb memory?
	m_MushroomIdle{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Idle.png") },
	m_MushroomAttack{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Attack.png") },
	m_MushroomPop{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Pop.png") },
	m_MushroomBoil{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Boil.png") },
	m_MushroomDeath{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Death.png") },
	m_MushroomCloud{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Cloud.png") },
	m_AcornIdle{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Fly.png") },
	m_AcornDrop{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Drop.png") }
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

	m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 2800.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 2800.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 2800.f, 450.f }, 600.f));

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 3850.f, 280.f }));
	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 1000.f, 170.f }, 5, 4, m_Camera.GetCuurentCameraBounds().width / 2));

	m_EnemyManager.AddEnemy(new Mushroom(m_MushroomIdle, m_MushroomBoil, m_MushroomAttack, m_MushroomPop, m_MushroomDeath, m_MushroomCloud, Vector2f{ 2200.f, 170.f }, 
		1, 1, m_Camera.GetCuurentCameraBounds().width / 2.5f));

	m_Vertices.reserve(20);
	m_Vertices.push_back(Vector2f{ 50.f, 170.f });
	m_Vertices.push_back(Vector2f{ 50.f, GetViewPort().height + 200.f });
	m_Vertices.push_back(Vector2f{ 11800.f, GetViewPort().height + 200.f });
	m_Vertices.push_back(Vector2f{ 11800.f, 400.f });

	m_Vertices.push_back(Vector2f{ 10000.f, 420.f });

	m_Vertices.push_back(Vector2f{ 7400.f, 230.f });
	m_Vertices.push_back(Vector2f{ 6700.f, 500.f });

	m_Vertices.push_back(Vector2f{ 4000.f, 380.f });
	m_Vertices.push_back(Vector2f{ 4000.f, 0.f });

	m_Vertices.push_back(Vector2f{ 4000.f, 190.f });
	m_Vertices.push_back(Vector2f{ 4000.f, 280.f });
	m_Vertices.push_back(Vector2f{ 3700.f, 280.f });
	m_Vertices.push_back(Vector2f{ 3700.f, 190.f });

	m_Vertices.push_back(Vector2f{ 3200.f, 170.f });
	m_Vertices.push_back(Vector2f{ 3200.f, 270.f });
	m_Vertices.push_back(Vector2f{ 3100.f, 270.f });
	m_Vertices.push_back(Vector2f{ 3100.f, 170.f });
}

void Game::Cleanup( )
{
	delete m_ForestBackground1;
	m_ForestBackground1 = nullptr;

	delete m_ForectBackground2;
	m_ForectBackground2 = nullptr;

	delete m_HealthSprite;
	m_HealthSprite = nullptr;
	delete m_CardsSprite;
	m_CardsSprite = nullptr;

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

	m_Vertices.clear();
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	// updates cuphead and pushes projectiles to player BulletManager
	m_Cuphead.Update(elapsedSec, pStates, m_Vertices, m_PlayerBulletManager, m_UIManager);

	// update enemies and enemy bullets if player is alive
	if (m_Cuphead.GetHealth() > 0) 
	{
		SpawnAcorns(elapsedSec);

		// update enemies and pushes projectiles to enemy BulletManager
		m_EnemyManager.UpdateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_EnemyBulletManager, m_Cuphead, m_UIManager);
	
		m_EnemyBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices, m_Cuphead, m_UIManager);

		// follow player if alive
		m_Camera.Aim(12400.f, 760.f, m_Cuphead.GetPosition());
	}
	else
	{
		// unfollow if player dead
		m_Camera.Aim(12400.f, 760.f, m_Cuphead.GetPlaceOfDeath());
		
	}

	// animate enemy bullets even if player is dead
	m_EnemyBulletManager.AnimateActiveBullets(elapsedSec);

	// animate enemies even if player is dead
	m_EnemyManager.AnimateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds());
	
	// update and animate even if player is dead
	m_PlayerBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices, m_Cuphead, m_UIManager);

	m_PlayerBulletManager.AnimateActiveBullets(elapsedSec);

	// delete player bullets if they collide with enemy and deal damage
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

						if (m_PlayerBulletManager[i]->Damage() - 0.3f > 0.001f)
						{
							m_PlayerBulletManager.RemoveProjectile(i);
						}
					}
				}
			}
		}
	}

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

	m_UIManager.Update(elapsedSec, m_Cuphead.GetHealth());
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();
		glScalef(0.66f, 0.67f, 0.f);
		m_ForestBackground1->Draw();
		m_ForectBackground2->Draw(Rectf{m_ForestBackground1->GetWidth(), 0.f, m_ForectBackground2->GetWidth(), m_ForectBackground2->GetHeight()});
	glPopMatrix();

	m_Cuphead.Draw();

	m_Camera.DrawBorderOverlay(GetViewPort().height / 20);

	// Hitbox
	utils::SetColor(Color4f{ 1,0,1,1 });
	utils::DrawPolygon(m_Vertices, true, 2.f);

	m_PlayerBulletManager.DrawActiveBullets();
	m_EnemyBulletManager.DrawActiveBullets();

	m_EnemyManager.DrawEnemies(m_Camera.GetCuurentCameraBounds());

	m_UIManager.Draw(m_Camera.GetCuurentCameraBounds());

	m_Camera.Reset();

	if (m_Cuphead.GetHealth() <= 0)
	{
		utils::SetColor(Color4f{ 0, 0, 0, 0.5f });
		utils::FillRect(GetViewPort());
	}
	
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

void Game::SpawnAcorns(float elapsedSec)
{
	static bool allowedSpawn{ false };

	if (m_Cuphead.GetPosition().x >= 4000.f)
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
					Vector2f{ m_Camera.GetCuurentCameraBounds().left - m_Camera.GetCuurentCameraBounds().width * 0.2f, 
					m_Camera.GetCuurentCameraBounds().bottom + m_Camera.GetCuurentCameraBounds().height * 0.9f},
					Vector2f{ 1, 0 }));
			}
			else
			{
				m_EnemyManager.AddEnemy(new Acorn(m_AcornIdle, m_AcornDrop,
					Vector2f{ m_Camera.GetCuurentCameraBounds().left + m_Camera.GetCuurentCameraBounds().width * 1.2f, 
					m_Camera.GetCuurentCameraBounds().bottom + m_Camera.GetCuurentCameraBounds().height * 0.9f},
					Vector2f{ -1, 0 }));
			}

			accuSec -= 5.f;
		}
	}
}
