#pragma once
#include "BaseGame.h"
#include <vector>
#include "Cuphead.h"
class Texture;
#include "Camera.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "VisualEffectManager.h"
#include "UIManager.h"
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
	Texture* m_ForestBackground1;
	Texture* m_ForestBackground2;

	const float m_LevelWidthWorld;
	const float m_LevelHeightWorld;
	const float m_ScaleX;
	const float m_ScaleY;

	Camera m_Camera;
	Vector2f m_TransformedPlayerPos;

	const Texture* m_EnemyDeathVFXSprite;
	EnemyManager m_EnemyManager;
	BulletManager m_PlayerBulletManager;
	BulletManager m_EnemyBulletManager;
	VisualEffectManager m_VFXManager;

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

	const Texture* m_HealthSprite;
	const Texture* m_CardsSprite;
	const Texture* m_IntroAnnouncementTexture;
	const Texture* m_DeathAnnouncementTexture;
	const Texture* m_BravoAnnouncementTexture;
	const Texture* m_PauseScreen;
	const Texture* m_DeathCardScreen;
	UIManager m_UIManager;
	
	const Texture* m_PeaShooterSprite;
	const Texture* m_PeaSpecialSprite;
	const Texture* m_PeaDeathVFX;

	const Texture* m_SpikeSprite;
	const Texture* m_ChomperSprite;

	const Texture* m_TulipIdle;
	const Texture* m_TulipAttack;
	const Texture* m_TulipSeed;
	const Texture* m_TulipSeedExplosion;

	const Texture* m_MushroomIdle;
	const Texture* m_MushroomAttack;
	const Texture* m_MushroomPop;
	const Texture* m_MushroomBoil;
	const Texture* m_MushroomDeath;
	const Texture* m_MushroomCloud;

	const Texture* m_AcornIdle;
	const Texture* m_AcornDrop;

	SoundStream* m_ForesFolliestSoundtrack;
	SoundEffect* m_IntroAnnouncementAudio;
	const SoundEffect* m_BravoAnnouncementAudio;

	const SoundEffect* m_MushroomShootSFX1;
	const SoundEffect* m_MushroomShootSFX2;
	const SoundEffect* m_TulipShootSFX;
	const SoundEffect* m_TulipSeedSFX;
	SoundEffect* m_AcornIdleSFX;
	SoundEffect* m_AcornFallSFX;
	const SoundEffect* m_ChomperBiteSFX1;
	const SoundEffect* m_ChomperBiteSFX2;
	
	Cuphead m_Cuphead;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void InitalizeEnemies();

	void ManagePlayerProjectiles();
	void ManageTakingDamageCuphead();
	
	void DrawScaledObjects() const;

	void SpawnAcorns(float elapsedSec);
	void RestartLevel();
	
	void HandleGameStateLogic(float elapsedSec);
	void UpdateForestFolliesLevel(float elapsedSec);

	void DeleteSound();
	void DeleteTextures();
};