#include "pch.h"
#include "EnemyMike.h"

Texture* EnemyMike::m_ptrSpriteSheet{ nullptr };
int EnemyMike::m_InstanceCounter{ 0 };


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
	m_IsAttackBoxOn = false;
	m_AttackBoxReset = false;
	m_IsTaunting = false;
	m_StayOnGround = true;
	m_IsBlocking = false;
	m_BlockingCounter = false;
	m_IsAggressive = false;
	m_IsStunned = false;
	m_BeDrawn = true;
	m_SpawnCoins = false;
	m_Health = 10;
	m_GotLightHitAmount = 0;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_InitialJumpPosition = Point2f{0.f, 0.f};
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;

	m_ChoicesDelayCounter = 0.f;
	m_MaxChoiceDelay = 3.f;
	m_StayOnTheGroundCounter = 0.f;
	m_StunnedCounter = 0.f;
	m_DeathCounter = 0.f;
	m_DissolveCounter = 0.f;

	//m_ptrSpriteSheet = new Texture("EnemyMike_Sprite.png");
	m_ChangedState = Status::Idle;
	m_EnemyStatus = Status::Idle;


	// Make Enemy Hitbox
	float HitboxWidth{ m_Width / 2.f};
	float HitboxHeight{ m_Height - m_Height / 6.f };
	m_HitboxOnOrigin.push_back(Point2f(0.f,0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidth, 0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidth, HitboxHeight));
	m_HitboxOnOrigin.push_back(Point2f(0.f, HitboxHeight));

	// Make Attackbox
	float AttackboxWidthLeft{ m_Width / 4.f };
	float AttackboxHeightBot{ m_Height / 3.f };
	float AttackboxWidth{ m_Width / 3.f * 2.f };
	float AttackboxHeight{ m_Height - m_Height / 2.f };
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidth, AttackboxHeightBot));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidthLeft, AttackboxHeightBot));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidthLeft, AttackboxHeight));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidth, AttackboxHeight));

	++m_InstanceCounter;

	if(m_InstanceCounter == 1)
	{
		EnemyMike::m_ptrSpriteSheet = new Texture("EnemyMike_Sprite.png");
	}
}

EnemyMike::~EnemyMike()
{
	if (m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_InstanceCounter;
}

EnemyMike::EnemyMike(const EnemyMike& other): EnemyMike{other.m_Position, other.m_Width, other.m_Height}
{
	m_IsLeft = other.m_IsLeft;
	m_Health = other.m_Health;
}

EnemyMike& EnemyMike::operator=(const EnemyMike& other)
{
	if (this != &other)
	{
		m_Position = other.m_Position;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_IsLeft = other.m_IsLeft;
		m_Health = other.m_Health;
	}
	return *this;
}

EnemyMike::EnemyMike(EnemyMike&& other) noexcept 
	: m_Position{ std::move(other.m_Position) }
	, m_Width{ std::move(other.m_Width) }
	, m_Height{ std::move(other.m_Height) }
{
	other.m_ptrSpriteSheet = nullptr;
}

EnemyMike& EnemyMike::operator=(EnemyMike&& other) noexcept
{
	if (this != &other)
	{
		m_Position = std::move(other.m_Position);
		m_Width = std::move(other.m_Width);
		m_Height = std::move(other.m_Height);
		m_IsLeft = std::move(other.m_IsLeft);
		m_Health = std::move(other.m_Health);
		other.m_ptrSpriteSheet = nullptr;
	}
	return *this;
}

void EnemyMike::Draw() const
{
	if(m_BeDrawn)
	{
		float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 8.f };
		float RowHeigth = m_ptrSpriteSheet->GetHeight() / 18.f;
		float RowIndex = float(m_EnemyStatus);

		Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth - 1.f };
		Rectf dstRect = Rectf{ 0.f, 0.f, m_Width, m_Height };

		TranslateSprite();
		m_ptrSpriteSheet->Draw(dstRect, srcRect);
		ResetSprite();

		utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
		utils::DrawPolygon(m_HitboxTransformed);

		utils::SetColor(Color4f(1.0f, 0.0f, 0, 1.0f));
		utils::DrawPolygon(m_AttackBoxTransformed);
	}
}

