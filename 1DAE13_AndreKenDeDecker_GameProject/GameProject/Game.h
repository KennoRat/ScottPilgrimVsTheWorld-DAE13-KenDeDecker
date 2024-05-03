#pragma once
#include "BaseGame.h"
#include "ScottPilgrim.h"
#include "EnemyMike.h"
#include "Camera.h"
#include "Coins.h"
#include "Wallet.h"
#include <iostream>

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

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void PlayeKeys(float elapsedSec);
	void QuicksortDraw() const;
	void ShowWallet();

	//Variables
	bool m_PlayerLightAttacked{false};
	bool m_PlayerResetLightAttackButton{false};
	bool m_PlayerUppercutAttack{ false };

	bool m_PlayerHeavyAttacked{ false };
	bool m_PlayerResetHeavyAttackButton{ false };

	bool m_PlayerResetRunLeft{true};
	bool m_PlayerResetRunRight{ true };

	bool m_DoShowWallet{ false };

	int m_AboveDecimalPoint{};
	int m_BelowDecimalPoint{};

	float m_PLAYER_HEIGHT{ 280.f };
	float m_PLAYER_WIDTH{ 320.f };

	ScottPilgrim* m_ptrPlayer;
	Texture* m_ptrMap;
	Camera* m_ptrCamera;
	Wallet* m_ptrWallet;

	//EnemyMike* m_ptrTestEnemy;

	std::vector<EnemyMike*> m_ptrEnemies{};
	std::vector<Coins*> m_ptrCoins{};

	//Timer
	float m_ShowWalletCounter{};
	const float m_MAX_SHOW_WALLET_DELAY{ 1.5f };

};