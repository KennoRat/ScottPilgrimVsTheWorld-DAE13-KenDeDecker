#include "pch.h"
#include "EnemyMike.h"

EnemyMike::EnemyMike(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_IsAttacking = false;
	m_IsLeft = true;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("EnemyMike_Sprite.png");
	m_ChangedState = Status::Idle;
	m_EnemyStatus = Status::Idle;
}

EnemyMike::~EnemyMike()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void EnemyMike::Draw() const
{
	float CollumnWidth{};
	float CollumnWidthPosition{};
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 26.f;
	float RowIndex = float(m_EnemyStatus);
	float FrameNrCopy{};
	float ExtraWidthDst{};
	float ExtraWidthPosSrc{};
	float ExtraDrawSrcWidth{};
	float ExtraHeightPosSrc{};
	float ExtraDrawSrcHeight{};

	switch (m_EnemyStatus)
	{
	case EnemyMike::Status::Idle:
		CollumnWidth = m_ptrSpriteSheet->GetWidth() / 25.5f;
		CollumnWidthPosition = CollumnWidth;
		FrameNrCopy = m_FrameNR;
		break;
	}

	Rectf srcRect = Rectf{ FrameNrCopy * CollumnWidthPosition + ExtraWidthPosSrc, RowHeigth * RowIndex + ExtraHeightPosSrc, CollumnWidth + ExtraDrawSrcWidth, RowHeigth + ExtraDrawSrcHeight };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Width + ExtraWidthDst, m_Height };

	m_ptrSpriteSheet->Draw(dstRect, srcRect);
}

void EnemyMike::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;
	switch (m_EnemyStatus)
	{
	case EnemyMike::Status::Idle:
		m_MaxFrame = 3.f;
		UpdateAnimation();
		break;
	}
}

void EnemyMike::UpdateAnimation()
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
			m_IsAttacking = false;
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void EnemyMike::TranslateSprite() const
{
	glPushMatrix();


	if (m_IsLeft)
	{
		glTranslatef(m_Position.x + m_Width, m_Position.y, 0.f);
		glScalef(-1.f, 1.f, 0.f);
	}
	else
	{
		glTranslatef(m_Position.x, m_Position.y, 0.f);
	}
}

void EnemyMike::ResetSprite() const
{
	glPopMatrix();
}

Point2f EnemyMike::GetPosition() const
{
	return m_Position;
}

void EnemyMike::ResetFrame()
{
	if ((m_ChangedState != m_EnemyStatus))
	{
		m_FrameNR = 0.f;
		m_ChangedState = m_EnemyStatus;
	}
}
