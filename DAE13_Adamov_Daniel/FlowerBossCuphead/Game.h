#pragma once
#include "BaseGame.h"
#include <vector>
#include "Cuphead.h"
class Texture;
#include "Camera.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "UIManager.h"

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

	Camera m_Camera;

	EnemyManager m_EnemyManager;
	BulletManager m_PlayerBulletManager;
	BulletManager m_EnemyBulletManager;

	enum class GameState
	{
		intro,
		gameplay,
		pause,
		death,
		win
	};
	GameState m_CurrentGameState;

	const Texture* m_HealthSprite;
	const Texture* m_CardsSprite;
	const Texture* m_IntroAnnouncment;
	const Texture* m_DeathAnnouncment;
	const Texture* m_BravoAnnouncment;
	const Texture* m_PauseScreen;
	UIManager m_UIManager;
	

	const Texture* m_PeaShooterSprite;
	const Texture* m_PeaSpecialSprite;
	Cuphead m_Cuphead;

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

	const float m_ScaledLevelWidth;
	const float m_ScaledLevelHeight;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void ManagePlayerProjectiles();
	void ManageTakingDamageCuphead();
	
	void DrawScaledObjects() const;

	void SpawnAcorns(float elapsedSec);
};