void EnemyMike::Update(float elapsedSec, const Point2f& PlayerPosition)
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
			m_ChoicesDelayCounter += elapsedSec;
			UpdateChoicesDelay(PlayerPosition);
		}
		break;
	case EnemyMike::Status::Walking:
		m_MaxFrame = 5.f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::Hit:
		if(m_GotLightHitAmount == 3)
		{
			m_IsStunned = true;
			m_IsDamaged = false;
			m_StunnedCounter += elapsedSec;
			UpdateStunned();
		}
		else if(m_GotLightHitAmount == 4)
		{
			m_MaxFrame = 6.f;
			m_MaxAnimation = 0.16f;
			UpdateAnimation();
		}
		else
		{
			m_MaxFrame = 2.f;
			m_MaxAnimation = 0.09f;
			m_ChoicesDelayCounter += elapsedSec;
			UpdateAnimation();
		}
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

		if(m_IsAlive)
		{
			m_StayOnTheGroundCounter += elapsedSec;
			UpdateStayOnTheGround();
		}
		else
		{
			m_DeathCounter += elapsedSec;
			m_DissolveCounter += elapsedSec;
			UpdateDeath();
		}

		break;
	case EnemyMike::Status::GettingUp:

		m_MaxFrame = 6.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		break;
	case EnemyMike::Status::LightAttack:

		m_MaxFrame = 7.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		if (m_FrameNR == 1.f || m_FrameNR == 5.f)
		{
			m_IsAttackBoxOn = true;
		}
		else
		{
			m_AttackBoxReset = true;
			m_IsAttackBoxOn = false;
		}
		break;

	case EnemyMike::Status::Sprinting:

		if(m_IsAggressive)
		{
			float PlayerDistance{ 180.f };
			if (m_Position.x < PlayerPosition.x) m_NewPosition.x = PlayerPosition.x - PlayerDistance;
			else m_NewPosition.x = PlayerPosition.x + PlayerDistance;
			m_NewPosition.y = PlayerPosition.y;
		}
	
		m_MaxFrame = 7.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();
		break;

	case EnemyMike::Status::Taunt:

		m_MaxFrame = 3.f;
		m_MaxAnimation = 0.20f;
		UpdateAnimation();

		break;

	case EnemyMike::Status::SpinKick:
		m_MaxFrame = 6.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		if (m_FrameNR == 3.f)
		{
			m_IsAttackBoxOn = true;
		}
		else
		{
			m_AttackBoxReset = true;
			m_IsAttackBoxOn = false;
		}
		break;
	case EnemyMike::Status::Block:
		m_MaxFrame = 2.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		m_BlockingCounter += elapsedSec;
		UpdateKeepBlocking(elapsedSec);
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

	if (m_IsLeft)
	{
		TranslationMat.SetAsTranslate(Vector2f(m_Position.x + m_Width / 2.f, m_Position.y));
		Matrix2x3 ScaleMat{};
		ScaleMat.SetAsScale(-1.f, 1.f);
		Matrix2x3 TransformMat{ TranslationMat * ScaleMat };
		m_AttackBoxTransformed = TransformMat.Transform(m_AttackBoxOnOrigin);
	}
	else m_AttackBoxTransformed = TranslationMat.Transform(m_AttackBoxOnOrigin);

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
			m_IsDamaged = false;
			m_IsColliding = false;
			m_IsGettingUp = false;
			m_IsAttacking = false;
			m_IsTaunting = false;
			m_IsStunned = false;

			if ((m_IsAlive == false || m_StayOnGround) && m_EnemyStatus == Status::OnTheGround)
			{
				m_FrameNR = 7.f;
			}
			else if(m_IsAlive == true && m_EnemyStatus == Status::OnTheGround && m_StayOnGround == false)
			{
				std::cout << "Getting Up is True" << std::endl;
				m_IsOnTheGround = false;
				m_EnemyStatus = Status::GettingUp;
				m_IsGettingUp = true;
				m_StayOnGround = true;
			}	

			if(m_IsAlive && (m_EnemyStatus == Status::LightAttack || m_EnemyStatus == Status::SpinKick))
			{
				int Randommove{ rand() % 2 };

				if(Randommove == 0)
				{
					int MoveDistance{ 100 };

					m_IsMoving = true;
					m_NewPosition.x = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.x;
					m_NewPosition.y = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.y;
					m_EnemyStatus = Status::Walking;
				}
			}

			if (m_GotLightHitAmount == 4)
			{
				m_GotLightHitAmount = 0;
			}
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void EnemyMike::UpdateChoicesDelay(const Point2f& PlayerPosition)
{
	if(m_ChoicesDelayCounter >= m_MaxChoiceDelay)
	{
		int RandomChoice{ rand() % 100 };

		m_GotLightHitAmount = 0;

		if(RandomChoice >= 60)
		{
			int MoveDistance{ 500 };

			m_IsMoving = true;
			m_NewPosition.x = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.x;
			m_NewPosition.y = PlayerPosition.y;
			m_EnemyStatus = Status::Walking;
		}
		else if(RandomChoice >= 15)
		{
			int MoveDistance{ 500 };
			float PlayerDistance{ 180.f };
			int IsAggressive(rand() % 2);

			if (IsAggressive == 0) m_IsAggressive = true;
			else m_IsAggressive = false;

			m_IsMoving = true;
			if(m_Position.x < PlayerPosition.x) m_NewPosition.x = PlayerPosition.x - PlayerDistance;
			else m_NewPosition.x = PlayerPosition.x + PlayerDistance;
			m_NewPosition.y = PlayerPosition.y;
			m_EnemyStatus = Status::Sprinting;
		}
		else if(RandomChoice >= 0)
		{
			m_FrameNR = 0.f;
			m_IsTaunting = true;
			m_EnemyStatus = Status::Taunt;
		}



		m_ChoicesDelayCounter -= m_MaxChoiceDelay;
		m_MaxChoiceDelay = float(rand() % 5 + 3);
		//std::cout << "Moving Is True" << std::endl;
	}
}

