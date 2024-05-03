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
	m_ptrPlayer = new ScottPilgrim(Point2f{ GetViewPort().left, GetViewPort().height / 4.f }, m_PLAYER_WIDTH, m_PLAYER_HEIGHT);

	m_ptrMap = new Texture("Level1_Sprite.png");

	m_ptrCamera = new Camera(GetViewPort().width, GetViewPort().height);

	m_ptrEnemies.push_back(new EnemyMike(Point2f{ 1200.f, GetViewPort().height / 4.f + 200.f}, 300.f, 225.f));
	m_ptrEnemies.push_back(new EnemyMike(Point2f{ 1200.f, GetViewPort().height / 4.f - 100.f }, 300.f, 225.f));
	//m_ptrEnemies.push_back(new EnemyMike(Point2f{ 1000.f, GetViewPort().height / 4.f + 100.f }, 300.f, 225.f));

	m_ptrWallet = new Wallet(70.f);
}

void Game::Cleanup( )
{
	delete m_ptrPlayer;
	m_ptrPlayer = nullptr;
	delete m_ptrMap;
	m_ptrMap = nullptr;
	delete m_ptrCamera;
	m_ptrCamera = nullptr;
	delete m_ptrWallet;
	m_ptrWallet = nullptr;
	
	for (EnemyMike* Enemies : m_ptrEnemies)
	{
		delete Enemies;
		Enemies = nullptr;
	}

	for(Coins* Money: m_ptrCoins)
	{
		delete Money;
		Money = nullptr;
	}
}

