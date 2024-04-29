#pragma once
#include <Vector2f.h>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "Matrix2x3.h"
#include <iostream>

class EnemyMike
{
public:

	EnemyMike(Point2f position, float width, float height);
	~EnemyMike();

	void Draw() const;
	void Update(float elapsedSec, const Point2f& PlayerPosition);
	void TranslateSprite() const;
	void ResetSprite() const;
	void CheckHit(const std::vector<Point2f>& Attackbox, bool JustToCheckCollision = false, bool GetThrownInTheAir = false, bool GetUppercut = false);
	bool CheckIdle() const;
	bool CheckIfAttackBoxIsOn();

	bool GetIsDamaged() const;
	bool GetIsColliding() const;
	bool GetIsLeft() const;
	int GetHealth() const;
	int GetGotLightHitAmount() const;
	Point2f GetPosition() const;
	std::vector<Point2f> GetAttackBox() const;

	void SetIsLeft(bool IsLeft);

	enum class Status
	{
		Idle = 1, Walking = 2, Hit = 3, Block = 4, Falling = 5, OnTheGround = 6, GettingUp = 7, LightAttack = 8, Sprinting = 9, Taunt = 10, SpinKick = 11, PickUpIdle = 12
	};

	Status m_EnemyStatus;

private:

	static Texture* m_ptrSpriteSheet;
	static int m_InstanceCounter;

	//Variables
	bool m_IsAlive;
	bool m_IsAttacking;
	bool m_IsDamaged;
	bool m_IsColliding;
	bool m_IsOnTheGround;
	bool m_IsLeft;
	bool m_IsFalling;
	bool m_IsGettingUp;
	bool m_IsMoving;
	bool m_IsAttackBoxOn;
	bool m_AttackBoxReset;
	bool m_IsTaunting;
	bool m_StayOnGround;
	bool m_IsBlocking;
	bool m_IsAggressive;
	bool m_IsStunned;
	Point2f m_Position;
	Point2f m_InitialJumpPosition;
	Point2f m_NewPosition;
	float m_Width;
	float m_Height;
	int m_Health;
	int m_GotLightHitAmount;
	Vector2f m_Velocity;
	//Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;

	//Time
	float m_ChoicesDelayCounter;
	float m_MaxChoiceDelay;
	float m_StayOnTheGroundCounter;
	const float m_MAX_STAY_ON_THE_GROUND_DELAY{3.f};
	float m_BlockingCounter;
	const float m_MAX_BLOCKING_DELAY{0.25f};
	float m_MoveToPlayerCounter;
	const float m_MOVE_TO_PLAYER_DELAY{0.3f};
	float m_StunnedCounter;
	const float m_MAX_STUNNED_DELAY{0.8f};


	//Arrays
	std::vector<Point2f> m_HitboxOnOrigin;
	std::vector<Point2f> m_HitboxTransformed;
	std::vector<Point2f> m_AttackBoxOnOrigin;
	std::vector<Point2f> m_AttackBoxTransformed;
	utils::HitInfo m_Hitinfo;
	Status m_ChangedState;

	//Functions
	void ResetFrame();
	void UpdateAnimation();
	void UpdateChoicesDelay(const Point2f& PlayerPosition);
	void UpdateStayOnTheGround();
	void UpdateKeepBlocking(float elapsedSec);
	void UpdateStunned();
	void GoToRandomPosition(float elapsedSec);
	void Block();
};

