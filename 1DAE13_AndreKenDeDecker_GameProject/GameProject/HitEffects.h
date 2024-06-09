#pragma once
#include "Texture.h"
#include "utils.h"

class HitEffects final
{
public:
	explicit HitEffects(Point2f position, float Size, bool Block = false);
	~HitEffects() noexcept;

	HitEffects(const HitEffects& other) = delete;
	HitEffects& operator=(const HitEffects& other) = delete;
	HitEffects(HitEffects&& other) = delete;
	HitEffects& operator=(HitEffects&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	bool GetIsDone() const;

private:
	static Texture* m_ptrSpriteSheet;
	static int m_InstanceCounter;

	Point2f m_Position;
	bool m_IsDone;
	bool m_IsBlockEffect;
	int m_FrameNR;
	const int m_MAX_FRAME{4};
	float m_Size;


	//Time
	float m_AnimationCounter;
	const float m_MAX_ANIMATION{ 0.10f };

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
};

