#include "pch.h"
#include "ScottPilgrim.h"

ScottPilgrim::ScottPilgrim(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_IsAttacking = false;
	m_IsLeft = false;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("PlayerScott_Sprite.png");
	m_ChangedState = Status::Idle;
	m_ScottStatus = Status::Idle;
}

ScottPilgrim::~ScottPilgrim()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void ScottPilgrim::Draw() const
{
	float CollumnWidth{}; // Width that will be taken from spritesheet
	float CollumnWidthPosition{};
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 61.f; // Row Height from spritesheet
	float RowIndex = float(m_ScottStatus);
	float FrameNrCopy{}; // Used when overriding frames to use when CollumnWidthPosition needs to be used eg. attacks that make the world sprite longer
	float ExtraWidthDst{}; // Makes the sprite bigger in the world space
	float ExtraWidthPosSrc{5.f}; // Makes adjustments to the position
	float ExtraDrawSrcWidth{};	 // Makes adjustments to the width
	float ExtraHeightPosSrc{};	 
	float ExtraDrawSrcHeight{};	 // Makes adjustments to the Height

	switch (m_ScottStatus)
	{
	case ScottPilgrim::Status::Idle:
		CollumnWidth = m_ptrSpriteSheet->GetWidth() / 49.5f;
		CollumnWidthPosition = CollumnWidth;
		FrameNrCopy = m_FrameNR;
		break;

	case ScottPilgrim::Status::MovingRight:
		RowIndex = float(Status::MovingLeft);
	case ScottPilgrim::Status::MovingLeft:
		CollumnWidth = m_ptrSpriteSheet->GetWidth() / 51.07f;
		CollumnWidthPosition = CollumnWidth;
		FrameNrCopy = m_FrameNR;
		ExtraWidthPosSrc = 6.f;
		ExtraDrawSrcWidth = -1.f;
		ExtraWidthDst = -m_Width / 10.f;
		break;
	case ScottPilgrim::Status::LightAttack:
		FrameNrCopy = 1.f;
		ExtraHeightPosSrc = -3.f;
		ExtraDrawSrcHeight = -6.f;
		if(m_FrameNR == 0)
		{
			CollumnWidth = 43.f;
			CollumnWidthPosition = 0.f;
		}
		else if(m_FrameNR == 1)
		{
			CollumnWidth = 58.f;
			CollumnWidthPosition = 43.f;
			ExtraWidthDst = m_Width/3.f;
		}
		else if(m_FrameNR == 2)
		{
			CollumnWidth = 54.f;
			CollumnWidthPosition = 103.f;
			ExtraWidthDst = m_Width / 3.f;
		}
		else if (m_FrameNR == 3)
		{
			CollumnWidth = 41.f;
			CollumnWidthPosition = 158.f;
		}

		break;
	}

	Rectf srcRect = Rectf{ FrameNrCopy * CollumnWidthPosition + ExtraWidthPosSrc, RowHeigth * RowIndex + ExtraHeightPosSrc, CollumnWidth + ExtraDrawSrcWidth, RowHeigth + ExtraDrawSrcHeight };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Width + ExtraWidthDst, m_Height};

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
		UpdateAnimation();
		break;
	case ScottPilgrim::Status::MovingRight:
	case ScottPilgrim::Status::MovingLeft:
		m_MaxAnimation = 0.09f;
		m_MaxFrame = 5.f;
		UpdateAnimation();
		break;
	case ScottPilgrim::Status::LightAttack:
		m_MaxAnimation = 0.09f;
		m_MaxFrame = 3.f;
		UpdateAnimation();
		break;
	}
}

void ScottPilgrim::UpdateAnimation()
{
	if (m_AnimationCounter >= m_MaxAnimation)
	{
		if (m_FrameNR < m_MaxFrame)
		{
			++m_FrameNR;
		}
		else
		{
			m_FrameNR = 0.f;
		/*	if(m_ScottStatus != Status::LightAttack)
			{
				m_IsAttacking = false;
			}*/
			m_IsAttacking = false;
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void ScottPilgrim::Move(float elapsedSec, bool moveRight, bool moveLeft, bool moveUp, bool moveDown)
{
	if(m_IsAttacking == false)
	{
		ResetFrame();
		if (moveRight)
		{
			m_ScottStatus = Status::MovingRight;
			m_IsLeft = false;
			m_Position.x += m_Velocity.x * elapsedSec;
			//m_Transform.Rotation.x = 0.f;
		}
		else if (moveLeft)
		{
			m_ScottStatus = Status::MovingLeft;
			m_IsLeft = true;
			m_Position.x -= m_Velocity.x * elapsedSec;
			//m_Transform.Rotation.x += 90.f * elapsedSec;
		}

		if (moveUp)
		{
			m_ScottStatus = Status::MovingRight;
			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else if (moveDown)
		{
			m_ScottStatus = Status::MovingLeft;
			m_Position.y -= m_Velocity.y * elapsedSec;
		}
		
		if(m_Position.y <= -3.0f)
		{
			m_Position.y = -3.0f;
		}
		else if (m_Position.y >= 430.0f)
		{
			m_Position.y = 430.0f;
		}

		if(m_Position.x <= 0.0f)
		{
			m_Position.x = 0.0f;
		}
	}
}

void ScottPilgrim::Attack(bool lightAttack)
{
	ResetFrame();
	m_ScottStatus = Status::LightAttack;
	m_IsAttacking = true;
}

void ScottPilgrim::TranslateSprite() const
{
	glPushMatrix();


	if(m_IsLeft)
	{
		glTranslatef(m_Position.x + m_Width, m_Position.y, 0.f);
		glScalef(-1.f, 1.f, 0.f);
	}
	else
	{
		glTranslatef(m_Position.x, m_Position.y, 0.f);
	}
}

void ScottPilgrim::ResetSprite() const
{
	glPopMatrix();
}

bool ScottPilgrim::CheckIdle() const
{
	if(m_IsAttacking)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Point2f ScottPilgrim::GetPosition() const
{
	return m_Position;
}

void ScottPilgrim::ResetFrame()
{
	if((m_ChangedState != m_ScottStatus))
	{
		m_FrameNR = 0.f;
		m_ChangedState = m_ScottStatus;
	}
}
