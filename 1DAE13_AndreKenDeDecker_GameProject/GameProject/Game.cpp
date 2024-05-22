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

	//Sound
	m_ptrLevel1Music = new SoundStream("Sounds/AnotherWinter(Mixed).mp3");
	m_ptrSoundEffects = new SoundEffects();

	m_ptrPlayer = new ScottPilgrim(Point2f{ GetViewPort().left, GetViewPort().height / 4.f }, m_PLAYER_WIDTH, m_PLAYER_HEIGHT, m_ptrSoundEffects);

	m_ptrMap = new Texture("Level1_Sprite.png");

	SVGParser::GetVerticesFromSvgFile("Level1_Sprite.svg", m_MapSvg);
	SVGParser::GetVerticesFromSvgFile("CameraBorder.svg", m_CameraSvg);

	m_ptrCamera = new Camera(GetViewPort().width, GetViewPort().height, m_ptrPlayer->GetPosition());

	m_ptrPlayerUI = new PlayerUI(70.f);

	m_ptrObjects.push_back(new Objects(Point2f{ 1000.f, GetViewPort().height / 4.f + 200.f }, 150.f, 100.f, m_ptrSoundEffects));
	m_ptrObjects.push_back(new Objects(Point2f{ 700.f, GetViewPort().height / 4.f + 200.f }, 150.f, 100.f, m_ptrSoundEffects));

	//Fight Areas
	m_FightAreasXPosition.push_back(1050.f);
	m_FightAreasXPosition.push_back(2860.f);
	m_FightAreasXPosition.push_back(20000.f);

	//Map Bounds //Transform Map Collisions

	Matrix2x3 TranslationMat{};
	TranslationMat.SetAsTranslate(Vector2f{ -50.f, -210.f });
	Matrix2x3 ScaleMat{};
	ScaleMat.SetAsScale(3.3f, 3.3f);
	Matrix2x3 TransformMat{ TranslationMat * ScaleMat };
	m_TransformedMapSvg = TransformMat.Transform(m_MapSvg[0]);

	m_TranformedBordersSvg.push_back(m_TransformedMapSvg);
}

void Game::Cleanup( )
{
	delete m_ptrPlayer;
	m_ptrPlayer = nullptr;
	delete m_ptrMap;
	m_ptrMap = nullptr;
	delete m_ptrCamera;
	m_ptrCamera = nullptr;
	delete m_ptrPlayerUI;
	m_ptrPlayerUI = nullptr;
	delete m_ptrLevel1Music;
	m_ptrLevel1Music = nullptr;
	delete m_ptrSoundEffects;
	m_ptrSoundEffects = nullptr;
	
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

	for(DamageNumbers* Damage: m_ptrDamageNumbers)
	{
		delete Damage;
		Damage = nullptr;
	}

	for(Objects* Objects: m_ptrObjects)
	{
		delete Objects;
		Objects = nullptr;
	}

}

