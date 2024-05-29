#include "pch.h"
#include "EnemyRichard.h"

EnemyRichard::EnemyRichard(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType) : EnemyMike(position, width, height, SoundEffects, EnemyType)
{
	m_Health = 30;
	m_MaxChoiceDelay = 1;
	m_BlockChancePercent = 30;
}

EnemyRichard::~EnemyRichard() noexcept
{
	if (m_RichardInstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_RichardInstanceCounter;
}
