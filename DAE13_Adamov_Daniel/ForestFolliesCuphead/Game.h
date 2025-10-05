#pragma once
#include "BaseGame.h"
#include <vector>

#include<memory>

#include "Cuphead.h"
class Texture;
#include "Camera.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "VisualEffectManager.h"
#include "UIManager.h"
#include "CoinManager.h"
class SoundStream;
class SoundEffect;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// VARIABLES
	std::vector<std::vector<Vector2f>> m_Vertices;
	std::unique_ptr<Texture> m_ForestBackground1;
	std::unique_ptr<Texture> m_ForestBackground2;

	const float m_LevelWidthWorld;
	const float m_LevelHeightWorld;
	const float m_ScaleX;
	const float m_ScaleY;

	Camera m_Camera;
	Vector2f m_TransformedPlayerPos;

	const std::unique_ptr<Texture> m_EnemyDeathVFXSprite;

	const std::unique_ptr<Texture> m_CoinTexture;
	const std::unique_ptr<SoundEffect> m_CoinPickupSFX;

	EnemyManager m_EnemyManager;
	BulletManager m_PlayerBulletManager;
	BulletManager m_EnemyBulletManager;
	VisualEffectManager m_VFXManager;
	CoinManager m_CoinManager;

	enum class GameState
	{
		intro,
		gameplay,
		pause,
		death,
		win
	};
	GameState m_CurrentGameState;

	bool m_ShouldSpawnAcorns;

	const std::unique_ptr<Texture> m_HealthSprite;
	const std::unique_ptr<Texture> m_CardsSprite;
	const std::unique_ptr<Texture> m_IntroAnnouncementTexture;
	const std::unique_ptr<Texture> m_DeathAnnouncementTexture;
	const std::unique_ptr<Texture> m_BravoAnnouncementTexture;
	const std::unique_ptr<Texture> m_PauseScreen;
	const std::unique_ptr<Texture> m_DeathCardScreen;
	const std::unique_ptr<Texture> m_ProgressMan;
	UIManager m_UIManager;
	
	const std::unique_ptr<Texture> m_PeaShooterSprite;
	const std::unique_ptr<Texture> m_PeaSpecialSprite;
	const std::unique_ptr<Texture> m_PeaDeathVFX;

	const std::unique_ptr<Texture> m_SpikeSprite;
	const std::unique_ptr<Texture> m_ChomperSprite;

	const std::unique_ptr<Texture> m_TulipIdle;
	const std::unique_ptr<Texture> m_TulipAttack;
	const std::unique_ptr<Texture> m_TulipSeed;
	const std::unique_ptr<Texture> m_TulipSeedExplosion;

	const std::unique_ptr<Texture> m_MushroomIdle;
	const std::unique_ptr<Texture> m_MushroomAttack;
	const std::unique_ptr<Texture> m_MushroomPop;
	const std::unique_ptr<Texture> m_MushroomBoil;
	const std::unique_ptr<Texture> m_MushroomDeath;
	const std::unique_ptr<Texture> m_MushroomCloud;

	const std::unique_ptr<Texture> m_AcornIdle;
	const std::unique_ptr<Texture> m_AcornDrop;

	std::unique_ptr<SoundStream> m_ForesFolliestSoundtrack;
	std::unique_ptr<SoundEffect> m_IntroAnnouncementAudio;
	const std::unique_ptr<SoundEffect> m_BravoAnnouncementAudio;

	const std::unique_ptr<SoundEffect> m_MushroomShootSFX1;
	const std::unique_ptr<SoundEffect> m_MushroomShootSFX2;
	const std::unique_ptr<SoundEffect> m_TulipShootSFX;
	const std::unique_ptr<SoundEffect> m_TulipSeedSFX;
	std::unique_ptr<SoundEffect> m_AcornIdleSFX;
	std::unique_ptr<SoundEffect> m_AcornFallSFX;
	const std::unique_ptr<SoundEffect> m_ChomperBiteSFX1;
	const std::unique_ptr<SoundEffect> m_ChomperBiteSFX2;
	
	
	Cuphead m_Cuphead;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void InitializeEnemies();
	void InitializeCoins();

	void ManagePlayerProjectiles(float elapsedSec);
	void ManageTakingDamageCuphead();
	
	void DrawScaledObjects() const;

	void SpawnAcorns(float elapsedSec);

	void RestartLevel();
	
	void HandleGameStateLogic(float elapsedSec);
	void UpdateForestFolliesLevel(float elapsedSec);
};