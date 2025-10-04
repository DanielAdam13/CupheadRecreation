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
#include "SoundStream.h"
#include "SoundEffect.h"
#include "Effect.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Vertices{},
	m_ForestBackground1{ std::make_unique<Texture>("ForestFollies_Background_First_Smaller.png") },
	m_ForestBackground2{ std::make_unique<Texture>("ForestFollies_Background_Second_Smaller.png") },
	m_LevelWidthWorld{ GetViewPort().width * 9.95f },
	m_LevelHeightWorld{ GetViewPort().height * 1.29f },
	m_ScaleX{ GetViewPort().width / (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth()) * 9.95f },
	m_ScaleY{ GetViewPort().height / m_ForestBackground1->GetHeight() * 1.29f },
	m_Camera{ GetViewPort().width, GetViewPort().height, m_ScaleX, m_ScaleY },
	m_TransformedPlayerPos{},
	m_EnemyManager{ m_EnemyDeathVFXSprite },
	m_PlayerBulletManager{},
	m_EnemyBulletManager{},
	m_VFXManager{},
	m_CoinManager{ m_CoinTexture, m_CoinPickupSFX },
	m_CurrentGameState{ GameState::intro },
	m_ShouldSpawnAcorns{ false },
	m_HealthSprite{ new Texture("UI/Health_Stages.png") },
	m_CardsSprite{ new Texture("UI/Cards_Stages.png") },
	m_IntroAnnouncementTexture{ new Texture("UI/Stages_Intro.png") }, // + ~60 mb memory
	m_DeathAnnouncementTexture{ new Texture("UI/YouDied_Stages.png") }, // + ~45 mb memory
	m_BravoAnnouncementTexture{ new Texture("UI/Bravo_Sprite.png") }, // + 200+ mb !!!???
	m_PauseScreen{ new Texture("UI/Pause_Screen.bmp") }, // 20 mb
	m_DeathCardScreen{ new Texture("UI/Death_Screen_UI.png") },
	m_ProgressMan{ new Texture("UI/RedMen.png") },
	m_UIManager{ m_HealthSprite, m_CardsSprite, m_IntroAnnouncementTexture, m_DeathAnnouncementTexture, m_BravoAnnouncementTexture, m_PauseScreen, m_DeathCardScreen, m_ProgressMan },
	m_PeaShooterSprite{ std::make_unique<Texture>("Projectile_Loop.png") },
	m_PeaSpecialSprite{ std::make_unique<Texture>("Projectile_Special_Loop.png") },
	m_PeaDeathVFX{ new Texture("PeaShot_Death.png") },
	m_SpikeSprite{ new Texture("Run'N'Gun/Sprite_Spike.png") },
	m_ChomperSprite{ new Texture("Run'N'Gun/Sprite_Chomper.png") },
	m_TulipIdle{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Idle.png") },
	m_TulipAttack{ new Texture("Run'N'Gun/Tulip/Sprite_Tulip_Attack.png") },
	m_TulipSeed{ std::make_unique<Texture>("Run'N'Gun/Tulip/Sprite_Tulip_Seed.png") },
	m_TulipSeedExplosion{ std::make_unique<Texture>("Run'N'Gun/Tulip/Sprite_Tulip_Explosion.png") },
	m_MushroomIdle{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Idle.png") },
	m_MushroomAttack{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Attack.png") },
	m_MushroomPop{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Pop.png") },
	m_MushroomBoil{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Boil.png") },
	m_MushroomDeath{ new Texture("Run'N'Gun/Mushroom/Sprite_Mushroom_Death.png") },
	m_MushroomCloud{ std::make_unique<Texture>("Run'N'Gun/Mushroom/Sprite_Mushroom_Cloud.png") },
	m_AcornIdle{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Fly.png") },
	m_AcornDrop{ new Texture("Run'N'Gun/Acorn/Sprite_Acorn_Drop.png") },
	/*m_DaisyRun{ new Texture("Run'N'Gun/Daisy/Sprite_Daisy_Run.png") },
	m_DaisyJump{ new Texture("Run'N'Gun/Daisy/Sprite_Daisy_Jump.png") },
	m_DaisyTurn{ new Texture("Run'N'Gun/Daisy/Sprite_Daisy_Turn.png") },*/
	m_EnemyDeathVFXSprite{ new Texture("VFX_1.png") },
	m_CoinTexture{ std::make_unique<Texture>("Coin_Stages.png") },
	m_ForesFolliestSoundtrack{ std::make_unique<SoundStream>("Audio/Forest Follies.mp3") },
	m_IntroAnnouncementAudio{ std::make_unique<SoundEffect>("Audio/Intro.mp3") },
	m_BravoAnnouncementAudio{ std::make_unique<SoundEffect>("Audio/Bravo.wav") },
	m_Cuphead{ Vector2f{ 640.f, 140.f}, (m_CurrentGameState == GameState::intro), 3,
	m_PeaShooterSprite, m_PeaSpecialSprite },
	m_MushroomShootSFX1{ new SoundEffect("Audio/Enemies/Mushroom Shoot 1.wav") },
	m_MushroomShootSFX2{ new SoundEffect("Audio/Enemies/Mushroom Shoot 2.wav") },
	m_TulipShootSFX{ new SoundEffect("Audio/Enemies/Tulip Shoot 1.wav") },
	m_TulipSeedSFX{ new SoundEffect("Audio/Enemies/Tulip Shoot 2.wav") },
	m_AcornIdleSFX{ new SoundEffect("Audio/Enemies/Acorn Idle.wav") },
	m_AcornFallSFX{ new SoundEffect("Audio/Enemies/Acorn Fall.wav") },
	m_ChomperBiteSFX1{ new SoundEffect("Audio/Enemies/Chomper Bite 1.wav") },
	m_ChomperBiteSFX2{ new SoundEffect("Audio/Enemies/Chomper Bite 2.wav") },
	m_CoinPickupSFX{ std::make_unique<SoundEffect>("Audio/Coin_Pickup.wav") }
{
	Initialize(); 
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	InitializeEnemies();
	InitializeCoins();

	SVGParser::GetVerticesFromSvgFile("ForestFollies_Background_Finished_Smaller_3.svg", m_Vertices);

	if (m_CurrentGameState == GameState::intro)
	{
		m_IntroAnnouncementAudio->Play(0);
	}
}

void Game::Cleanup()
{
	DeleteTextures();

	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		m_Vertices[i].clear();
	}
	m_Vertices.clear();

	DeleteSounds();
}

void Game::Update( float elapsedSec )
{
	HandleGameStateLogic(elapsedSec); // handles the GameState enum class and transitions between enum states

	UpdateForestFolliesLevel(elapsedSec); // updates the whole level - enemies, player, bullets
}

void Game::Draw( ) const
{
	ClearBackground( );
	
	m_Camera.Apply(); // everything is offset by camera
	
	DrawScaledObjects(); // scaling all objects by screen size

	m_Camera.DrawBorderOverlay(GetViewPort().height / 20);
	m_Camera.Reset(); // resetting the camera matrix each frame
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
		if (m_CurrentGameState == GameState::death)
		{
			m_ForesFolliestSoundtrack->Stop();
			RestartLevel();
		}
		else if (m_CurrentGameState == GameState::pause)
		{
			m_CurrentGameState = GameState::gameplay;
		}
		else m_Cuphead.ToggleParryState();
		break;
	case SDLK_ESCAPE:
		if (m_CurrentGameState == GameState::gameplay)
		{
			m_AcornFallSFX->PauseAll();
			m_CurrentGameState = GameState::pause;
		}
		else if (m_CurrentGameState == GameState::pause)
		{
			m_AcornFallSFX->ResumeAll();
			m_CurrentGameState = GameState::gameplay;
		}
		break;
	case SDLK_m:
		m_ForesFolliestSoundtrack->Stop(); // mute
		break;
	case SDLK_r:
		if (m_CurrentGameState != GameState::intro)
		{
			m_ForesFolliestSoundtrack->Stop();
			RestartLevel();
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

void Game::InitializeEnemies()
{
	/*m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 1700.f, -(m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 1700.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 1700.f, 500.f }, 600.f, m_ChomperBiteSFX1, m_ChomperBiteSFX2));*/

	m_EnemyManager.AddEnemy(new Mushroom(m_MushroomIdle, m_MushroomBoil, m_MushroomAttack, m_MushroomPop, m_MushroomDeath, m_MushroomCloud,
		Vector2f{ 2800.f, 130.f }, m_MushroomShootSFX1, m_MushroomShootSFX2,
		1, 1, m_Camera.GetCuurentCameraBounds().width / 2.5f));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 3190.f, 350.f }, Vector2f{ 3190.f, 250.f }, Vector2f{ 3190.f, 450.f }, 250.f));

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 3850.f, 280.f }, m_TulipShootSFX, m_TulipSeedSFX, 5, 4, GetViewPort().width * 0.7f));

	m_EnemyManager.AddEnemy(new Mushroom(m_MushroomIdle, m_MushroomBoil, m_MushroomAttack, m_MushroomPop, m_MushroomDeath, m_MushroomCloud,
		Vector2f{ 5050.f, 420.f }, m_MushroomShootSFX1, m_MushroomShootSFX2,
		1, 1, m_Camera.GetCuurentCameraBounds().width / 2.5f));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 5700.f, 650.f }, Vector2f{ 5700.f, 550.f }, Vector2f{ 5700.f, 750.f }, 250.f));

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 6500.f, 430.f }, m_TulipShootSFX, m_TulipSeedSFX, 5, 4, GetViewPort().width * 0.7f));

	m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 7000.f, -(m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 7000.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 7000.f, 550.f }, 600.f, m_ChomperBiteSFX1, m_ChomperBiteSFX2));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 7650.f, 350.f }, Vector2f{ 7650.f, 250.f }, Vector2f{ 7650.f, 450.f }, 250.f));

	m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 8200.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 8200.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 8200.f, 550.f }, 600.f, m_ChomperBiteSFX1, m_ChomperBiteSFX2));

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 9000.f, 90.f }, m_TulipShootSFX, m_TulipSeedSFX, 5, 4, GetViewPort().width * 0.7f));

	m_EnemyManager.AddEnemy(new Spike(m_SpikeSprite, Vector2f{ 10190.f, 580.f }, Vector2f{ 10190.f, 480.f }, Vector2f{ 10190.f, 680.f }, 250.f));

	m_EnemyManager.AddEnemy(new BigChomper(m_ChomperSprite, Vector2f{ 10200.f, (m_ChomperSprite->GetHeight() / 4) / 2 },
		Vector2f{ 10200.f, -(m_ChomperSprite->GetHeight() / 4) * 1.5f }, Vector2f{ 10200.f, 550.f }, 600.f, m_ChomperBiteSFX1, m_ChomperBiteSFX2));

	m_EnemyManager.AddEnemy(new Mushroom(m_MushroomIdle, m_MushroomBoil, m_MushroomAttack, m_MushroomPop, m_MushroomDeath, m_MushroomCloud,
		Vector2f{ 11150.f, 450.f }, m_MushroomShootSFX1, m_MushroomShootSFX2,
		1, 1, m_Camera.GetCuurentCameraBounds().width / 2.5f));

	m_EnemyManager.AddEnemy(new Tulip(m_TulipIdle, m_TulipAttack, m_TulipSeed, m_TulipSeedExplosion, Vector2f{ 11450.f, 330.f }, m_TulipShootSFX, m_TulipSeedSFX, 5, 4, GetViewPort().width * 0.7f));
}

