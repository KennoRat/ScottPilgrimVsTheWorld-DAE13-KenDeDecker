#pragma once
#include "Texture.h"
#include "utils.h"

class Wallet
{
public:
	Wallet(float size);

	void Draw() const;
	void Update(float elapsedSec, Point2f Position, int AboveDecimal, int BelowDecimal);

private:

	Texture* m_ptrSpriteSheet;

	Point2f m_Position;
	float m_Size;
	int m_AboveDecimalPoint;
	int m_BelowDecimalPoint;
	float m_FrameNR;
	const float m_MAX_FRAME{ 14.f };

	//Time
	float m_AnimationCounter;
	const float m_MAX_ANIMATION{ 0.10f };

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
};

