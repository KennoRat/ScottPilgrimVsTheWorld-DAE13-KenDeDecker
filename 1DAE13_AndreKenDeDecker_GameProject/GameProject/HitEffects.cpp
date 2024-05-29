#include "pch.h"
#include "HitEffects.h"

Texture* HitEffects::m_ptrSpriteSheet{ nullptr };
int HitEffects::m_InstanceCounter{ 0 };

HitEffects::HitEffects(Point2f position, float Size, bool Block):m_Position{position}, m_Size{ Size },m_IsBlockEffect{Block}
{
	m_IsDone = false;
	m_FrameNR = 0;
	m_AnimationCounter = 0.f;

	++m_InstanceCounter;

	if (m_InstanceCounter == 1)
	{
		m_ptrSpriteSheet = new Texture("HitEffects_Sprite.png");
	}
}

HitEffects::~HitEffects() noexcept
{
	if (m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_InstanceCounter;
}

void HitEffects::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 5.f };
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 2.f;
	float RowIndex;
	if (m_IsBlockEffect) RowIndex = 0.f;
	else RowIndex = 1.f;


	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Size, m_Size };

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect, srcRect);
	ResetSprite();
}

void HitEffects::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;

	if (m_AnimationCounter >= m_MAX_ANIMATION)
	{
		if (m_FrameNR < m_MAX_FRAME)
		{
			++m_FrameNR;
		}
		else
		{
			
			m_IsDone = true;
		}

		m_AnimationCounter -= m_MAX_ANIMATION;
	}
}

bool HitEffects::GetIsDone() const
{
	return m_IsDone;
}

void HitEffects::TranslateSprite() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
}

void HitEffects::ResetSprite() const
{
	glPopMatrix();
}
