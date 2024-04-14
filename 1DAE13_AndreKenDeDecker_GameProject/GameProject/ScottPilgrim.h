#pragma once
#include <Vector2f.h>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "Matrix2x3.h"
#include <iostream>


class ScottPilgrim final
{
public:
	ScottPilgrim(Point2f position, float width, float height);
	~ScottPilgrim();

	void Draw() const;
	void Update(float elapsedSec);
	/*void CheckKeys(float elapsedSec, bool KeyPressed = false);*/
	void CheckKeys(float elapsedSec, bool moveRight, bool moveLeft = false ,bool moveUp = false, bool moveDown = false);
	void Attack(bool lightAttack, bool heavyAttack = false, bool jumpAttack = false, bool uppercut = false);
	void Jump(bool moving = false, bool WasRunningBeforeJump = false);
	void TranslateSprite() const;
	void ResetSprite() const;
	bool CheckIdle() const;
	bool CheckIfHitboxIsOn() const;
	void LightAttackCounterIncrement(bool IsHit);
	void HeavyAttackCounterIncrement(bool IsHit);

	bool GetIsHit() const;
	bool GetIsJumping() const;
	bool GetIsRunningTrigger() const;
	int GetHeavyAttackCounter() const;
	Point2f GetPosition() const;
	float GetWidth() const;
	std::vector<Point2f> GetAttackBox() const;

	void SetIsRunningTrigger(bool IsRunningTrigger);

	enum class Status
	{
		Idle = 1, MovingLeft = 2, MovingRight = 20, LightAttack = 3, HeavyAttack = 4, Jump = 5, JumpKick = 6, Uppercut = 7, RunningLeft = 8, RunningRight = 21, SpinKick = 9
	};

	Status m_ScottStatus;

private:
	// Variables
	bool m_IsAlive;
	bool m_IsAttacking;
	bool m_IsJumping;
	bool m_JumpStartUpAndLand;
	bool m_MovingWhenJumping;
	bool m_IsJumpKicking;
	bool m_IsHitboxOn;
	bool m_IsLeft;
	bool m_WasRunningBeforeJump;
	bool m_IsRunningTrigger;
	bool m_DidLightAttackHit;
	bool m_DidHeavyAttackHit;
	int m_LightAttackCounter;
	int m_HeavyAttackCounter;
	Point2f m_Position;
	Point2f m_InitialJumpPosition;
	float m_Width;
	float m_Height;
	float m_MaxFrame;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	Status m_ChangedState;

	// Time
	float m_AnimationCounter;
	float m_MaxAnimation;
	float m_LightAttackDelayCounter;
	const float m_MAX_LIGHTATTACK_DELAY{1.5f};
	float m_HeavyAttackDelayCounter;
	const float m_MAX_HEAVYATTACK_DELAY{1.5f};
	float m_JumpDelayCounter;
	const float m_MAX_JUMP_DELAY{ 0.15f };
	float m_RunningDelayCounter;
	const float m_MAX_RUNNING_DELAY{ 0.3f };
	
	// Arrays
	std::vector<Point2f> m_PlayerHitboxOnOrigin;
	std::vector<Point2f> m_AttackBoxOnOrigin;
	std::vector<Point2f> m_PlayerHitboxTransformed;
	std::vector<Point2f> m_AttackBoxTransformed;

	// Functions
	void ResetFrame();
	void UpdateAnimation();
	void UpdateLightAttackDelay();
	void UpdateHeavyAttackDelay();
	void UpdateJumpDelay();
	void UpdateRunningDelay();
	void UpdatePositionDuringJump(float elapsedSec, bool isAttacking = false);
};

