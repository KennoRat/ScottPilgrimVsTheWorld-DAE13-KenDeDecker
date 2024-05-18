#pragma once
#include "BaseGame.h"
#include "ScottPilgrim.h"
#include "EnemyMike.h"
#include "EnemyLee.h"
#include "Camera.h"
#include "Coins.h"
#include "PlayerUI.h"
#include <iostream>
#include "Objects.h"
#include <SVGParser.h>
#include "DamageNumbers.h"

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
	void PlayerKeys(float elapsedSec);
	void SortDraw() const;
	void SortYPosition(int Amount, const std::vector<EnemyMike*>& Vectors) const;
	void SortDrawObject(const EnemyMike* Enemy) const;
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

	const float m_PLAYER_HEIGHT{ 280.f };
	const float m_PLAYER_WIDTH{ 320.f };
	const float m_ENEMY_HEIGHT{ 230.f };
	const float m_ENEMY_WIDTH{ 280.f };
	const float m_COINS_SIZE{ 80.f };
	const float m_DAMAGE_SIZE{ 40.f };

	ScottPilgrim* m_ptrPlayer;
	Texture* m_ptrMap;
	Camera* m_ptrCamera;
	PlayerUI* m_ptrPlayerUI;

	std::vector<EnemyMike*> m_ptrEnemies{};
	std::vector<Objects*> m_ptrObjects{};
	std::vector<Coins*> m_ptrCoins{};
	std::vector<DamageNumbers*> m_ptrDamageNumbers{};

	std::vector<std::vector<Point2f>> m_MapSvg;
	std::vector<Point2f> m_TransformSvg;

	std::vector<std::vector<Point2f>> m_CameraSvg;

	//Timer
	float m_ShowWalletCounter{};
	const float m_MAX_SHOW_WALLET_DELAY{ 1.5f };

};