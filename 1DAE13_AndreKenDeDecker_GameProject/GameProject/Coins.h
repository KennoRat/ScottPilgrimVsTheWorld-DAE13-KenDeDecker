#pragma once
#include "Vector2f.h"
#include "Texture.h"
#include "utils.h"
#include "Matrix2x3.h"

class Coins
{
public:
	enum class Type
	{
		Cents5 = 1, Cents10 = 2, Cents25 = 3, Dollar1 = 4, Dollars2 = 5
	};

	Type m_CoinType;

	Coins(Point2f position, float size, Type coin);
	~Coins() noexcept;

	Coins(const Coins& other);
	Coins& operator=(const Coins& other);
	Coins(Coins&& other) noexcept;
	Coins& operator=(Coins&& other) noexcept;

	void Draw() const;
	void Update(float elapsedSec);

	std::vector<Point2f> GetHitbox() const;
	int GetValue() const ;

private:
	static Texture* m_ptrSpriteSheet;
	static int m_InstanceCounter;

	Point2f m_Position;
	Vector2f m_Velocity;
	bool m_IsFalling;
	bool m_IsFlying;
	int m_Bounce;
	float m_Size;
	float m_NewYPosition;
	float m_FrameNR;
	const float m_MAX_FRAME{9.f};

	//Time
	float m_AnimationCounter;
	const float m_MAX_ANIMATION{0.10f};

	//Arrays
	std::vector<Point2f> m_HitboxOrigin;
	std::vector<Point2f> m_HitboxTransformed;

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
	void UpdatePositionAfterSpawn(float elapsedSec);
};