void Game::InitializeCoins()
{
	m_CoinManager.AddCoin(Vector2f{ 2620.f, 570.f });
	m_CoinManager.AddCoin(Vector2f{ 6650.f, 670.f });
	m_CoinManager.AddCoin(Vector2f{ 10190.f, 770.f });
}

void Game::ManagePlayerProjectiles(float elapsedSec)
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
						m_EnemyManager[j]->TakeDamage(elapsedSec, m_PlayerBulletManager[i]->Damage(), m_UIManager);

						if (m_PlayerBulletManager[i]->Damage() - 1.2f > 0.001f)
						{
							m_VFXManager.AddEffect(new Effect(m_PlayerBulletManager[i]->GetHitbox().center, m_PeaDeathVFX, 6, 1, 0.5f));
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

	glScalef(m_ScaleX, // x ratio
		m_ScaleY, // y ratio
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

	m_CoinManager.DrawCoins(m_Camera.GetCuurentCameraBounds());
	m_VFXManager.DrawEffects();

	m_UIManager.Draw(m_Camera.GetCuurentCameraBounds());
	glPopMatrix();
}

void Game::SpawnAcorns(float elapsedSec)
{
	if (!m_ShouldSpawnAcorns && m_Cuphead.GetPosition().x >= (m_ForestBackground1->GetWidth() + m_ForestBackground2->GetWidth()) / 3)
	{
		m_ShouldSpawnAcorns = true; // stays on
	}

	if (m_ShouldSpawnAcorns)
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
					Vector2f{ 1, 0 }, m_AcornIdleSFX, m_AcornFallSFX));
			}
			else // from right
			{
				m_EnemyManager.AddEnemy(new Acorn(m_AcornIdle, m_AcornDrop,
					Vector2f{ m_Camera.GetCuurentCameraBounds().left + m_Camera.GetCuurentCameraBounds().width * 1.05f, 
					m_Camera.GetCuurentCameraBounds().bottom + m_Camera.GetCuurentCameraBounds().height * 0.9f},
					Vector2f{ -1, 0 }, m_AcornIdleSFX, m_AcornFallSFX));
			}

			accuSec -= 5.f;
		}
	}
}

