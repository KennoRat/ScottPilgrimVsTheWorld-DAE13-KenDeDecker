#include "pch.h"
#include "DamageNumbers.h"

Texture* DamageNumbers::m_ptrSpriteSheet{ nullptr };
Texture* DamageNumbers::m_ptrKapowSprite{ nullptr };
int DamageNumbers::m_InstanceCounter{ 0 };

DamageNumbers::DamageNumbers(Point2f position, float Size, int number, bool Kapow): m_Position{position}, m_Size{Size}, m_FrameNR{number}, m_IsKapowEffect{Kapow}
{
	m_IsDone = false;
	m_Rumble = 0;
	m_LifetimeCounter = 0.0f;

	//std::cout << "Damage Number Created" << std::endl;

	++m_InstanceCounter;

	if (m_InstanceCounter == 1)
	{
		m_ptrSpriteSheet = new Texture("HitNumbers.png");
		m_ptrKapowSprite = new Texture("KpowEffect.png");
	}
}

DamageNumbers::~DamageNumbers() noexcept
{
	if (m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;

		delete m_ptrKapowSprite;
		m_ptrKapowSprite = nullptr;
	}

	//std::cout << "Damage Number Deleted" << std::endl;

	--m_InstanceCounter;
}

void DamageNumbers::Draw() const noexcept
{
	if(m_IsKapowEffect)
	{
		float CollumnWidth{ m_ptrKapowSprite->GetWidth() / 2.f };
		float RowHeigth = m_ptrKapowSprite->GetHeight();
		float RowIndex = 0.f;

		Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth };
		Rectf dstRect = Rectf{ 0.f, 0.f, m_ptrKapowSprite->GetWidth(), m_ptrKapowSprite->GetHeight()};

		TranslateSprite();
		m_ptrKapowSprite->Draw(dstRect, srcRect);
		ResetSprite();
	}
	else
	{
		float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 10.f };
		float RowHeigth = m_ptrSpriteSheet->GetHeight();
		float RowIndex = 0.f;

		Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth - 5.f, RowHeigth };
		Rectf dstRect = Rectf{ 0.f, 0.f, m_Size, m_Size };

		//utils::FillRect(Rectf{ m_Position.x, m_Position.y, m_Size, m_Size });

		TranslateSprite();
		m_ptrSpriteSheet->Draw(dstRect, srcRect);
		ResetSprite();
	}
}

void DamageNumbers::Update(float elapsedSec)
{
	m_LifetimeCounter += elapsedSec;
	if(m_IsKapowEffect)
	{
		if (m_LifetimeCounter > m_MAX_LIFETIME) m_IsDone = true;
		else if (m_Rumble < 10)
		{
			m_Position.y -= m_VELOCITY_Y * elapsedSec;
			++m_Rumble;
		}
		else if (m_Rumble < 20)
		{
			m_Position.y += m_VELOCITY_Y * elapsedSec;
			++m_Rumble;
		}
		else
		{
			m_Rumble = 0;
			if (m_FrameNR == 0) ++m_FrameNR;
		}
	}
	else
	{
		if (m_LifetimeCounter > m_MAX_LIFETIME) m_IsDone = true;
		else m_Position.y += m_VELOCITY_Y * elapsedSec;
	}
}

bool DamageNumbers::GetIsDone() const
{
	return m_IsDone;
}

void DamageNumbers::TranslateSprite() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
}

void DamageNumbers::ResetSprite() const
{
	glPopMatrix();
}
