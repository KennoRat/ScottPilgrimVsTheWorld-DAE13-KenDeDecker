#include "pch.h"
#include "ScottPilgrim.h"

ScottPilgrim::ScottPilgrim(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_Velocity = Vector2f{ 50.f, 50.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("PlayerScott_Sprite.png");
}

ScottPilgrim::~ScottPilgrim()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void ScottPilgrim::Draw() const
{
	float CollumnWidth = m_ptrSpriteSheet->GetWidth() / 49.5f;
	float RowWidth = m_ptrSpriteSheet->GetHeight() / 60.f;

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth + 8.f, RowWidth, CollumnWidth, RowWidth };
	Rectf dstRect = Rectf{ m_Position.x, m_Position.y, m_Width, m_Height };

	m_ptrSpriteSheet->Draw(dstRect, srcRect);
}

void ScottPilgrim::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;
	if(m_AnimationCounter >= m_MaxAnimation)
	{
		if(m_FrameNR < m_MaxFrame)
		{
			++m_FrameNR;
		}
		else
		{
			m_FrameNR = 0.f;
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}