void Game::Update( float elapsedSec )
{
	PlayeKeys(elapsedSec);

	m_ptrPlayer->Update(elapsedSec);
	
	//Enemies Update
	int EnemyNumber{ 0 };

	for (EnemyMike* Enemies : m_ptrEnemies)
	{
		Enemies->Update(elapsedSec, m_ptrPlayer->GetPosition());

		const float PLAYER_VS_ENEMY_Y_DISTANCE{ 30.f };

		if (m_ptrPlayer->CheckIfAttackBoxIsOn() && (m_ptrPlayer->GetPosition().y >= Enemies->GetPosition().y - PLAYER_VS_ENEMY_Y_DISTANCE && m_ptrPlayer->GetPosition().y <= Enemies->GetPosition().y + PLAYER_VS_ENEMY_Y_DISTANCE))
		{
			//std::cout << "Scott is Hitting" << std::endl;
			if (m_PlayerLightAttacked)
			{
				//std::cout << "Scott is Hitting" << std::endl;
				if (m_PlayerUppercutAttack)
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()), false, false, true);
				}
				else
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()));
				}
				m_ptrPlayer->LightAttackCounterIncrement(Enemies->GetIsDamaged());

			}
			else if (m_PlayerHeavyAttacked)
			{
				if (m_ptrPlayer->GetHeavyAttackCounter() == 1)
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()), false, true);
				}
				else
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()));
				}

				m_ptrPlayer->HeavyAttackCounterIncrement(Enemies->GetIsDamaged());
			}

			if (Enemies == m_ptrEnemies.back())
			{
			m_PlayerLightAttacked = false;
			m_PlayerUppercutAttack = false;
			m_PlayerHeavyAttacked = false;
			}
		}

		if (Enemies->CheckIfAttackBoxIsOn() && (m_ptrPlayer->GetPosition().y >= Enemies->GetPosition().y - PLAYER_VS_ENEMY_Y_DISTANCE && m_ptrPlayer->GetPosition().y <= Enemies->GetPosition().y + PLAYER_VS_ENEMY_Y_DISTANCE))
		{
			//std::cout << "Enemy is Hitting" << std::endl;
			if (Enemies->m_EnemyStatus == EnemyMike::Status::LightAttack) m_ptrPlayer->CheckHit(std::vector<Point2f>(Enemies->GetAttackBox()), Enemies->GetIsLeft());
			else if (Enemies->m_EnemyStatus == EnemyMike::Status::SpinKick) m_ptrPlayer->CheckHit(std::vector<Point2f>(Enemies->GetAttackBox()), Enemies->GetIsLeft(), false, true);
		}

		if (Enemies->CheckIdle())
		{
			if (m_ptrPlayer->GetPosition().x > Enemies->GetPosition().x) Enemies->SetIsLeft(false);
			else Enemies->SetIsLeft(true);
			Enemies->m_EnemyStatus = EnemyMike::Status::Idle;
		}
		else if(Enemies->GetSpawnCoins())
		{
			m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), 80.f, Coins::Type::Cents5));
			m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), 80.f, Coins::Type::Cents10));
			m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), 80.f, Coins::Type::Cents25));


			delete Enemies;
			m_ptrEnemies.erase(m_ptrEnemies.begin() + EnemyNumber);
		}
		++EnemyNumber;
	}

	// Coins Update

	int CoinNumber{ 0 };

	for (Coins* Money : m_ptrCoins)
	{

		Money->Update(elapsedSec);

		utils::HitInfo m_Hitinfo;

		if(utils::Raycast(m_ptrPlayer->GetHitbox(), Money->GetHitbox()[0], Money->GetHitbox()[1], m_Hitinfo))
		{
			if(Money->m_CoinType == Coins::Type::Dollar1 || Money->m_CoinType == Coins::Type::Dollars2)
			{
				m_AboveDecimalPoint += Money->GetValue();
			}
			else 
			{
				m_BelowDecimalPoint += Money->GetValue();

				if(m_BelowDecimalPoint > 100)
				{
					m_BelowDecimalPoint -= 100;
					++m_AboveDecimalPoint;
				}
			}

			m_DoShowWallet = true;
			m_ShowWalletCounter = 0.f;

			delete Money;
			m_ptrCoins.erase(m_ptrCoins.begin() + CoinNumber);

		}

		++CoinNumber;
	}

	if (m_DoShowWallet)
	{
		m_ShowWalletCounter += elapsedSec;
		ShowWallet();
		m_ptrWallet->Update(elapsedSec, Point2f{ m_ptrPlayer->GetPosition().x - 50.f, m_ptrPlayer->GetPosition().y + m_PLAYER_HEIGHT / 4.f * 3.f }, m_AboveDecimalPoint, m_BelowDecimalPoint);
	}
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

	//Coins 
	for (Coins* Money : m_ptrCoins)
	{
		Money->Draw();
	}

	//Draw Player and Enemies depending on Y-axis
	QuicksortDraw();

	//Draw Wallet
	if (m_DoShowWallet)
	{
		m_ptrWallet->Draw();
	}

	//Camera
	m_ptrCamera->Reset();

}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{

	// Attack Moves and Jump
	if(e.keysym.sym == SDLK_j && m_PlayerResetLightAttackButton)
	{
		if ( m_ptrPlayer->CheckIdle())
		{
			for (EnemyMike* Enemies : m_ptrEnemies)
			{
				if ((Enemies->GetHealth() == 1 || Enemies->GetGotLightHitAmount() == 3 )&& m_ptrPlayer->GetIsJumping() == false && m_PlayerUppercutAttack == false)
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()), true);
					if (Enemies->GetIsColliding())
					{
						m_ptrPlayer->Attack(false, false, false, true);
						m_PlayerUppercutAttack = true;
					}
				}
			}
			if(m_PlayerUppercutAttack == false) m_ptrPlayer->Attack(true);
			m_PlayerLightAttacked = true;
			m_PlayerResetLightAttackButton = false;
		}
		else if (m_ptrPlayer->GetIsJumping())
		{
			m_ptrPlayer->Attack(false, false, true);
			m_PlayerLightAttacked = true;
		}

	}
	else if(e.keysym.sym == SDLK_k && m_PlayerResetHeavyAttackButton)
	{
		if( m_ptrPlayer->CheckIdle())
		{
			m_ptrPlayer->Attack(false, true);
			m_PlayerHeavyAttacked = true;
			m_PlayerResetHeavyAttackButton = false;
		}
	}
	else if (e.keysym.sym == SDLK_l && (m_ptrPlayer->CheckIdle() || m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::Block))
	{
		m_ptrPlayer->Block();
	}

	if (e.keysym.sym == SDLK_SPACE && m_ptrPlayer->CheckIdle())
	{
		if (m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::MovingLeft || m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::MovingRight) m_ptrPlayer->Jump(true);
		else if (m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::RunningLeft || m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::RunningRight) m_ptrPlayer->Jump(true, true);
		else m_ptrPlayer->Jump();
	}

	if((e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_q || e.keysym.sym == SDLK_LEFT || e.keysym.sym == SDLK_RIGHT) && m_PlayerResetRunRight && m_PlayerResetRunLeft)
	{
		if (m_ptrPlayer->GetIsRunningTrigger() == true && m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::Idle)
		{
			if (e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_RIGHT) m_ptrPlayer->m_ScottStatus = ScottPilgrim::Status::RunningRight;
			else if (e.keysym.sym == SDLK_q || e.keysym.sym == SDLK_LEFT)m_ptrPlayer->m_ScottStatus = ScottPilgrim::Status::RunningLeft;
		}
		else
		{
			if (e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_RIGHT) m_PlayerResetRunRight = false;
			else if (e.keysym.sym == SDLK_q || e.keysym.sym == SDLK_LEFT) m_PlayerResetRunLeft = false;
			
			m_ptrPlayer->SetIsRunningTrigger(true);
			//std::cout << "Running trigger is true" << std::endl;
		}
	}
}