void EnemyMike::UpdateStayOnTheGround()
{
	if(m_StayOnTheGroundCounter >= m_MAX_STAY_ON_THE_GROUND_DELAY)
	{
		std::cout << "Stay on the ground" << std::endl;
		m_StayOnTheGroundCounter -= m_MAX_STAY_ON_THE_GROUND_DELAY;
		m_StayOnGround = false;
	}

}

void EnemyMike::UpdateKeepBlocking(float elapsedSec)
{
	if(m_BlockingCounter >= m_MAX_BLOCKING_DELAY)
	{
		std::cout << "Stop Blocking" << std::endl;
		m_IsBlocking = false;
		m_BlockingCounter -= m_MAX_BLOCKING_DELAY;
	}

	if (m_IsLeft)m_Position.x += m_Velocity.x * elapsedSec;
	else m_Position.x -= m_Velocity.x * elapsedSec;
}

void EnemyMike::UpdateStunned()
{
	if(m_StunnedCounter >= m_MAX_STUNNED_DELAY)
	{
		m_FrameNR = 3.f;
		m_AnimationCounter = 0.f;
		m_GotLightHitAmount = 4;
		m_StunnedCounter -= m_MAX_STUNNED_DELAY;
	}
	else
	{
		m_FrameNR = 3.f;
	}

}

void EnemyMike::UpdateDeath()
{
	if(m_DeathCounter >= m_MAX_DEATH_DELAY)
	{
		m_SpawnCoins = true;
		m_BeDrawn = false;
	}
	else if(m_DissolveCounter >= m_MAX_DISSOLVE_DELAY)
	{
		m_BeDrawn = !m_BeDrawn;
		m_DissolveCounter -= m_MAX_DISSOLVE_DELAY;
	}
}

void EnemyMike::GoToRandomPosition(float elapsedSec)
{
	Vector2f Speed{};

	float DistanceRange{3.f};
	float Top{ 430.0f };
	float Bottom{ -3.f };

	if (m_EnemyStatus == Status::Sprinting)
	{
		Speed.x = 2.f * m_Velocity.x;
		Speed.y = 2.f * m_Velocity.y;
	}
	else Speed = m_Velocity;

	if (m_NewPosition.x < m_Position.x + DistanceRange && m_NewPosition.x > m_Position.x - DistanceRange) m_NewPosition.x = m_Position.x;
	else if (m_NewPosition.x > m_Position.x) m_Position.x += Speed.x * elapsedSec;
	else if (m_NewPosition.x < m_Position.x) m_Position.x -= Speed.x * elapsedSec;


	if (m_NewPosition.y < m_Position.y + DistanceRange && m_NewPosition.y > m_Position.y - DistanceRange) m_NewPosition.y = m_Position.y;
	else if (m_NewPosition.y > m_Position.y) m_Position.y += Speed.y * elapsedSec;
	else if (m_NewPosition.y < m_Position.y) m_Position.y -= Speed.y * elapsedSec;
	
	if (m_Position.y <= -Bottom)
	{
		m_NewPosition.y = -Bottom;
		m_Position.y = -Bottom;
	}
	else if (m_Position.y >= Top)
	{
		m_NewPosition.y = Top;
		m_Position.y = Top;
	}

	if (m_NewPosition.x == m_Position.x && m_NewPosition.y == m_Position.y)
	{
		if (m_EnemyStatus == Status::Sprinting)
		{
			int RandomChoice{ rand() % 2 };
			m_IsAttacking = true;
			if(RandomChoice == 0)
			{
				m_EnemyStatus = Status::LightAttack;
			}
			else if(RandomChoice == 1)
			{
				m_EnemyStatus = Status::SpinKick;
			}
		}
		m_IsMoving = false;
		m_IsAggressive = false;
		m_FrameNR = 0.f;
	}
}

