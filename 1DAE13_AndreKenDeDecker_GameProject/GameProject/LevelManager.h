#pragma once
#include "utils.h"
#include "ScottPilgrim.h"
#include "EnemyMike.h"
#include "EnemyLee.h"
#include "EnemyLuke.h"
#include "EnemyRichard.h"
#include "Camera.h"
#include "Coins.h"
#include "PlayerUI.h"
#include <iostream>
#include "Objects.h"
#include <SVGParser.h>
#include "DamageNumbers.h"
#include "SoundStream.h"
#include "SoundEffects.h"
#include "HitEffects.h"
#include "SnowEffect.h"

class LevelManager final
{
public:
	explicit LevelManager(Rectf Viewport, SoundEffects* SoundEffects);
	~LevelManager() noexcept;

	LevelManager(const LevelManager& other) = delete;
	LevelManager& operator=(const LevelManager& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	LevelManager& operator=(LevelManager&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;
	void ResetLevel();

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
private:
	//ViewPort
	Rectf m_ViewPort;

	//Functions
	void PlayerKeys(float elapsedSec);
	void SortDraw() const;
	void SortYPosition(int Amount, const std::vector<EnemyMike*>& Vectors) const;
	void SortDrawObject(const EnemyMike* Enemy) const;
	void ShowWallet();
	void FightingAreaEvent();
	void FallPit();

	//Variables
	bool m_PlayerLightAttacked{ false };
	bool m_PlayerResetLightAttackButton{ false };
	bool m_PlayerUppercutAttack{ false };

	bool m_PlayerHeavyAttacked{ false };
	bool m_PlayerResetHeavyAttackButton{ false };

	bool m_PlayerHasHitAnEnemy{ false };
	bool m_PlayNoHitEffect{ false };

	bool m_PlayerResetRunLeft{ true };
	bool m_PlayerResetRunRight{ true };

	bool m_DoShowWallet{ false };
	bool m_BeatLevel{ false };
	bool m_ShowHitboxesAndBorders{ false };

	int m_AboveDecimalPoint{};
	int m_BelowDecimalPoint{};
	int m_FightEvents{ 0 };
	int m_PlayerHealth{ 50 };

	const float m_PLAYER_HEIGHT{ 280.f };
	const float m_PLAYER_WIDTH{ 320.f };
	const float m_ENEMY_HEIGHT{ 230.f };
	const float m_ENEMY_WIDTH{ 280.f };
	const float m_COINS_SIZE{ 80.f };
	const float m_DAMAGE_SIZE{ 40.f };
	const float m_HITEFFECT_SIZE{ 170.f };

	bool m_StopCamera{ false };
	float m_StopCameraXPosition{};

	utils::HitInfo m_Hitinfo;

	//Timer
	float m_ShowWalletCounter{};
	const float m_MAX_SHOW_WALLET_DELAY{ 1.5f };

	//Classes
	ScottPilgrim* m_ptrPlayer;
	Texture* m_ptrMap;
	Camera* m_ptrCamera;
	PlayerUI* m_ptrPlayerUI;
	SnowEffect* m_ptrSnowEffect;

	std::vector<EnemyMike*> m_ptrEnemies{};
	std::vector<Objects*> m_ptrObjects{};
	std::vector<Coins*> m_ptrCoins{};
	std::vector<DamageNumbers*> m_ptrDamageNumbers{};
	std::vector<HitEffects*> m_ptrHitEffects{};

	std::vector<std::vector<Point2f>> m_MapSvg;
	std::vector<Point2f> m_TransformedMapSvg;

	std::vector<std::vector<Point2f>> m_CameraSvg;
	std::vector<Point2f> m_TransformedCameraSvg;

	std::vector<std::vector<Point2f>> m_PitSvg;
	std::vector<Point2f> m_TransformedPitSvg;

	std::vector<std::vector<Point2f>> m_TranformedBordersSvg;

	std::vector<float> m_FightAreasXPosition;

	//Sound
	SoundEffects* m_ptrSoundEffects;

	//Textre
	Texture* m_ptrPit;
};

