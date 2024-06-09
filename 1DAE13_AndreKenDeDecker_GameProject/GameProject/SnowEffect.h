#pragma once
#include "utils.h"
class SnowEffect final
{
public:
	explicit SnowEffect(float ScreenWidth, float ScreenHeight);

	void Update(float elapsedSec);
	void Draw() const;
	void CheckKeys(float elapsedSec, bool moveRight, bool moveLeft = false, bool moveUp = false, bool moveDown = false);

private:
	static const int m_SNOW_AMOUNT{ 8 };
	struct SnowData
	{
		Point2f m_PositionStart;
		Point2f m_PositionEnd;
		int m_Size;
		float m_Length;
	};

	SnowData m_Snow[m_SNOW_AMOUNT]{};

	float m_ScreenWidth;
	float m_ScreenHeight;

};

