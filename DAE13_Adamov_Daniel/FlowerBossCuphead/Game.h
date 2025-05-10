#pragma once
#include "BaseGame.h"
#include <vector>
#include "Cuphead.h"
class Texture;
#include "Camera.h"
#include "EnemyManager.h"
#include "BulletManager.h"

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
	std::vector<Vector2f> m_Vertices;
	Texture* m_ForestBackground1;
	Texture* m_ForectBackground2;

	Camera m_Camera;

	EnemyManager m_EnemyManager;
	BulletManager m_PlayerBulletManager;
	BulletManager m_EnemyBulletManager;

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

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void SpawnAcorns(float elapsedSec);
};