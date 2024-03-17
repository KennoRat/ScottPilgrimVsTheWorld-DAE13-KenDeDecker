#pragma once
#include <Vector2f.h>
#include "Texture.h"


class ScottPilgrim final
{
public:
	ScottPilgrim(Point2f position, float width, float height);
	~ScottPilgrim();

	void Draw() const;
	void Update(float elapsedSec);
	void Move(float elapsedSec, bool moveRight, bool moveLeft = false ,bool moveUp = false, bool moveDown = false);
	void Attack(bool lightAttack);
	void TranslateSprite() const;
	void ResetSprite() const;
	bool CheckIdle() const;

	Point2f GetPosition() const;

	enum class Status
	{
		Idle = 1, MovingLeft = 3, MovingRight = 4 ,LightAttack = 35
	};

	Status m_ScottStatus;
private:
	bool m_IsAlive;
	bool m_IsAttacking;
	bool m_IsLeft;
	Point2f m_Position;
	float m_Width;
	float m_Height;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;
	
	Status m_ChangedState;
	void ResetFrame();
	void UpdateAnimation();
};