void Game::Update(float elapsedSec)
{
	//Player Update
	PlayerKeys(elapsedSec);

	m_ptrPlayer->Update(elapsedSec, m_TranformedBordersSvg);

	if (m_ptrPlayer->CheckIdle())
	{
		m_PlayerLightAttacked = false;
		m_PlayerUppercutAttack = false;
		m_PlayerHeavyAttacked = false;
	}

	//Objects Update
	for (Objects* Objects : m_ptrObjects)
	{
		float yPosition{ m_PLAYER_HEIGHT / 5.f * 3.f };
		const float PLAYER_OBJECTS_Y_DISTANCE{ 30.f };
		if (Objects->GetIsPickedUp() && Objects->GetPlayer() == m_ptrPlayer)
		{
			float xPosition{ 10.f };
			Objects->Update(elapsedSec, Point2f{ m_ptrPlayer->GetPosition().x - xPosition , m_ptrPlayer->GetPosition().y + yPosition }, m_ptrPlayer->GetIsLeft(), m_TranformedBordersSvg);

			if (m_ptrPlayer->GetIsDamaged())
			{
				Objects->DroppedObject(m_ptrPlayer->GetPosition().y);
			}

			if (m_ptrPlayer->GetThrowObject()) Objects->ThrownObject(m_ptrPlayer->GetPosition().y);

			if (Objects->GetIsFlipped() != m_ptrPlayer->GetFlipBox()) Objects->SetIsFlipped(m_ptrPlayer->GetFlipBox());

			if (Objects->GetRumble() != m_ptrPlayer->GetObjectRumble()) Objects->SetRumble(m_ptrPlayer->GetObjectRumble());

		}
		else if(Objects->GetIsPickedUp() == false)
		{
			Objects->Update(elapsedSec, m_TranformedBordersSvg);

			if (m_ptrPlayer->CheckIfAttackBoxIsOn())
			{
				if ((m_ptrPlayer->GetPosition().y >= Objects->GetPosition().y - PLAYER_OBJECTS_Y_DISTANCE && m_ptrPlayer->GetPosition().y <= Objects->GetPosition().y + PLAYER_OBJECTS_Y_DISTANCE)
					&& Objects->CheckIfOverlapping(m_ptrPlayer->GetAttackBox()))
				{
					if (m_PlayerHeavyAttacked || (m_PlayerLightAttacked && m_ptrPlayer->GetHasPickedUpAnObject() == true))
					{
						Objects->Collision(m_ptrPlayer->GetAttackBox(), m_ptrPlayer->GetIsLeft());
					}
					else if (m_PlayerLightAttacked && m_ptrPlayer->GetHasPickedUpAnObject() == false)
					{
						Objects->PickUpPlayer(m_ptrPlayer->GetAttackBox(), m_ptrPlayer);
						m_ptrPlayer->HasPickedUpObject(Objects->GetIsPickedUp(), Objects);
						m_PlayerLightAttacked = false;
						m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::RecycleHit);
					}
				}
			}
		}

		if (m_ptrPlayer->GetHealth() > 0 && Objects->GetDoDamage() && (Objects->GetFallYPosition() >= m_ptrPlayer->GetPosition().y - yPosition && Objects->GetFallYPosition() <= m_ptrPlayer->GetPosition().y + yPosition)
			 && Objects->GetEnemy() != nullptr)
		{
			const Point2f DAMAGE_POSITION_PLAYER{ Point2f{ m_ptrPlayer->GetPosition().x + m_PLAYER_WIDTH / 4.f, m_ptrPlayer->GetPosition().y + m_PLAYER_HEIGHT / 3.f * 2.f} };
			m_ptrPlayer->CheckHit(Objects->GetHitbox(), Objects->GetIsLeft(), 6);
			if (m_ptrPlayer->GetIsDamaged())
			{
				Objects->ObjectHit(true);
				m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION_PLAYER, m_DAMAGE_SIZE, 6));
			}
			else if (m_ptrPlayer->GetIsHitWhileBlocking()) Objects->ObjectHit(true);
		}

		for (EnemyMike* Enemies : m_ptrEnemies)
		{
			if(Objects->GetIsPickedUp() && Objects->GetEnemy() == Enemies)
			{
				float xPosition{ 10.f };
				float yPosition{ m_ENEMY_HEIGHT / 3.f * 2.f };
				Objects->Update(elapsedSec, Point2f{ Enemies->GetPosition().x - xPosition , Enemies->GetPosition().y + yPosition }, Enemies->GetIsLeft(), m_TranformedBordersSvg);

				if (Enemies->GetIsDamaged())
				{
					Objects->DroppedObject(Enemies->GetPosition().y);
				}

				if (Enemies->GetThrowObject()) Objects->ThrownObject(Enemies->GetPosition().y);

				if (Objects->GetIsFlipped() != Enemies->GetFlipBox()) Objects->SetIsFlipped(Enemies->GetFlipBox());

				if (Objects->GetRumble() != Enemies->GetObjectRumble()) Objects->SetRumble(Enemies->GetObjectRumble());
			}
			else if(Objects->GetIsPickedUp() == false && Objects->GetPlayer() == nullptr)
			{
				if (Enemies->CheckIfAttackBoxIsOn())
				{
					const float ENEMY_OBJECTS_Y_DISTANCE{ 30.f };

					if (Enemies->GetIsPickingUp() && (Enemies->GetPosition().y >= Objects->GetPosition().y - ENEMY_OBJECTS_Y_DISTANCE && Enemies->GetPosition().y <= Objects->GetPosition().y + ENEMY_OBJECTS_Y_DISTANCE))
					{
						Objects->PickUpEnemy(Enemies->GetAttackBox(), Enemies);
						Enemies->HasPickedUpObject(Objects->GetIsPickedUp(), Objects);
					}
				}
			}
		}
	}

	//Play No enemies Hit effect
	if (m_ptrPlayer->CheckIfAttackBoxIsOn() && m_PlayerHasHitAnEnemy == false && m_PlayerLightAttacked && m_PlayNoHitEffect)
	{
		m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::HitAir);
		m_PlayNoHitEffect = false;
	}


	//Enemies Update

	for (int EnemyIndex{ 0 }; EnemyIndex < m_ptrEnemies.size(); ++EnemyIndex)
	{
		EnemyMike* Enemies = m_ptrEnemies[EnemyIndex];

		bool ObjectFound{ false };
		for (Objects* Objects : m_ptrObjects)
		{
			if(Objects->GetPosition().x + 250.f > Enemies->GetPosition().x && Objects->GetPosition().x - 250.f < Enemies->GetPosition().x && Objects->GetIsPickedUp() == false)
			{
				Enemies->Update(elapsedSec, m_ptrPlayer->GetPosition(), m_TranformedBordersSvg, Objects->GetPosition(), Objects->GetIsLeft());
				ObjectFound = true;
				break;
			}
		}
		if(ObjectFound == false) Enemies->Update(elapsedSec, m_ptrPlayer->GetPosition(), m_TranformedBordersSvg);

		const float PLAYER_VS_ENEMY_Y_DISTANCE{ 30.f };
		const Point2f DAMAGE_POSITION{ Point2f{ Enemies->GetPosition().x + m_ENEMY_WIDTH / 4.f, Enemies->GetPosition().y + m_ENEMY_HEIGHT } };

		//See if player is on the same Y position to hit enemies
		if (m_ptrPlayer->CheckIfAttackBoxIsOn() && (m_ptrPlayer->GetPosition().y >= Enemies->GetPosition().y - PLAYER_VS_ENEMY_Y_DISTANCE && m_ptrPlayer->GetPosition().y <= Enemies->GetPosition().y + PLAYER_VS_ENEMY_Y_DISTANCE))
		{
			if (m_PlayerLightAttacked)
			{
				if (m_PlayerUppercutAttack)
				{
					Enemies->CheckHit(m_ptrPlayer->GetAttackBox(), 6, false, false, true);
					if (Enemies->GetIsHit())
					{
						if(m_PlayerHasHitAnEnemy == false)
						{
							m_ptrPlayer->LightAttackCounterIncrement(true);
							m_PlayerHasHitAnEnemy = true;
						}
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 6));
					}
				}
				else
				{
					if (m_ptrPlayer->GetHasPickedUpAnObject()) Enemies->CheckHit(m_ptrPlayer->GetAttackBox(), 7);
					else Enemies->CheckHit(m_ptrPlayer->GetAttackBox(), 2);

					if (Enemies->GetIsHit())
					{
						if(m_ptrPlayer->GetHasPickedUpAnObject()) m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 7));
						else
						{
							if (m_PlayerHasHitAnEnemy == false)
							{
								m_ptrPlayer->LightAttackCounterIncrement(true);
								m_PlayerHasHitAnEnemy = true;
							}
							m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 2));
						}
					}
				}

			}
			else if (m_PlayerHeavyAttacked)
			{
				if (m_ptrPlayer->GetHeavyAttackCounter() == 1)
				{
					Enemies->CheckHit(m_ptrPlayer->GetAttackBox(), 5, false, true);
					if (Enemies->GetIsHit())
					{
						if (m_PlayerHasHitAnEnemy == false)
						{
							m_ptrPlayer->HeavyAttackCounterIncrement(true);
							m_PlayerHasHitAnEnemy = true;
						}
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 5));
					}
				}
				else
				{
					Enemies->CheckHit(m_ptrPlayer->GetAttackBox(), 4);
					if (Enemies->GetIsHit())
					{
						if (m_PlayerHasHitAnEnemy == false)
						{
							m_ptrPlayer->HeavyAttackCounterIncrement(true);
							m_PlayerHasHitAnEnemy = true;
						}
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 4));
					}
				}
			}

			if (Enemies->GetHealth() <= 0 && Enemies->GetIsHit() && (m_PlayerLightAttacked || m_PlayerHeavyAttacked))
			{
				Point2f DAMAGE_POSITION_KAPOW;
				if (Enemies->GetIsLeft()) DAMAGE_POSITION_KAPOW = Point2f{ Enemies->GetPosition().x - m_ENEMY_WIDTH / 5.f, Enemies->GetPosition().y + m_ENEMY_HEIGHT / 2.f };
				else DAMAGE_POSITION_KAPOW = Point2f{ Enemies->GetPosition().x + m_ENEMY_WIDTH / 5.f, Enemies->GetPosition().y + m_ENEMY_HEIGHT / 2.f };
				
				m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION_KAPOW, m_DAMAGE_SIZE, 0, true));
			}


			if (Enemies == m_ptrEnemies.back())
			{
				if (m_PlayerHasHitAnEnemy) m_PlayerHasHitAnEnemy = false;
				m_PlayerLightAttacked = false;
				m_PlayerUppercutAttack = false;
				m_PlayerHeavyAttacked = false;
			}

		}

		if (Enemies->CheckIfAttackBoxIsOn() && (m_ptrPlayer->GetPosition().y >= Enemies->GetPosition().y - PLAYER_VS_ENEMY_Y_DISTANCE && m_ptrPlayer->GetPosition().y <= Enemies->GetPosition().y + PLAYER_VS_ENEMY_Y_DISTANCE))
		{
			//std::cout << "Enemy is Hitting" << std::endl;
			const Point2f DAMAGE_POSITION_PLAYER{ Point2f{ m_ptrPlayer->GetPosition().x + m_PLAYER_WIDTH / 4.f, m_ptrPlayer->GetPosition().y + m_PLAYER_HEIGHT / 3.f * 2.f}};

			if(Enemies->GetHasPickedUp())
			{
				if(Enemies->m_EnemyStatus == EnemyMike::Status::PickUpAttack)
				{
					m_ptrPlayer->CheckHit(std::vector<Point2f>(Enemies->GetAttackBox()), Enemies->GetIsLeft(), 7);
					if (m_ptrPlayer->GetIsDamaged())
					{
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION_PLAYER, m_DAMAGE_SIZE, 7));
						m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::RecycleHit);
					}
				}
			}
			else
			{
				if (Enemies->m_EnemyStatus == EnemyMike::Status::LightAttack)
				{
					m_ptrPlayer->CheckHit(std::vector<Point2f>(Enemies->GetAttackBox()), Enemies->GetIsLeft(), 2);
					if (m_ptrPlayer->GetIsDamaged())
					{
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION_PLAYER, m_DAMAGE_SIZE, 2));
					}
				}
				else if (Enemies->m_EnemyStatus == EnemyMike::Status::SpinKick)
				{
					m_ptrPlayer->CheckHit(std::vector<Point2f>(Enemies->GetAttackBox()), Enemies->GetIsLeft(), 6, false, true);
					if (m_ptrPlayer->GetIsDamaged())
					{
						m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION_PLAYER, m_DAMAGE_SIZE, 6));
					}
				}
			}
		}

		if (Enemies->CheckIdle())
		{
			if (m_ptrPlayer->GetPosition().x > Enemies->GetPosition().x) Enemies->SetIsLeft(false);
			else Enemies->SetIsLeft(true);
			if(Enemies->GetHasPickedUp()) Enemies->m_EnemyStatus = EnemyMike::Status::PickUpIdle;
			else Enemies->m_EnemyStatus = EnemyMike::Status::Idle;
		}

		for (Objects* Objects : m_ptrObjects)
		{
			if (Enemies->GetHealth() > 0 && Objects->GetDoDamage() && (Objects->GetFallYPosition() >= Enemies->GetPosition().y - PLAYER_VS_ENEMY_Y_DISTANCE && Objects->GetFallYPosition() <= Enemies->GetPosition().y + PLAYER_VS_ENEMY_Y_DISTANCE)
				&& Objects->GetEnemy() != Enemies)
			{
				Enemies->CheckHit(Objects->GetHitbox(), 6, false, false, false, true);
				if (Enemies->GetIsHit())
				{
					Objects->ObjectHit(true);
					m_ptrDamageNumbers.push_back(new DamageNumbers(DAMAGE_POSITION, m_DAMAGE_SIZE, 6));
					m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::RecycleHit);
				}
				else if (Enemies->GetIsBlocking()) Objects->ObjectHit(true);
			}
		}

		if (Enemies->GetSpawnCoins())
		{
			m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::CoinSpawn);

			if(Enemies->GetEnemyType() == "Lee" || Enemies->GetEnemyType() == "Luke")
			{
				m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), m_COINS_SIZE, Coins::Type::Cents25));
				m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), m_COINS_SIZE, Coins::Type::Cents25));
			}
			else
			{
				m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), m_COINS_SIZE, Coins::Type::Cents5));
				m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), m_COINS_SIZE, Coins::Type::Cents10));
				m_ptrCoins.push_back(new Coins(Enemies->GetPosition(), m_COINS_SIZE, Coins::Type::Cents25));
			}

			delete Enemies;
			m_ptrEnemies.erase(m_ptrEnemies.begin() + EnemyIndex);

			--EnemyIndex;
		}
	}

	// Coins Update

	for (Coins* Money : m_ptrCoins)
	{
		Money->Update(elapsedSec);
	}

	for (int CoinIndex{ 0 }; CoinIndex < m_ptrCoins.size(); ++CoinIndex)
	{
		Coins* Money = m_ptrCoins[CoinIndex];

		utils::HitInfo m_Hitinfo;

		if (utils::Raycast(m_ptrPlayer->GetHitbox(), Money->GetHitbox()[0], Money->GetHitbox()[1], m_Hitinfo))
		{
			m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::CoinPickUp);

			if (Money->m_CoinType == Coins::Type::Dollar1 || Money->m_CoinType == Coins::Type::Dollars2)
			{
				m_AboveDecimalPoint += Money->GetValue();
			}
			else
			{
				m_BelowDecimalPoint += Money->GetValue();

				if (m_BelowDecimalPoint > 100)
				{
					m_BelowDecimalPoint -= 100;
					++m_AboveDecimalPoint;
				}
			}

			m_DoShowWallet = true;
			m_ShowWalletCounter = 0.f;

			delete Money;
			m_ptrCoins.erase(m_ptrCoins.begin() + CoinIndex);

			--CoinIndex;

		}

	}

	// PlayerUI Update
	if (m_DoShowWallet)
	{
		m_ShowWalletCounter += elapsedSec;
		ShowWallet();
		m_ptrPlayerUI->Update(elapsedSec, Point2f{ m_ptrPlayer->GetPosition().x - 50.f, m_ptrPlayer->GetPosition().y + m_PLAYER_HEIGHT / 4.f * 3.f }, m_AboveDecimalPoint, m_BelowDecimalPoint);
	}
	m_ptrPlayerUI->SetPlayerHealth(m_ptrPlayer->GetHealth());

	// DamageNumbers Update
	for (DamageNumbers* Damage : m_ptrDamageNumbers)
	{
		Damage->Update(elapsedSec);
	}

	for(int DamageIndex{}; DamageIndex < m_ptrDamageNumbers.size(); ++DamageIndex)
	{
		DamageNumbers* Damage = m_ptrDamageNumbers[DamageIndex];
		if(Damage->GetIsDone())
		{
			delete Damage;
			m_ptrDamageNumbers.erase(m_ptrDamageNumbers.begin() + DamageIndex);

			--DamageIndex;
		}
	}

	//Fight Area
	FightingAreaEvent();

	//Camera Update
	m_ptrCamera->Update(elapsedSec);

	//Sound
	if(m_BackgroundMusicOn == false)
	{
		m_ptrLevel1Music->SetVolume(m_BackgroundVolume);
		//m_ptrLevel1Music->Play(true);
		m_BackgroundMusicOn = true;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	Rectf dstRectfMap{ 0.f, 0.f, m_ptrMap->GetWidth() * 3.3f, GetViewPort().height * 1.3f };
	Rectf srcRectMap{ 10.f, -63.f, m_ptrMap->GetWidth(), 280.f };



	//Camera
	if(m_StopCamera)  m_ptrCamera->Aim(dstRectfMap.width, dstRectfMap.height, Point2f{ m_StopCameraXPosition , m_ptrPlayer->GetPosition().y });
	else m_ptrCamera->Aim(dstRectfMap.width, dstRectfMap.height, Point2f{ m_ptrPlayer->GetPosition().x + m_ptrPlayer->GetWidth() / 2.f , m_ptrPlayer->GetPosition().y });

	//Draw Map
	m_ptrMap->Draw(dstRectfMap, srcRectMap);
	utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
	utils::DrawPolygon(m_TransformedMapSvg);
	utils::DrawPolygon(m_TransformedCameraSvg);


	//Coins 
	for (Coins* Money : m_ptrCoins)
	{
		Money->Draw();
	}

	//Objects
	for (Objects* Objects : m_ptrObjects)
	{
		if(Objects->GetIsPickedUp() == false) Objects->Draw();
	}

	//Draw Player and Enemies depending on Y-axis
	SortDraw();

	//Draw PlayerUI
	if (m_DoShowWallet)
	{
		m_ptrPlayerUI->Draw();
	}

	//Draw Damage
	for (DamageNumbers* Damage : m_ptrDamageNumbers)
	{
		Damage->Draw();
	}

	//Camera
	m_ptrCamera->Reset();
	
	//Draw PlayerUI
	m_ptrPlayerUI->DrawUI();
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
				if ((Enemies->GetHealth() <= 2 || Enemies->GetGotLightHitAmount() == 3 ) && m_ptrPlayer->GetIsJumping() == false && m_PlayerUppercutAttack == false && m_ptrPlayer->GetHasPickedUpAnObject() == false)
				{
					Enemies->CheckHit(std::vector<Point2f>(m_ptrPlayer->GetAttackBox()), 0, true);
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
			m_PlayNoHitEffect = true;
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

	if(e.keysym.sym == SDLK_p)
	{
		std::cout << "XPosition: " << m_ptrPlayer->GetPosition().x << std::endl;
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

void Game::PlayerKeys(float elapsedSec)
{
	 /*Check keyboard state*/
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_D] && pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_D] && pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, true);
	}
	else if (pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true, false, true);
	}
	else if (pStates[SDL_SCANCODE_D])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, true, false);
	}
	else if (pStates[SDL_SCANCODE_A])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, true);
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, false, true);
	}
	else if (pStates[SDL_SCANCODE_W])
	{
		m_ptrPlayer->CheckKeys(elapsedSec, false, false, true);
	}
	else if (m_ptrPlayer->CheckIdle())
	{
		m_ptrPlayer->m_ScottStatus = ScottPilgrim::Status::Idle;
	}
}

