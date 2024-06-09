#pragma once
#include "Texture.h"
#include "utils.h"

class PlayerUI final
{
public:
	explicit PlayerUI(float size);
	~PlayerUI() noexcept;

	PlayerUI(const PlayerUI& other) = delete;
	PlayerUI& operator=(const PlayerUI& other) = delete;
	PlayerUI(PlayerUI&& other) = delete;
	PlayerUI& operator=(PlayerUI&& other) = delete;

	void Draw() const;
	void DrawUI() const;
	void Update(float elapsedSec, Point2f Position, int AboveDecimal, int BelowDecimal);

	void SetPlayerHealth(int Health);

private:

	Texture* m_ptrSpriteSheet;
	Texture* m_ptrSpriteUI;

	Point2f m_Position;
	float m_Size;
	int m_AboveDecimalPoint;
	int m_BelowDecimalPoint;
	int m_PlayerHealth;
	float m_FrameNR;
	const float m_MAX_FRAME{ 14.f };

	//Time
	float m_AnimationCounter;
	const float m_MAX_ANIMATION{ 0.10f };

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
};

