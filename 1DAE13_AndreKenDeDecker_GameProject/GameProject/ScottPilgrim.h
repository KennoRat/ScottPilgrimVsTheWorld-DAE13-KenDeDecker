#pragma once
#include <Vector2f.h>
#include "Texture.h"

class ScottPilgrim
{
public:
	ScottPilgrim(Point2f position, float width, float height);
	~ScottPilgrim();

	void Draw() const;
	void Update(float elapsedSec);
	void Move();


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
};