void Game::SortDraw() const
{
	int AmountOfEnemiesAbovePlayerYPosition{};
	std::vector<EnemyMike*> m_ptrEnemiesAbovePlayer{};

	int AmountOfEnemiesBelowPlayerYPosition{};
	std::vector<EnemyMike*> m_ptrEnemiesBelowPlayer{};

	for (EnemyMike* Enemies : m_ptrEnemies)
	{
		if (Enemies->GetPosition().y >= m_ptrPlayer->GetPosition().y)
		{
			m_ptrEnemiesAbovePlayer.push_back(Enemies);
			++AmountOfEnemiesAbovePlayerYPosition;
		}
		if (Enemies->GetPosition().y < m_ptrPlayer->GetPosition().y)
		{
			m_ptrEnemiesBelowPlayer.push_back(Enemies);
			++AmountOfEnemiesBelowPlayerYPosition;
		}
	}

	SortYPosition(AmountOfEnemiesAbovePlayerYPosition, m_ptrEnemiesAbovePlayer);

	if(m_ptrPlayer->GetHoldingObject() != nullptr)
	{
		m_ptrPlayer->GetHoldingObject()->Draw();
	}
	m_ptrPlayer->Draw();

	SortYPosition(AmountOfEnemiesBelowPlayerYPosition, m_ptrEnemiesBelowPlayer);
}

