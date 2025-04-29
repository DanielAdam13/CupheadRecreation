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
	m_Cuphead{ Vector2f{GetViewPort().width / 2, 100.f}, true },
	m_Vertices{},
	m_ForestBackground1{ new Texture("ForestFollies_Background_First.png") },
	m_ForectBackground2{ new Texture("ForestFollies_Background_Second.png") },
	m_PlayerCamera{ GetViewPort().width, GetViewPort().height },
	m_EnemyManager{},
	m_BulletManager{}
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

	m_Cuphead.Update(elapsedSec, pStates, m_Vertices, m_BulletManager);

	m_EnemyManager.UpdateEnemies(elapsedSec, m_Cuphead);

	m_PlayerCamera.Aim(12400.f, 760.f, m_Cuphead.GetPosition());

	m_BulletManager.UpdateActiveBullets(elapsedSec, m_PlayerCamera);

	for (int i{}; i < m_BulletManager.GetVectorSize(); ++i)
	{
		if (m_BulletManager[i] != nullptr)
		{
			for (int j{}; j < m_EnemyManager.GetVectorSize(); ++j)
			{
				if (utils::IsOverlapping(m_EnemyManager[j]->GetBounds(), m_BulletManager[i]->GetBounds()))
				{
					m_BulletManager.RemoveProjectile(i);
				}
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

	m_PlayerCamera.DrawBorderOverlay(GetViewPort().height / 20);

	// Hitbox
	utils::SetColor(Color4f{ 1,0,1,1 });
	//utils::DrawPolygon(m_Vertices, true, 2.f);

	m_BulletManager.DrawActiveBullets();
	m_EnemyManager.DrawEnemies();

	m_PlayerCamera.Reset();
	
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
		m_Cuphead.Parry();
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	/*switch ( e.keysym.sym )
	{
	case SDLK_LSHIFT:
		m_Cuphead.StartDash();
		break;
	}*/
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
