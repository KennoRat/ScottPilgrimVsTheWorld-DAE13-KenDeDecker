#include "pch.h"
#include "EnemyMike.h"

EnemyMike::EnemyMike(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = true;
	m_IsAttacking = false;
	m_IsDamaged = false;
	m_IsColliding = false;
	m_IsLeft = false;
	m_IsFalling = false;
	m_IsOnTheGround = false;
	m_IsGettingUp = false;
	m_IsMoving = false;
	m_Health = 10;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_InitialJumpPosition = Point2f{0.f, 0.f};
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;

	m_MovingDelayCounter = 0.f;
	m_MaxMovingDelay = 3.f;

	m_ptrSpriteSheet = new Texture("EnemyMike_Sprite.png");
	m_ChangedState = Status::Idle;
	m_EnemyStatus = Status::Idle;


	// Make Enemy Hitbox
	float HitboxWidth{ m_Width / 2.f};
	float HitboxHeight{ m_Height - m_Height / 6.f };
	m_HitboxOnOrigin.push_back(Point2f(0.f,0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidth, 0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidth, HitboxHeight));
	m_HitboxOnOrigin.push_back(Point2f(0.f, HitboxHeight));
}

EnemyMike::~EnemyMike()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void EnemyMike::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 8.f};
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 9.f;
	float RowIndex = float(m_EnemyStatus);

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth - 1.f };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Width, m_Height };

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect, srcRect);
	ResetSprite();

	utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
	utils::DrawPolygon(m_HitboxTransformed);
}

