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
	void Update(float elapsedSec);
	void TranslateSprite() const;
	void ResetSprite() const;
	void CheckHit(const std::vector<Point2f>& Attackbox, bool JustToCheckCollision = false, bool GetThrownInTheAir = false);
	bool CheckIdle() const;
	
	bool GetIsDamaged() const;
	bool GetIsColliding() const;
	int GetHealth() const;

	Point2f GetPosition() const;

	enum class Status
	{
		Idle = 1, Walking = 2, Hit = 3, Block = 4, Falling = 5, OnTheGround = 6, GettingUp = 7, LightAttack = 8
	};

	Status m_EnemyStatus;

private:
	bool m_IsAlive;
	bool m_IsAttacking;
	bool m_IsDamaged;
	bool m_IsColliding;
	bool m_IsOnTheGround;
	bool m_IsLeft;
	bool m_IsFalling;
	bool m_IsGettingUp;
	Point2f m_Position;
	Point2f m_InitialJumpPosition;
	float m_Width;
	float m_Height;
	int m_Health;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;

	std::vector<Point2f> m_HitboxOnOrigin;
	std::vector<Point2f> m_HitboxTransformed;
	utils::HitInfo m_Hitinfo;
	Status m_ChangedState;
	void ResetFrame();
	void UpdateAnimation();
};

