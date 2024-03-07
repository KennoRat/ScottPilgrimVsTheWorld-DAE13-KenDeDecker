#pragma once
#include <Vector2f.h>
#include "Texture.h"
#include "Transform.h"

class ScottPilgrim
{
public:
	ScottPilgrim(Point2f position, float width, float height);
	~ScottPilgrim();

	void Draw() const;
	void Update(float elapsedSec);
	void Move(float elapsedSec, bool moveRight, bool moveLeft = false ,bool moveUp = false, bool moveDown = false);

	enum class Status
	{
		Idle = 1, Moving = 3
	};

	Status m_ScottStatus;

private:
	bool m_IsAlive;
	Point2f m_Position;
	float m_Width;
	float m_Height;
	Vector2f m_Velocity;
	Texture* m_ptrSpriteSheet;
	float m_FrameNR;
	float m_MaxFrame;
	float m_AnimationCounter;
	float m_MaxAnimation;
	Transform m_Transform{};
	
	bool m_NotIdle;
	void ResetFrame();
};