void EnemyMike::Block()
{
	m_EnemyStatus = Status::Block;

	m_IsBlocking = true;
	m_IsDamaged = true;
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

void EnemyMike::CheckHit(const std::vector<Point2f>& Attackbox, bool JustToCheckCollision, bool GetThrownInTheAir, bool GetUppercut)
{

	if(m_IsDamaged == false && m_IsAlive && m_EnemyStatus != Status::OnTheGround)
	{
		if (utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo))
		{
			int ChanceToBlock{ rand() % 100 };
			if(ChanceToBlock >= 80 && m_GotLightHitAmount < 3)
			{
				Block();
			}
			else if (m_IsBlocking == false)
			{
				if (JustToCheckCollision)
				{
					m_IsColliding = true;
				}
				else
				{
					ResetFrame();
					--m_Health;
					m_IsMoving = false;
					m_IsAttacking = false;
					m_IsAggressive = false;

					if (m_Health == 0) m_IsAlive = false;

					if (m_IsAlive == false && GetUppercut == false && GetThrownInTheAir == false)
					{
						const float JUMP_SPEED{ 200.f };
						const float THROWBACK_SPEED{ 100.f };
						m_EnemyStatus = Status::Falling;
						m_InitialJumpPosition = m_Position;
						m_Velocity.y = JUMP_SPEED;
						m_Velocity.x = THROWBACK_SPEED;
						m_Position.y += 5.f;
						m_IsFalling = true;
						m_GotLightHitAmount = 0;
					}
					else if (GetUppercut)
					{
						const float JUMP_SPEED{ 1000.f };
						m_EnemyStatus = Status::Falling;
						m_InitialJumpPosition = m_Position;
						m_Velocity.y = JUMP_SPEED;
						m_Position.y += 5.f;
						m_IsFalling = true;
						m_GotLightHitAmount = 0;
					}
					else if (GetThrownInTheAir)
					{
						const float JUMP_SPEED{ 500.f };
						const float THROWBACK_SPEED{ 800.f };
						m_EnemyStatus = Status::Falling;
						m_InitialJumpPosition = m_Position;
						m_Velocity.y = JUMP_SPEED;
						m_Velocity.x = THROWBACK_SPEED;
						m_Position.y += 5.f;
						m_IsFalling = true;
						m_GotLightHitAmount = 0;
					}
					else
					{
						m_EnemyStatus = Status::Hit;
						++m_GotLightHitAmount;
						m_IsDamaged = true;
					}
					ResetFrame();

				}
			}
		}
	}
}

bool EnemyMike::CheckIdle() const
{
	if (m_IsAttacking || m_IsDamaged || m_IsFalling || m_IsOnTheGround || m_IsAlive == false || m_IsGettingUp || m_IsMoving || m_IsTaunting || m_IsBlocking || m_IsStunned)
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

bool EnemyMike::GetIsLeft() const
{
	return m_IsLeft;
}

bool EnemyMike::GetSpawnCoins() const
{
	return m_SpawnCoins;
}

int EnemyMike::GetHealth() const
{
	return m_Health;
}

int EnemyMike::GetGotLightHitAmount() const
{
	return m_GotLightHitAmount;
}

Point2f EnemyMike::GetPosition() const
{
	return m_Position;
}

bool EnemyMike::CheckIfAttackBoxIsOn()
{
	if (m_AttackBoxReset && m_IsAttackBoxOn)
	{
		// Resets attackbox so it doesn't hit player multiple times
		m_AttackBoxReset = false;
		return true;
	}
	else false;
}

std::vector<Point2f> EnemyMike::GetAttackBox() const
{
	return std::vector<Point2f>(m_AttackBoxTransformed);
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
