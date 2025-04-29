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
	Cuphead m_Cuphead;
	std::vector<Vector2f> m_Vertices;
	Texture* m_ForestBackground1;
	Texture* m_ForectBackground2;

	Camera m_PlayerCamera;

	EnemyManager m_EnemyManager;
	BulletManager m_BulletManager;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
};