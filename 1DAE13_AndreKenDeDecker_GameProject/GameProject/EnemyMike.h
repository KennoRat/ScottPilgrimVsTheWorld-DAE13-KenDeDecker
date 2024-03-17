#pragma once
#include <Vector2f.h>
#include "Texture.h"

class EnemyMike
{
public:

	EnemyMike(Point2f position, float width, float height);
	~EnemyMike();

	void Draw() const;
	void Update(float elapsedSec);
	void TranslateSprite() const;
	void ResetSprite() const;

	Point2f GetPosition() const;

	enum class Status
	{
		Idle = 1
	};

	Status m_EnemyStatus;

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