void Game::SortYPosition(int Amount, const std::vector<EnemyMike*>& ptrEnemies) const
{
	if (Amount == 3)
	{
		int AmountOfEnemiesAbove{};
		std::vector<EnemyMike*> m_ptrEnemiesAbove{};
		int AmountOfEnemiesBelow{};
		std::vector<EnemyMike*> m_ptrEnemiesBelow{};

		for(int EnemiesIndex{1}; EnemiesIndex < 3; ++EnemiesIndex)
		{
			if (ptrEnemies[EnemiesIndex]->GetPosition().y >= ptrEnemies[0]->GetPosition().y)
			{
				m_ptrEnemiesAbove.push_back(ptrEnemies[EnemiesIndex]);
				++AmountOfEnemiesAbove;
			}
			if (ptrEnemies[EnemiesIndex]->GetPosition().y < ptrEnemies[0]->GetPosition().y)
			{
				m_ptrEnemiesBelow.push_back(ptrEnemies[EnemiesIndex]);
				++AmountOfEnemiesBelow;
			}
		}

		SortYPosition(AmountOfEnemiesAbove, m_ptrEnemiesAbove);
		ptrEnemies[0]->Draw();
		SortYPosition(AmountOfEnemiesBelow, m_ptrEnemiesBelow);

	}
	else if (Amount == 2)
	{
		if (ptrEnemies[0]->GetPosition().y > ptrEnemies[1]->GetPosition().y)
		{
			SortDrawObject(ptrEnemies[0]);
			SortDrawObject(ptrEnemies[1]);
		}
		else
		{
			SortDrawObject(ptrEnemies[1]);
			SortDrawObject(ptrEnemies[0]);
		}
	}
	else if (Amount == 1) SortDrawObject(ptrEnemies[0]);
}

