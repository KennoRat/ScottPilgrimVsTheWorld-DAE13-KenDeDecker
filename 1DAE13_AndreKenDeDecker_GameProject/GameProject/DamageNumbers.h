#pragma once
#include <iostream>
#include "Texture.h"
#include "utils.h"

class DamageNumbers
{
public:
	DamageNumbers(Point2f position, float Size, int number);
	~DamageNumbers() noexcept;

	DamageNumbers(const DamageNumbers& other);
	DamageNumbers& operator=(const DamageNumbers& other);
	DamageNumbers(DamageNumbers&& other);
	DamageNumbers& operator=(DamageNumbers&& other);

	void Draw() const;
	void Update(float elapsedSec);

	bool GetIsDone() const;

private:
	static Texture* m_ptrSpriteSheet;
	static int m_InstanceCounter;

	Point2f m_Position;
	bool m_IsDone;
	int m_FrameNR;
	float m_Size;
	const float m_VELOCITY_Y{100.f};

	//Time
	float m_LifetimeCounter;
	const float m_MAX_LIFETIME{ 1.2f };

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
};