void Game::RestartLevel()
{
	m_Cuphead.ResetPlayer(Vector2f{ GetViewPort().width / 2, GetViewPort().height * 6.5f / 36.f });
	m_EnemyManager.DeleteAllEnemies();
	m_CoinManager.DeleteAllCoins();
	Game::InitializeEnemies();
	Game::InitializeCoins();
	m_ShouldSpawnAcorns = false;
	m_UIManager.ResetUI();
	m_CurrentGameState = GameState::intro;
	m_IntroAnnouncementAudio->Play(0);
}

void Game::HandleGameStateLogic(float elapsedSec)
{
	static float m_GameplayAccuSec{ 0.f };
	static bool m_ShouldDecreaseVolume{ false };

	switch (m_CurrentGameState)
	{
	case Game::GameState::intro:
		m_GameplayAccuSec += elapsedSec;
		if (m_GameplayAccuSec >= 2.8f)
		{
			m_CurrentGameState = GameState::gameplay;
			m_ForesFolliestSoundtrack->SetVolume(60);
			m_ForesFolliestSoundtrack->Play(m_CurrentGameState == GameState::gameplay);
			m_GameplayAccuSec -= 2.8f;
		}
		break;
	case Game::GameState::gameplay:
		if (m_Cuphead.GetPosition().x >= 12000.f)
		{
			m_CurrentGameState = GameState::win;
		}
		break;
	case Game::GameState::death:
		if (!m_ShouldDecreaseVolume)
		{
			m_ForesFolliestSoundtrack->SetVolume(30);
			m_GameplayAccuSec += elapsedSec;
			if (m_GameplayAccuSec >= 1.f)
			{
				m_ForesFolliestSoundtrack->SetVolume(10);
				m_ShouldDecreaseVolume = true;
			}
		}
		break;
	case GameState::win:
		static bool canPlayBravoAudio{ true };

		if (canPlayBravoAudio)
		{
			m_BravoAnnouncementAudio->Play(0);
			canPlayBravoAudio = false;
		}

		if (m_ForesFolliestSoundtrack->GetVolume() != 0)
		{
			m_GameplayAccuSec += elapsedSec;
			if (m_GameplayAccuSec >= 0.08f)
			{
				//m_AcornIdleSFX->StopAll();
				m_ForesFolliestSoundtrack->SetVolume(m_ForesFolliestSoundtrack->GetVolume() - 2);
				m_GameplayAccuSec -= 0.08f;
			}
		}
		
		else
		{
			m_ForesFolliestSoundtrack->Stop();
			m_GameplayAccuSec += elapsedSec;

			if (m_GameplayAccuSec >= 3.f)
			{
				RestartLevel();
				canPlayBravoAudio = true;
				m_GameplayAccuSec -= 3.f;
			}
		}
	}
}

