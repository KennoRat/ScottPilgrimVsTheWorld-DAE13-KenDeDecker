#pragma once
#include <Vector2f.h>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "Matrix2x3.h"
#include <iostream>
#include "SoundEffects.h"

class Objects;

class EnemyMike
{
public:

	EnemyMike(Point2f position, float width, float height, SoundEffects* SoundEffects,const std::string& EnemyType = "Mike");
	virtual ~EnemyMike() noexcept;

	EnemyMike(const EnemyMike& other);
	EnemyMike& operator=(const EnemyMike& other);
	EnemyMike(EnemyMike&& other) noexcept;
	EnemyMike& operator=(EnemyMike&& other) noexcept;


	void Draw() const;
	virtual void Update(float elapsedSec, const Point2f& PlayerPosition, const std::vector<std::vector<Point2f>>& MapSvg, const Point2f& ObjectPosition = Point2f {0.f, 0.f}, bool ObjectIsLeft = false);
	void CheckHit(const std::vector<Point2f>& Attackbox, int GetDamage, bool JustToCheckCollision = false, bool GetThrownInTheAir = false, bool GetUppercut = false, bool IsAnObject = false);
	void HasPickedUpObject(bool HasPickUp, Objects* Object);

	bool CheckIdle() const;
	bool CheckIfAttackBoxIsOn();

	bool GetIsDamaged() const;
	bool GetIsBlocking() const;
	bool GetIsColliding() const;
	bool GetIsLeft() const;
	bool GetFlipBox() const;
	bool GetThrowObject() const;
	bool GetSpawnCoins() const;
	bool GetHasPickedUp() const;
	bool GetIsPickingUp() const;
	bool GetIsHit() const;
	int GetHealth() const;
	int GetGotLightHitAmount() const;
	int GetObjectRumble() const;
	Point2f GetPosition() const;
	std::vector<Point2f> GetAttackBox() const;
	Objects* GetHoldingObject() const;
	std::string GetEnemyType() const;

	void SetIsLeft(bool IsLeft);

	enum class Status
	{
		Idle = 1, Walking = 2, Hit = 3, Block = 4, Falling = 5, OnTheGround = 6, GettingUp = 7, LightAttack = 8, Sprinting = 9, Taunt = 10, SpinKick = 11
		, PickUpIdle = 12, PickUpWalk = 13, PickUpRun = 14, PickUp = 15, PickUpAttack = 16, PickUpThrow = 17
	};

	Status m_EnemyStatus;

protected:
	
	std::string m_EnemyType;

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
	bool m_BeDrawn;
	bool m_SpawnCoins;
	bool m_HasPickUpObject;
	bool m_IsPickingUp;
	bool m_FlipObject;
	bool m_ThrowObject;
	bool m_IsGoingToThrow;
	bool m_JustSpawned;
	bool m_IsHit;
	bool m_DamagedWhileStunned;
	int m_Health;
	int m_GotLightHitAmount;
	int m_ObjectRumble;
	float m_Width;
	float m_Height;
	float m_FrameNR;
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;
	Point2f m_Position;
	Point2f m_InitialJumpPosition;
	Point2f m_NewPosition;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;

	//Time
	float m_ChoicesDelayCounter;
	float m_MaxChoiceDelay;
	float m_StayOnTheGroundCounter;
	const float m_MAX_STAY_ON_THE_GROUND_DELAY{3.f};
	float m_BlockingCounter;
	const float m_MAX_BLOCKING_DELAY{0.25f};
	float m_StunnedCounter;
	const float m_MAX_STUNNED_DELAY{1.2f};
	float m_DeathCounter;
	const float m_MAX_DEATH_DELAY{ 2.4f };
	float m_DissolveCounter;
	const float m_MAX_DISSOLVE_DELAY{ 0.3f };

	//Class Association
	Objects* m_ptrHoldingObject;

	//Arrays
	std::vector<Point2f> m_HitboxOnOrigin;
	std::vector<Point2f> m_HitboxTransformed;
	std::vector<Point2f> m_AttackBoxOnOrigin;
	std::vector<Point2f> m_AttackBoxTransformed;
	utils::HitInfo m_Hitinfo;
	Status m_ChangedState;

	//Sound 
	SoundEffects* m_ptrSoundEffects;

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
	void ResetFrame();
	void UpdateAnimation();
	void UpdateChoicesDelay(const Point2f& PlayerPosition);
	virtual void UpdateChoicesDelay(const Point2f& PlayerPosition, const Point2f& ObjectPosition, bool ObjectIsLeft = false);
	void UpdateStayOnTheGround();
	void UpdateKeepBlocking(float elapsedSec);
	void UpdateStunned();
	void UpdateDeath();
	void GoToRandomPosition(float elapsedSec);
	void Block();
	void CheckIfGoingOutOfBounds(const std::vector<std::vector<Point2f>>& MapSvg);

	static Texture* m_ptrMikeSpriteSheet;
	static int m_MikeInstanceCounter;
	static Texture* m_ptrLeeSpriteSheet;
	static int m_LeeInstanceCounter;
	static Texture* m_ptrLukeSpriteSheet;
	static int m_LukeInstanceCounter;
};

