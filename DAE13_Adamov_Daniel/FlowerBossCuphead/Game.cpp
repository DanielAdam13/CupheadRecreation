#include "pch.h"
#include "Game.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"
#include "Camera.h"
#include "Enemy.h"
#include "Projectile.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Cuphead{ Vector2f{GetViewPort().width / 2, 100.f}, true, 3 },
	m_Vertices{},
	m_ForestBackground1{ new Texture("ForestFollies_Background_First.png") },
	m_ForectBackground2{ new Texture("ForestFollies_Background_Second.png") },
	m_Camera{ GetViewPort().width, GetViewPort().height },
	m_EnemyManager{},
	m_PlayerBulletManager{},
	m_EnemyBulletManager{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
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
	m_Vertices.push_back(Vector2f{ 4000.f, 310.f });
	m_Vertices.push_back(Vector2f{ 3700.f, 310.f });
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

	m_Vertices.clear();
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	// updates cuphead and pushes projectiles to player BulletManager
	m_Cuphead.Update(elapsedSec, pStates, m_Vertices, m_PlayerBulletManager);

	// update enemies and enemy bullets if player is alive
	if (m_Cuphead.GetHealth() > 0) 
	{
		// update enemies and pushes projectiles to enemy BulletManager
		m_EnemyManager.UpdateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_EnemyBulletManager, m_Cuphead);
	
		m_EnemyBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices);

		// follow player if alive
		m_Camera.Aim(12400.f, 760.f, m_Cuphead.GetPosition());
	}
	else
	{
		// unfollow if player dead
		m_Camera.Aim(12400.f, 760.f, m_Cuphead.GetPlaceOfDeath());
		
	}
	
	// update and animate even if player is dead
	m_PlayerBulletManager.UpdateActiveBullets(elapsedSec, m_Camera.GetCuurentCameraBounds(), m_Vertices);
	m_PlayerBulletManager.AnimateActiveBullets(elapsedSec);

	// animate enemy bullets even if player is dead
	m_EnemyBulletManager.AnimateActiveBullets(elapsedSec);
	
	// animate enemies even if player is dead
	m_EnemyManager.AnimateEnemies(elapsedSec, m_Camera.GetCuurentCameraBounds());


	// delete player bullets if they collide with enemy
	for (int j{}; j < m_EnemyManager.GetVectorSize(); ++j)
	{
		if (m_EnemyManager[j] != nullptr)
		{
			for (int i{}; i < m_PlayerBulletManager.GetVectorSize(); ++i)
			{
				if (m_PlayerBulletManager[i] != nullptr)
				{
					if (utils::IsOverlapping(m_EnemyManager[j]->GetBounds(), m_PlayerBulletManager[i]->GetBounds()))
					{
						m_PlayerBulletManager.RemoveProjectile(i);
					}
				}
			}
		}
	}

	// deal damage to cuphead if player collides with enemy
	for (int i{}; i < m_EnemyManager.GetVectorSize(); ++i)
	{
		if (m_EnemyManager[i] != nullptr)
		{
			if (utils::IsOverlapping(m_EnemyManager[i]->GetBounds(), m_Cuphead.GetBounds()) && m_Cuphead.GetHealth() > 0)
			{
				m_Cuphead.Hit();
			}
		}
	}
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
