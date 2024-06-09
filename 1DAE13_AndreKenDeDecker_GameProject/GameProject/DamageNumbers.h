#pragma once
#include <iostream>
#include "Texture.h"
#include "utils.h"

class DamageNumbers final
{
public:
	explicit DamageNumbers(Point2f position, float Size, int number, bool Kapow = false);
	~DamageNumbers() noexcept;

	DamageNumbers(const DamageNumbers& other) = delete;
	DamageNumbers& operator=(const DamageNumbers& other) = delete;
	DamageNumbers(DamageNumbers&& other) = delete;
	DamageNumbers& operator=(DamageNumbers&& other) = delete;

	void Draw() const noexcept;
	void Update(float elapsedSec);

	bool GetIsDone() const;

private:
	static Texture* m_ptrSpriteSheet;
	static Texture* m_ptrKapowSprite;
	static int m_InstanceCounter;

	Point2f m_Position;
	bool m_IsDone;
	bool m_IsKapowEffect;
	int m_Rumble;
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

