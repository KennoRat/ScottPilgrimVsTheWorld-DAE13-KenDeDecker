#include "pch.h"
#include "DamageNumbers.h"

Texture* DamageNumbers::m_ptrSpriteSheet{ nullptr };
int DamageNumbers::m_InstanceCounter{ 0 };

DamageNumbers::DamageNumbers(Point2f position, float Size, int number): m_Position{position}, m_Size{Size}, m_FrameNR{number}
{
	m_IsDone = false;
	m_LifetimeCounter = 0.0f;

	std::cout << "Damage Number Created" << std::endl;

	++m_InstanceCounter;

	if (m_InstanceCounter == 1)
	{
		m_ptrSpriteSheet = new Texture("HitNumbers.png");
	}
}

DamageNumbers::~DamageNumbers() noexcept
{
	if (m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	std::cout << "Damage Number Deleted" << std::endl;

	--m_InstanceCounter;
}

DamageNumbers::DamageNumbers(const DamageNumbers& other) : DamageNumbers(other.m_Position, other.m_Size, other.m_FrameNR)
{
}

DamageNumbers& DamageNumbers::operator=(const DamageNumbers& other)
{
	if (this != &other)
	{
		m_Position = other.m_Position;
		m_Size = other.m_Size;
		m_FrameNR = other.m_FrameNR;
		m_LifetimeCounter = other.m_LifetimeCounter;
	}
	return *this;
}

DamageNumbers::DamageNumbers(DamageNumbers&& other):
  m_Position{ std::move(other.m_Position) }
, m_Size{ std::move(other.m_Size) }
, m_FrameNR{ std::move(other.m_FrameNR) }
{
	other.m_ptrSpriteSheet = nullptr;
}

DamageNumbers& DamageNumbers::operator=(DamageNumbers&& other)
{
	if (this != &other)
	{
		m_Position = std::move(other.m_Position);
		m_Size = std::move(other.m_Size);
		m_FrameNR = std::move(other.m_FrameNR);
		m_LifetimeCounter = std::move(other.m_LifetimeCounter);
		other.m_ptrSpriteSheet = nullptr;
	}
	return *this;
}

void DamageNumbers::Draw() const
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

void DamageNumbers::Update(float elapsedSec)
{
	m_LifetimeCounter += elapsedSec;

	if (m_LifetimeCounter > m_MAX_LIFETIME) m_IsDone = true;
	else m_Position.y += m_VELOCITY_Y * elapsedSec;
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
