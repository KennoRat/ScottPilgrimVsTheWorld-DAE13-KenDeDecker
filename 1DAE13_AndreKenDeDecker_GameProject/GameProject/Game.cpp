#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_ptrPlayer = new ScottPilgrim(Point2f{ GetViewPort().left, GetViewPort().height / 4.f }, 300.f, 225.f); // 150.f, 225.f

	m_ptrMap = new Texture("Level1_Sprite.png");

	m_ptrCamera = new Camera(GetViewPort().width, GetViewPort().height);

	m_ptrTestEnemy = new EnemyMike(Point2f{ 1000.f, GetViewPort().height / 4.f }, 300.f, 225.f);
}

void Game::Cleanup( )
{
	delete m_ptrPlayer;
	m_ptrPlayer = nullptr;
	delete m_ptrMap;
	m_ptrMap = nullptr;
	delete m_ptrCamera;
	m_ptrCamera = nullptr;
	
	delete m_ptrTestEnemy;
	m_ptrTestEnemy = nullptr;
}

void Game::Update( float elapsedSec )
{
	PlayeKeys(elapsedSec);

	/*m_ptrPlayer->CheckKeys(elapsedSec, true);*/
	m_ptrPlayer->Update(elapsedSec);
	

	m_ptrTestEnemy->Update(elapsedSec);

	if(m_ptrPlayer->CheckIfHitboxIsOn())
	{
		m_ptrTestEnemy->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()));
		if (m_PlayerAttacked)
		{
			m_ptrPlayer->LightAttackCounter(m_ptrTestEnemy->GetIsDamaged());
			m_PlayerAttacked = false;
		}
	}

	if(m_ptrTestEnemy->CheckIdle())
	{
		m_ptrTestEnemy->m_EnemyStatus = EnemyMike::Status::Idle;
	}

	//std::cout << "X Position: " << m_ptrPlayer->GetPosition().x << std::endl;
	//std::cout << "Y Position: " << m_ptrPlayer->GetPosition().y << std::endl;
}

void Game::Draw( ) const
{
	ClearBackground( );

	Rectf dstRectfMap{ 0.f, 0.f, m_ptrMap->GetWidth() * 3.3f, GetViewPort().height * 1.3f };
	Rectf srcRectMap{ 10.f, -63.f, m_ptrMap->GetWidth(), 280.f };

	//Camera
	m_ptrCamera->Aim(dstRectfMap.width, dstRectfMap.height, Point2f{ m_ptrPlayer->GetPosition().x + m_ptrPlayer->GetWidth() / 2.f , m_ptrPlayer->GetPosition().y });

	//Draw Map
	m_ptrMap->Draw(dstRectfMap, srcRectMap);

	if(m_ptrPlayer->GetPosition().y - 20.f <= m_ptrTestEnemy->GetPosition().y)
	{
		//Draw Enemy
		m_ptrTestEnemy->Draw();

		//Draw Player
		m_ptrPlayer->Draw();
	}
	else
	{
		//Draw Player
		m_ptrPlayer->Draw();

		//Draw Enemy
		m_ptrTestEnemy->Draw();
	}

	

	m_ptrCamera->Reset();
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_j)
	{
		m_ptrPlayer->Attack(true);
		m_PlayerAttacked = true;
	}
}


void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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

void Game::PlayeKeys(float elapsedSec)
{
	 /*Check keyboard state*/
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	/*if(pStates[SDL_SCANCODE_J])
	{
		m_ptrPlayer->Attack(true);
		m_ptrPlayer->LightAttackCounter(m_ptrTestEnemy->GetIsDamaged());
	}*/
	if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_DOWN])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, true);
	}
	else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_DOWN])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false);
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true);
	}
	else if (pStates[SDL_SCANCODE_DOWN])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_UP])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, true);
	}
	else if (m_ptrPlayer->CheckIdle())
	{
		m_ptrPlayer->m_ScottStatus = ScottPilgrim::Status::Idle;
	}
}
