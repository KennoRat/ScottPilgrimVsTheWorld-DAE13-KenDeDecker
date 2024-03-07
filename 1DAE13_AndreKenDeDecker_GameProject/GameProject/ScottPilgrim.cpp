#include "pch.h"
#include "ScottPilgrim.h"

ScottPilgrim::ScottPilgrim(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("PlayerScott_Sprite.png");
	m_NotIdle = false;

	m_ScottStatus = Status::Idle;
}

ScottPilgrim::~ScottPilgrim()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void ScottPilgrim::Draw() const
{
	float CollumnWidth;
	float RowWidth = m_ptrSpriteSheet->GetHeight() / 61.f;

	switch (m_ScottStatus)
	{
	case ScottPilgrim::Status::Idle:
		CollumnWidth = m_ptrSpriteSheet->GetWidth() / 49.5f;
		break;
	case ScottPilgrim::Status::Moving:
		CollumnWidth = m_ptrSpriteSheet->GetWidth() / 51.07f;
		break;
	}

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth + 5.f, RowWidth * float(m_ScottStatus), CollumnWidth, RowWidth};
	Rectf dstRect = Rectf{ m_Position.x, m_Position.y, m_Width, m_Height };

	m_ptrSpriteSheet->Draw(dstRect, srcRect);
}

void ScottPilgrim::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;
	switch (m_ScottStatus)
	{
	case ScottPilgrim::Status::Idle:
		m_MaxAnimation = 0.09f;
		m_MaxFrame = 7.f;
		if (m_AnimationCounter >= m_MaxAnimation)
		{
			if (m_FrameNR < m_MaxFrame)
			{
				++m_FrameNR;
			}
			else
			{
				m_FrameNR = 0.f;
			}
			m_AnimationCounter -= m_MaxAnimation;
		}
		m_NotIdle = false;
		break;
	case ScottPilgrim::Status::Moving:
		m_MaxAnimation = 0.09f;
		m_MaxFrame = 5.f;
		if (m_AnimationCounter >= m_MaxAnimation)
		{
			if (m_FrameNR < m_MaxFrame)
			{
				++m_FrameNR;
			}
			else
			{
				m_FrameNR = 0.f;
			}
			m_AnimationCounter -= m_MaxAnimation;
		}
		break;
	}
}

void ScottPilgrim::Move(float elapsedSec, bool moveRight, bool moveLeft, bool moveUp, bool moveDown)
{
	m_ScottStatus = Status::Moving;
	ResetFrame();
	if(moveRight)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
	}
	else if(moveLeft)
	{
		m_Position.x -= m_Velocity.x * elapsedSec;
	}

	if(moveUp)
	{
		m_Position.y += m_Velocity.y * elapsedSec;
	}
	else if (moveDown)
	{
		m_Position.y -= m_Velocity.y * elapsedSec;
	}

}

void ScottPilgrim::ResetFrame()
{
	if(m_NotIdle == false)
	{
		m_FrameNR = 0.f;
		m_NotIdle = true;
	}
}
