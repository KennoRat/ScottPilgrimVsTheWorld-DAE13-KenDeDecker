#include "pch.h"
#include "SnowEffect.h"

SnowEffect::SnowEffect(float ScreenWidth, float ScreenHeight): m_ScreenWidth{ScreenWidth}, m_ScreenHeight{ ScreenHeight }
{
	for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
	{
		const int RandomPositionX = rand() % int(m_ScreenWidth);
		const int RandomPositionY = rand() % int(m_ScreenHeight);
		const int RandomSize = rand() % 5 + 6;
		const float RandomLength = float(rand() % 5 + 5);

		m_Snow[SnowIndex].m_Size = RandomSize;
		m_Snow[SnowIndex].m_Length = RandomLength;
		m_Snow[SnowIndex].m_PositionStart = Point2f{ float(RandomPositionX) , float(RandomPositionY) };
		m_Snow[SnowIndex].m_PositionEnd = Point2f{ float(RandomPositionX) - m_Snow[SnowIndex].m_Length , float(RandomPositionY) - m_Snow[SnowIndex].m_Length };
	}
}

void SnowEffect::Update(float elapsedSec)
{
	for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
	{
		if (m_Snow[SnowIndex].m_PositionStart.y < 0.f)
		{
			const float RandomX = rand() % int(m_ScreenWidth) + m_ScreenWidth / 4.f;
			const float RandomY = rand() % int(m_ScreenHeight / 4.f) + m_ScreenHeight;
			m_Snow[SnowIndex].m_Length = float(rand() % 5 + 5);
			m_Snow[SnowIndex].m_Size = rand() % 5 + 6;;
			m_Snow[SnowIndex].m_PositionStart.y = RandomY;
			m_Snow[SnowIndex].m_PositionStart.x = RandomX;
			m_Snow[SnowIndex].m_PositionEnd.y = RandomY - m_Snow[SnowIndex].m_Length;
			m_Snow[SnowIndex].m_PositionEnd.x = RandomX - m_Snow[SnowIndex].m_Length;
		}

		m_Snow[SnowIndex].m_PositionStart.y -= elapsedSec * 200.f;
		m_Snow[SnowIndex].m_PositionEnd.y -= elapsedSec * 200.f;
		m_Snow[SnowIndex].m_PositionStart.x -= elapsedSec * 200.f;
		m_Snow[SnowIndex].m_PositionEnd.x -= elapsedSec * 200.f;
	}
}

void SnowEffect::Draw() const
{
	utils::SetColor(Color4f{ 0.8f, 0.8f, 0.8f, 1.f });

	for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
	{
		utils::DrawLine(m_Snow[SnowIndex].m_PositionStart, m_Snow[SnowIndex].m_PositionEnd, m_Snow[SnowIndex].m_Size);
	}
}

void SnowEffect::CheckKeys(float elapsedSec, bool moveRight, bool moveLeft, bool moveUp, bool moveDown)
{
	float SnowSpeed{50.f};
	if (moveRight)
	{
		for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
		{
			m_Snow[SnowIndex].m_PositionStart.x -= elapsedSec * SnowSpeed;
			m_Snow[SnowIndex].m_PositionEnd.x -= elapsedSec * SnowSpeed;
		}
	}
	else if (moveLeft)
	{
		for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
		{
			m_Snow[SnowIndex].m_PositionStart.x += elapsedSec * SnowSpeed;
			m_Snow[SnowIndex].m_PositionEnd.x += elapsedSec * SnowSpeed;
		}
	}

	if (moveUp)
	{
		for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
		{
			m_Snow[SnowIndex].m_PositionStart.y -= elapsedSec * SnowSpeed;
			m_Snow[SnowIndex].m_PositionEnd.y -= elapsedSec * SnowSpeed;
		}
	}
	else if (moveDown)
	{
		for (int SnowIndex{}; SnowIndex < m_SNOW_AMOUNT; ++SnowIndex)
		{
			m_Snow[SnowIndex].m_PositionStart.y += elapsedSec * SnowSpeed;
			m_Snow[SnowIndex].m_PositionEnd.y += elapsedSec * SnowSpeed;
		}
	}

}