void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//Reset Keys
	if (e.keysym.sym == SDLK_j) m_PlayerResetLightAttackButton = true;

	if (e.keysym.sym == SDLK_k) m_PlayerResetHeavyAttackButton = true;

	if (e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_RIGHT) m_PlayerResetRunRight = true;

	if (e.keysym.sym == SDLK_q || e.keysym.sym == SDLK_LEFT) m_PlayerResetRunLeft = true;

	if (e.keysym.sym == SDLK_l && (m_ptrPlayer->CheckIdle() || m_ptrPlayer->m_ScottStatus == ScottPilgrim::Status::Block)) m_ptrPlayer->Block(true);

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

	if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_D] && pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_RIGHT] && pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_D] && pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, true);
	}
	else if (pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false);
	}
	else if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true);
	}
	else if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, true);
	}
	else if (m_ptrPlayer->CheckIdle())
	{
		m_ptrPlayer->m_ScottStatus = ScottPilgrim::Status::Idle;
	}
}

void Game::QuicksortDraw() const
{
	int AmountOfEnemiesAbovePlayerYPosition{};
	std::vector<EnemyMike*> m_ptrEnemiesAbovePlayer{};

	for (EnemyMike* Enemies : m_ptrEnemies)
	{
		if (Enemies->GetPosition().y >= m_ptrPlayer->GetPosition().y)
		{
			m_ptrEnemiesAbovePlayer.push_back(Enemies);
			++AmountOfEnemiesAbovePlayerYPosition;
		}
	}

	if (AmountOfEnemiesAbovePlayerYPosition == 2)
	{
		if (m_ptrEnemiesAbovePlayer[0]->GetPosition().y > m_ptrEnemiesAbovePlayer[1]->GetPosition().y)
		{
			m_ptrEnemiesAbovePlayer[0]->Draw();
			m_ptrEnemiesAbovePlayer[1]->Draw();
		}
		else
		{
			m_ptrEnemiesAbovePlayer[1]->Draw();
			m_ptrEnemiesAbovePlayer[0]->Draw();
		}
	}
	else if (AmountOfEnemiesAbovePlayerYPosition == 1) m_ptrEnemiesAbovePlayer[0]->Draw();

	m_ptrPlayer->Draw();

	int AmountOfEnemiesBelowPlayerYPosition{};
	std::vector<EnemyMike*> m_ptrEnemiesBelowPlayer{};

	for (EnemyMike* Enemies : m_ptrEnemies)
	{
		if (Enemies->GetPosition().y < m_ptrPlayer->GetPosition().y)
		{
			m_ptrEnemiesBelowPlayer.push_back(Enemies);
			++AmountOfEnemiesBelowPlayerYPosition;
		}
	}

	if (AmountOfEnemiesBelowPlayerYPosition == 2)
	{
		if (m_ptrEnemiesBelowPlayer[0]->GetPosition().y > m_ptrEnemiesBelowPlayer[1]->GetPosition().y)
		{
			m_ptrEnemiesBelowPlayer[0]->Draw();
			m_ptrEnemiesBelowPlayer[1]->Draw();
		}
		else
		{
			m_ptrEnemiesBelowPlayer[1]->Draw();
			m_ptrEnemiesBelowPlayer[0]->Draw();
		}
	}
	else if (AmountOfEnemiesBelowPlayerYPosition == 1) m_ptrEnemiesBelowPlayer[0]->Draw();
	
}

void Game::ShowWallet()
{
	if(m_ShowWalletCounter >= m_MAX_SHOW_WALLET_DELAY)
	{
		m_DoShowWallet = false;
		m_ShowWalletCounter -= m_MAX_SHOW_WALLET_DELAY;
	}
}