void Game::UpdateForestFolliesLevel(float elapsedSec)
{
	if (m_CurrentGameState != GameState::pause && m_CurrentGameState != GameState::win)
	{
		// Check keyboard state
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		// updates cuphead and pushes projectiles to player BulletManager
		m_Cuphead.Update(elapsedSec, pStates, m_Vertices, m_PlayerBulletManager, m_UIManager);

		// update enemies and enemy bullets if player is alive
		if (m_Cuphead.GetHealth() > 0)
		{
			m_ForesFolliestSoundtrack->SetVolume(60);
			SpawnAcorns(elapsedSec);

			// update enemies and pushes projectiles to enemy BulletManager
			m_EnemyManager.UpdateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_EnemyBulletManager, m_Cuphead, m_UIManager, m_VFXManager);

			m_EnemyBulletManager.UpdateActiveBullets(elapsedSec, Rectf{ m_Camera.GetCuurentCameraBounds().left, m_Camera.GetCuurentCameraBounds().bottom, m_Camera.GetCuurentCameraBounds().width * m_ScaleX, m_Camera.GetCuurentCameraBounds().height * m_ScaleY },
				m_Vertices, m_Cuphead, m_UIManager);

			// follow player if alive
			m_Camera.Aim(m_LevelWidthWorld, m_LevelHeightWorld, m_Cuphead.GetPosition());
		}
		else
		{
			m_CurrentGameState = GameState::death;

			// unfollow if player dead
			m_Camera.Aim(m_LevelWidthWorld, m_LevelHeightWorld, m_Cuphead.GetPlaceOfDeath());
		}

		// animate enemy bullets even if player is dead
		m_EnemyBulletManager.AnimateActiveBullets(elapsedSec);

		m_CoinManager.UpdateCoins(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Cuphead.GetHitbox());

		// animate enemies even if player is dead
		m_EnemyManager.AnimateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds());

		// update and animate even if player is dead
		m_PlayerBulletManager.UpdateActiveBullets(elapsedSec,
			Rectf{ m_Camera.GetCuurentCameraBounds().left, m_Camera.GetCuurentCameraBounds().bottom, m_Camera.GetCuurentCameraBounds().width * m_ScaleX, m_Camera.GetCuurentCameraBounds().height * m_ScaleY },
			m_Vertices, m_Cuphead, m_UIManager);

		m_PlayerBulletManager.AnimateActiveBullets(elapsedSec);

		// delete player bullets if they collide with enemy and deal damage
		ManagePlayerProjectiles(elapsedSec);

		ManageTakingDamageCuphead();

		m_VFXManager.UpdateEffects(elapsedSec);
	}
	else if (m_CurrentGameState == GameState::pause)
	{
		m_ForesFolliestSoundtrack->SetVolume(20);

		// follow player if paused
		m_Camera.Aim(m_LevelWidthWorld, m_LevelHeightWorld, m_Cuphead.GetPosition());
	}

	m_UIManager.Update(elapsedSec, m_Cuphead.GetHealth(), int(m_CurrentGameState));
}

