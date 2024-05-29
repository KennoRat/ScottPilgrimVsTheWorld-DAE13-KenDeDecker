#include "pch.h"
#include "EnemyLuke.h"

EnemyLuke::EnemyLuke(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType) : EnemyMike(position, width, height, SoundEffects ,EnemyType)
{
	m_Health = 40;
	m_MaxChoiceDelay = 1;
	m_BlockChancePercent = 25;
}

EnemyLuke::~EnemyLuke() noexcept
{
	if (m_LukeInstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_LukeInstanceCounter;
}