void Game::SortDrawObject(const EnemyMike* Enemy) const
{
	if(Enemy->GetHoldingObject() != nullptr)
	{
		Enemy->GetHoldingObject()->Draw();
	}
	Enemy->Draw();
}

void Game::ShowWallet()
{
	if(m_ShowWalletCounter >= m_MAX_SHOW_WALLET_DELAY)
	{
		m_DoShowWallet = false;
		m_ShowWalletCounter -= m_MAX_SHOW_WALLET_DELAY;
	}
}

void Game::FightingAreaEvent()
{
	if(m_ptrPlayer->GetPosition().x >= m_FightAreasXPosition[0] && m_StopCamera == false)
	{
		m_StopCamera = true;
		float EnemiesSpawnPosition = m_FightAreasXPosition[0] + GetViewPort().width / 3.f * 2.f;
		if(m_FightEvents == 0)
		{
			m_ptrEnemies.push_back(new EnemyMike(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f + 100.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
			m_ptrEnemies.push_back(new EnemyLee(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f - 100.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
			m_ptrEnemies.push_back(new EnemyMike(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f + 200.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
		}
		else if(m_FightEvents == 1)
		{
			m_ptrEnemies.push_back(new EnemyLuke(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f + 100.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
			m_ptrEnemies.push_back(new EnemyLee(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f - 100.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
			m_ptrEnemies.push_back(new EnemyMike(Point2f{ EnemiesSpawnPosition, GetViewPort().height / 4.f + 200.f }, m_ENEMY_WIDTH, m_ENEMY_HEIGHT, m_ptrSoundEffects));
		}
		++m_FightEvents;

		m_StopCameraXPosition = m_FightAreasXPosition[0] + m_ptrPlayer->GetWidth() / 2.f;
		m_FightAreasXPosition.erase(m_FightAreasXPosition.begin());

		//Transform Camera Collisions
		Matrix2x3 TranslationMat{};
		TranslationMat.SetAsTranslate(Vector2f{ m_StopCameraXPosition - GetViewPort().width / 2.f, 640.f });
		m_TransformedCameraSvg = TranslationMat.Transform(m_CameraSvg[0]);

		m_TranformedBordersSvg.push_back(m_TransformedCameraSvg);
	}

	if(m_StopCamera && m_ptrEnemies.empty())
	{
		m_StopCamera = false;
		m_TranformedBordersSvg.pop_back();
	}
}
