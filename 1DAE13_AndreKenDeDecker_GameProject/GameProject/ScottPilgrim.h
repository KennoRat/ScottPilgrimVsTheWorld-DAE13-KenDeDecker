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
	void Attack(bool lightAttack);
	void TranslateSprite() const;
	void ResetSprite() const;
	bool CheckIdle() const;
	bool CheckIfHitboxIsOn() const;
	void LightAttackCounter(bool IsHit);

	Point2f GetPosition() const;
	float GetWidth() const;
	std::vector<Point2f> GetAttackBox() const;

	enum class Status
	{
		Idle = 1, MovingLeft = 2, MovingRight = 4 ,LightAttack = 3
	};

	Status m_ScottStatus;

private:
	// Variable
	bool m_IsAlive;
	bool m_IsAttacking;
	bool m_IsHitboxOn;
	bool m_IsLeft; 
	int m_LightAttackCounter;
	Point2f m_Position;
	float m_Width;
	float m_Height;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	Status m_ChangedState;

	// Time
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;
	float m_AttackDelayCounter;
	float m_MAX_ATTACK_DELAY{1.5f};
	
	// Arrays
	std::vector<Point2f> m_PlayerHitboxOnOrigin;
	std::vector<Point2f> m_AttackBoxOnOrigin;
	std::vector<Point2f> m_PlayerHitboxTransformed;
	std::vector<Point2f> m_AttackBoxTransformed;

	// Functions
	void ResetFrame();
	void UpdateAnimation();
	void UpdateAttackDeley();
};