void EnemyMike::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;

	float HitboxHeight{ m_Height - m_Height / 6.f };

	switch (m_EnemyStatus)
	{
	case EnemyMike::Status::Idle:
		m_MaxFrame = 3.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		if(m_IsMoving == false)
		{
			m_MovingDelayCounter += elapsedSec;
			UpdateMovingDelay();
		}
		break;
	case EnemyMike::Status::Walking:
		m_MaxFrame = 5.f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::Hit:
		m_MaxFrame = 2.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::Falling:

		if (m_Velocity.y > 580.f) m_FrameNR = 0;
		else if (m_Velocity.y > 300.f) m_FrameNR = 1;
		else if (m_Velocity.y > 150.f) m_FrameNR = 2;
		else if (m_Velocity.y > 0.f) m_FrameNR = 3;
		else if (m_Position.y > m_InitialJumpPosition.y + 2.f * HitboxHeight / 4.f * 2.f) m_FrameNR = 4;
		else if (m_Position.y > m_InitialJumpPosition.y) m_FrameNR = 5;

		if (m_InitialJumpPosition.y < m_Position.y)
		{
			if (m_IsLeft == false)
			{
				m_Position.x -= m_Velocity.x * elapsedSec;
				m_InitialJumpPosition.x -= m_Velocity.x * elapsedSec;
			}
			else
			{
				m_Position.x += m_Velocity.x * elapsedSec;
				m_InitialJumpPosition.x += m_Velocity.x * elapsedSec;
			}

			m_Velocity.y -= 2000.f * elapsedSec;
			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else if (m_Position.y == m_InitialJumpPosition.y)
		{
			m_IsFalling = false;
			m_IsDamaged = false;
			m_IsOnTheGround = true;
			m_EnemyStatus = Status::OnTheGround;
			m_AnimationCounter = 0.f;
		}
		else
		{
			m_FrameNR = 0;
			m_Position.y = m_InitialJumpPosition.y;
			m_Velocity = Vector2f{ 200.f, 150.f };
		}

		break;

	case EnemyMike::Status::OnTheGround:

		m_MaxFrame = 7.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		break;
	case EnemyMike::Status::GettingUp:

		m_MaxFrame = 6.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		break;
	}

	// Transform Hitboxes
	Matrix2x3 TranslationMat{};
	TranslationMat.SetAsTranslate(Vector2f(m_Position));

	if(m_IsLeft)
	{
		TranslationMat.SetAsTranslate(Vector2f(m_Position.x + m_Width / 2.f, m_Position.y));
		Matrix2x3 ScaleMat{};
		ScaleMat.SetAsScale(-1.f, 1.f);
		Matrix2x3 TransformMat{ TranslationMat * ScaleMat };
		m_HitboxTransformed = TransformMat.Transform(m_HitboxOnOrigin);
	}
	else m_HitboxTransformed = TranslationMat.Transform(m_HitboxOnOrigin);

	if (m_IsMoving) GoToRandomPosition(elapsedSec);
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
			m_IsDamaged = false;
			m_IsColliding = false;
			m_IsGettingUp = false;

			if (m_IsAlive == false && m_EnemyStatus == Status::OnTheGround)
			{
				m_FrameNR = 7.f;
			}
			else if(m_IsAlive == true && m_EnemyStatus == Status::OnTheGround)
			{
				std::cout << "Getting Up is True" << std::endl;
				m_IsOnTheGround = false;
				m_EnemyStatus = Status::GettingUp;
				m_IsGettingUp = true;
			}	
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void EnemyMike::UpdateMovingDelay()
{
	if(m_MovingDelayCounter >= m_MaxMovingDelay)
	{
		int MoveDistance{500};

		m_IsMoving = true;
		m_NewPosition.x = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.x;
		m_NewPosition.y = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.y;
		m_EnemyStatus = Status::Walking;
		m_MovingDelayCounter -= m_MaxMovingDelay;
		m_MaxMovingDelay = float(rand() % 5 + 3);

		std::cout << "Moving Is True" << std::endl;
	}
}

void EnemyMike::GoToRandomPosition(float elapsedSec)
{
	if (m_NewPosition.x < m_Position.x + 2.f && m_NewPosition.x > m_Position.x - 2.f) m_NewPosition.x = m_Position.x;
	else if (m_NewPosition.x > m_Position.x) m_Position.x += m_Velocity.x * elapsedSec;
	else if (m_NewPosition.x < m_Position.x) m_Position.x -= m_Velocity.x * elapsedSec;
	

	if(m_NewPosition.y < m_Position.y + 2.f && m_NewPosition.y > m_Position.y - 2.f) m_NewPosition.y = m_Position.y;
	else if (m_NewPosition.y > m_Position.y) m_Position.y += m_Velocity.y * elapsedSec;
	else if (m_NewPosition.y < m_Position.y) m_Position.y -= m_Velocity.y * elapsedSec;
	
	if (m_Position.y <= -3.0f)
	{
		m_NewPosition.y = -3.0f;
		m_Position.y = -3.0f;
	}
	else if (m_Position.y >= 430.0f)
	{
		m_NewPosition.y = 430.0f;
		m_Position.y = 430.0f;
	}

	if (m_NewPosition.x == m_Position.x && m_NewPosition.y == m_Position.y)
	{
		m_IsMoving = false;
		m_FrameNR = 0.f;
	}
}

void EnemyMike::TranslateSprite() const
{
	glPushMatrix();


	if (m_IsLeft)
	{
		glTranslatef(m_Position.x + m_Width/2.f, m_Position.y, 0.f);
		glScalef(-1.f, 1.f, 0.f);
	}
	else
	{
		glTranslatef(m_Position.x, m_Position.y, 0.f);
	}
	
	if(m_IsDamaged && (m_FrameNR == 0 || m_FrameNR == 1 || m_FrameNR == 2))
	{
		glTranslatef(-m_Width / 7.f, 1.f, 0.f);
	}
}

void EnemyMike::ResetSprite() const
{
	glPopMatrix();
}

void EnemyMike::CheckHit(const std::vector<Point2f>& Attackbox, bool JustToCheckCollision, bool GetThrownInTheAir)
{

	if(m_IsDamaged == false && m_IsAlive)
	{
		if (utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo))
		{
			if(JustToCheckCollision)
			{
				m_IsColliding = true;
			}
			else
			{
				ResetFrame();
				--m_Health;
				if (m_Health == 0)
				{
					const float JUMP_SPEED{ 1000.f };
					m_EnemyStatus = Status::Falling;
					m_InitialJumpPosition = m_Position;
					m_Velocity.y = JUMP_SPEED;
					m_Position.y += 5.f;
					m_IsFalling = true;
					m_IsAlive = false;
				}
				else if(GetThrownInTheAir)
				{
					const float JUMP_SPEED{ 500.f };
					const float THROWBACK_SPEED{ 400.f };
					m_EnemyStatus = Status::Falling;
					m_InitialJumpPosition = m_Position;
					m_Velocity.y = JUMP_SPEED;
					m_Velocity.x = THROWBACK_SPEED;
					m_Position.y += 5.f;
					m_IsFalling = true;
				}
				else
				{
					m_EnemyStatus = Status::Hit;
					m_IsDamaged = true;
				}
				ResetFrame();
				
			}
		}
	}
}

bool EnemyMike::CheckIdle() const
{
	if (m_IsAttacking || m_IsDamaged || m_IsFalling || m_IsOnTheGround || m_IsAlive == false || m_IsGettingUp || m_IsMoving)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool EnemyMike::GetIsDamaged() const
{
	return m_IsDamaged;
}

bool EnemyMike::GetIsColliding() const
{
	return m_IsColliding;
}

int EnemyMike::GetHealth() const
{
	return m_Health;
}

Point2f EnemyMike::GetPosition() const
{
	return m_Position;
}

void EnemyMike::SetIsLeft(bool IsLeft)
{
	m_IsLeft = IsLeft;
}

void EnemyMike::ResetFrame()
{
	if (m_ChangedState != m_EnemyStatus)
	{
		m_FrameNR = 0.f;
		m_ChangedState = m_EnemyStatus;
		/*std::cout << "Reset Frames" << std::endl;*/
	}
}