void Game::DeleteSounds()
{
	delete m_MushroomShootSFX1;
	m_MushroomShootSFX1 = nullptr;
	delete m_MushroomShootSFX2;
	m_MushroomShootSFX2 = nullptr;
	delete m_TulipSeedSFX;
	m_TulipSeedSFX = nullptr;
	delete m_TulipShootSFX;
	m_TulipShootSFX = nullptr;
	delete m_AcornIdleSFX;
	m_AcornIdleSFX = nullptr;
	delete m_AcornFallSFX;
	m_AcornFallSFX = nullptr;
	delete m_ChomperBiteSFX1;
	m_ChomperBiteSFX1 = nullptr;
	delete m_ChomperBiteSFX2;
	m_ChomperBiteSFX2 = nullptr;
}

void Game::DeleteTextures()
{
	delete m_HealthSprite;
	m_HealthSprite = nullptr;
	delete m_CardsSprite;
	m_CardsSprite = nullptr;
	delete m_IntroAnnouncementTexture;
	m_IntroAnnouncementTexture = nullptr;
	delete m_DeathAnnouncementTexture;
	m_DeathAnnouncementTexture = nullptr;
	delete m_BravoAnnouncementTexture;
	m_BravoAnnouncementTexture = nullptr;
	delete m_PauseScreen;
	m_PauseScreen = nullptr;
	delete m_DeathCardScreen;
	m_DeathCardScreen = nullptr;
	delete m_ProgressMan;
	m_ProgressMan = nullptr;

	delete m_SpikeSprite;
	m_SpikeSprite = nullptr;

	delete m_ChomperSprite;
	m_ChomperSprite = nullptr;

	delete m_TulipAttack;
	m_TulipAttack = nullptr;
	delete m_TulipIdle;
	m_TulipIdle = nullptr;

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

	delete m_AcornIdle;
	m_AcornIdle = nullptr;
	delete m_AcornDrop;
	m_AcornDrop = nullptr;

	/*delete m_DaisyRun;
	m_DaisyRun = nullptr;
	delete m_DaisyJump;
	m_DaisyJump = nullptr;
	delete m_DaisyTurn;
	m_DaisyTurn = nullptr;*/

	delete m_EnemyDeathVFXSprite;
	m_EnemyDeathVFXSprite = nullptr;
	delete m_PeaDeathVFX;
	m_PeaDeathVFX = nullptr;
}
