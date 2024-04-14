#include "pch.h"
#include "ScottPilgrim.h"

ScottPilgrim::ScottPilgrim(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_IsAttacking = false;
	m_IsLeft = false;
	m_IsHitboxOn = false;
	m_DidLightAttackHit = false;
	m_DidHeavyAttackHit = false;
	m_IsJumping = false;
	m_WasRunningBeforeJump = false;
	m_IsRunningTrigger = false;
	m_MovingWhenJumping = false;
	m_IsJumpKicking = false;
	m_JumpStartUpAndLand = false;
	m_LightAttackCounter = 0;
	m_HeavyAttackCounter = 0;
	m_Velocity = Vector2f{ 300.f, 150.f };
	m_FrameNR = 0.f;

	m_AnimationCounter = 0.f;
	m_LightAttackDelayCounter = 0.f;
	m_HeavyAttackDelayCounter = 0.f;
	m_JumpDelayCounter = 0.f;
	m_RunningDelayCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("PlayerScott_Sprite.png");
	m_ChangedState = Status::Idle;
	m_ScottStatus = Status::Idle;

	// Make Player Hitbox
	float HitboxWidthLeft{m_Width / 2.f};
	float HitboxHeight{m_Height - m_Height/6.f};
	m_PlayerHitboxOnOrigin.push_back(Point2f(HitboxWidthLeft, 0.f));
	m_PlayerHitboxOnOrigin.push_back(Point2f(0.f, 0.f));
	m_PlayerHitboxOnOrigin.push_back(Point2f(0.f, HitboxHeight));
	m_PlayerHitboxOnOrigin.push_back(Point2f(HitboxWidthLeft, HitboxHeight));

	// Make Attack Hitbox
	float AttackboxWidthLeft{ m_Width / 4.f };
	float AttackboxHeightBot{ m_Height / 3.f };
	float AttackboxWidth{ m_Width / 4.f * 3.f};
	float AttackboxHeight{m_Height - m_Height / 2.5f };
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidth, AttackboxHeightBot));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidthLeft, AttackboxHeightBot));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidthLeft, AttackboxHeight));
	m_AttackBoxOnOrigin.push_back(Point2f(AttackboxWidth, AttackboxHeight));
}

ScottPilgrim::~ScottPilgrim()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void ScottPilgrim::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 18.f }; // Width that will be taken from spritesheet
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 10.f; // Row Height from spritesheet
	float RowIndex = float(m_ScottStatus);

	if(m_ScottStatus == Status::MovingRight)
	{
		RowIndex = float(Status::MovingLeft);
	}
	else if (m_ScottStatus == Status::RunningRight)
	{
		RowIndex = float(Status::RunningLeft);
	}
	

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth - 1.f };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Width, m_Height};

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect, srcRect);
	ResetSprite();

	utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
	utils::DrawPolygon(m_PlayerHitboxTransformed);


	utils::SetColor(Color4f(1.0f, 0.0f, 0, 1.0f));
	utils::DrawPolygon(m_AttackBoxTransformed);
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

		if (m_DidLightAttackHit) m_MaxAnimation = 0.09f;
		else m_MaxAnimation = 0.07f;
		
		if(m_LightAttackCounter == 0)
		{
			m_MaxFrame = 3.f;
		}
		else if(m_LightAttackCounter == 1)
		{
			m_MaxFrame = 7.f;
		}
		else if (m_LightAttackCounter <= 3)
		{
			m_MaxFrame = 10.f;
		}

		
		if (m_FrameNR == 1.f || m_FrameNR == 5.f || m_FrameNR == 8.f)
		{
			m_IsHitboxOn = true;
		}
		else
		{
			m_IsHitboxOn = false;
		}
		UpdateAnimation();
		break;
	case ScottPilgrim::Status::HeavyAttack:
		if (m_DidHeavyAttackHit) m_MaxAnimation = 0.09f;
		else m_MaxAnimation = 0.07f;

		m_MaxFrame = 6.f;
		if (m_FrameNR == 2.f)
		{
			m_IsHitboxOn = true;
		}
		else
		{
			m_IsHitboxOn = false;
		}
		UpdateAnimation();
		break;
	case ScottPilgrim::Status::Jump:

			UpdatePositionDuringJump(elapsedSec);

		break;
	case ScottPilgrim::Status::JumpKick:

			UpdatePositionDuringJump(elapsedSec, true);

			if (m_FrameNR == 3.f || m_FrameNR == 5.f)
			{
				m_IsHitboxOn = true;
			}
			else
			{
				m_IsHitboxOn = false;
			}
		break;
	case ScottPilgrim::Status::Uppercut:

		m_MaxAnimation = 0.09f;

		m_MaxFrame = 7.f;

		if (m_FrameNR == 3.f)
		{
			m_IsHitboxOn = true;
		}
		else
		{
			m_IsHitboxOn = false;
		}

		UpdateAnimation();

		break;
	case ScottPilgrim::Status::RunningRight:
	case ScottPilgrim::Status::RunningLeft:
		m_MaxAnimation = 0.09f;
		m_MaxFrame = 6.f;

		UpdateAnimation();
		break;
	case ScottPilgrim::Status::SpinKick:
		if (m_DidHeavyAttackHit) m_MaxAnimation = 0.09f;
		else m_MaxAnimation = 0.07f;

		m_MaxFrame = 6.f;
		if (m_FrameNR == 2.f)
		{
			m_IsHitboxOn = true;
		}
		else
		{
			m_IsHitboxOn = false;
		}
		UpdateAnimation();
		break;
	}

	if(m_LightAttackCounter > 0)
	{
		m_LightAttackDelayCounter += elapsedSec;
		UpdateLightAttackDelay();
	}

	if(m_HeavyAttackCounter > 0)
	{
		m_HeavyAttackDelayCounter += elapsedSec;
		UpdateHeavyAttackDelay();
	}

	if (m_IsRunningTrigger == true)
	{
		m_RunningDelayCounter += elapsedSec;
		UpdateRunningDelay();
	}

	// Transform Hitboxes
	Matrix2x3 TranslationMat{};
	TranslationMat.SetAsTranslate(Vector2f(m_Position));

	m_PlayerHitboxTransformed = TranslationMat.Transform(m_PlayerHitboxOnOrigin);

	if(m_IsLeft)
	{
		TranslationMat.SetAsTranslate(Vector2f(m_Position.x + m_Width / 2.f, m_Position.y));
		Matrix2x3 ScaleMat{};
		ScaleMat.SetAsScale(-1.f, 1.f);
		Matrix2x3 TransformMat{TranslationMat * ScaleMat};
		m_AttackBoxTransformed = TransformMat.Transform(m_AttackBoxOnOrigin);
	}
	else m_AttackBoxTransformed = TranslationMat.Transform(m_AttackBoxOnOrigin);
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
			if(m_ScottStatus == Status::LightAttack || m_ScottStatus == Status::HeavyAttack || m_ScottStatus == Status::JumpKick || m_ScottStatus == Status::Uppercut || m_ScottStatus == Status::SpinKick)
			{
				m_IsAttacking = false;
				m_IsJumpKicking = false;
				/*std::cout << "Attacking is false." << std::endl;*/
			}
			
			if (m_DidLightAttackHit)
			{
				if(m_LightAttackCounter == 3)
				{
					m_LightAttackCounter = 0;
				}
				else if(m_LightAttackCounter < 3)
				{
					++m_LightAttackCounter;
				}

				m_DidLightAttackHit = false;
			}
			else if(m_DidHeavyAttackHit)
			{
				if (m_HeavyAttackCounter == 2)
				{
					m_HeavyAttackCounter = 0;
				}
				else if (m_HeavyAttackCounter < 2)
				{
					++m_HeavyAttackCounter;
				}

				m_DidHeavyAttackHit = false;
			}
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void ScottPilgrim::UpdateJumpDelay()
{
	if(m_JumpDelayCounter >= m_MAX_JUMP_DELAY)
	{
		m_JumpStartUpAndLand = false;
		m_JumpDelayCounter -= m_MAX_JUMP_DELAY;
	}
}

void ScottPilgrim::UpdateLightAttackDelay()
{
	if(m_LightAttackDelayCounter >= m_MAX_LIGHTATTACK_DELAY)
	{
		m_LightAttackCounter = 0;
		/*std::cout << "No After Hit" << std::endl;*/
		m_LightAttackDelayCounter -= m_MAX_LIGHTATTACK_DELAY;
	}
}

void ScottPilgrim::UpdateRunningDelay()
{
	if (m_RunningDelayCounter >= m_MAX_RUNNING_DELAY)
	{
		m_IsRunningTrigger = false;
		m_RunningDelayCounter -= m_MAX_RUNNING_DELAY;
		//std::cout << "No Running Delay" << std::endl;
	}
}

void ScottPilgrim::UpdateHeavyAttackDelay()
{
	if(m_HeavyAttackDelayCounter >= m_MAX_HEAVYATTACK_DELAY)
	{
		m_HeavyAttackCounter = 0;
		m_HeavyAttackDelayCounter -= m_MAX_HEAVYATTACK_DELAY;
	}
}


void ScottPilgrim::UpdatePositionDuringJump(float elapsedSec, bool isAttacking)
{
	if (m_JumpStartUpAndLand)
	{
		m_JumpDelayCounter += elapsedSec;
		//std::cout << "Counter: " << m_JumpDelayCounter << std::endl;
		m_FrameNR = 0;
		UpdateJumpDelay();
	}
	else
	{
		float HitboxHeight{ m_Height - m_Height / 6.f };

		if(isAttacking)
		{
			if(m_IsJumpKicking)
			{
				m_MaxAnimation = 0.09f;
				m_MaxFrame = 5.f;
				UpdateAnimation();
			}
			else m_FrameNR = 5.f;
			
		}
		else
		{
			if (m_Velocity.y > 580.f) m_FrameNR = 1;
			else if (m_Velocity.y > 300.f) m_FrameNR = 1;
			else if (m_Velocity.y > 150.f) m_FrameNR = 2;
			else if (m_Velocity.y > 0.f) m_FrameNR = 3;
			else if (m_Position.y > m_InitialJumpPosition.y + 2.f * HitboxHeight / 4.f * 2.f) m_FrameNR = 4;
			else if (m_Position.y > m_InitialJumpPosition.y + 2.f * HitboxHeight / 4.f) m_FrameNR = 5;
			else if (m_Position.y > m_InitialJumpPosition.y) m_FrameNR = 6;
		}
		
		if (m_InitialJumpPosition.y < m_Position.y)
		{
			if (m_MovingWhenJumping)
			{
				if (m_IsLeft)
				{
					if(m_WasRunningBeforeJump)
					{
						m_Position.x -= 2.f * m_Velocity.x * elapsedSec;
						m_InitialJumpPosition.x -= 2.f * m_Velocity.x * elapsedSec;
					}
					else
					{
						m_Position.x -= m_Velocity.x * elapsedSec;
						m_InitialJumpPosition.x -= m_Velocity.x * elapsedSec;
					}
				}
				else
				{
					if (m_WasRunningBeforeJump)
					{
						m_Position.x += 2.f * m_Velocity.x * elapsedSec;
						m_InitialJumpPosition.x += 2.f * m_Velocity.x * elapsedSec;
					}
					else
					{
						m_Position.x += m_Velocity.x * elapsedSec;
						m_InitialJumpPosition.x += m_Velocity.x * elapsedSec;
					}
				}

			}
			m_Velocity.y -= 4500.f * elapsedSec;
			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else if (m_Position.y == m_InitialJumpPosition.y && m_MovingWhenJumping == false)
		{
			m_IsJumping = false;
			m_IsAttacking = false;
			/*std::cout << "Jumping is false." << std::endl;*/
			m_AnimationCounter = 0.f;
		}
		else
		{
			m_ScottStatus = Status::Jump;
			m_FrameNR = 0;
			m_Position.y = m_InitialJumpPosition.y;
			m_Velocity.y = 150.f;
			m_JumpStartUpAndLand = true;
			m_MovingWhenJumping = false;
			m_WasRunningBeforeJump = false;
		}
	}
}

void ScottPilgrim::CheckKeys(float elapsedSec, bool moveRight, bool moveLeft, bool moveUp, bool moveDown)
{
	if(m_IsAttacking == false && m_IsJumping == false)
	{
		ResetFrame();
		if (moveRight)
		{
			m_IsLeft = false;
			if(m_ScottStatus == Status::RunningRight)m_Position.x += 2.f * m_Velocity.x * elapsedSec;
			else
			{
				m_Position.x += m_Velocity.x * elapsedSec;
				m_ScottStatus = Status::MovingRight;
			}
			//m_Transform.Rotation.x = 0.f;
		}
		else if (moveLeft)
		{
			m_IsLeft = true;
			if (m_ScottStatus == Status::RunningLeft)m_Position.x -= 2.f * m_Velocity.x * elapsedSec;
			else
			{
				m_Position.x -= m_Velocity.x * elapsedSec;
				m_ScottStatus = Status::MovingLeft;
			}
			//m_Transform.Rotation.x += 90.f * elapsedSec;
		}

		if (moveUp)
		{
			if ((m_ScottStatus == Status::RunningRight || m_ScottStatus == Status::RunningLeft) && (moveRight || moveLeft))m_Position.y += m_Velocity.y * elapsedSec;
			else
			{
				m_ScottStatus = Status::MovingRight;
				m_Position.y += m_Velocity.y * elapsedSec;
			}
		}
		else if (moveDown)
		{
			if ((m_ScottStatus == Status::RunningRight || m_ScottStatus == Status::RunningLeft) && (moveRight || moveLeft))m_Position.y -= m_Velocity.y * elapsedSec;
			else
			{
				m_ScottStatus = Status::MovingLeft;
				m_Position.y -= m_Velocity.y * elapsedSec;
			}
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

void ScottPilgrim::Attack(bool lightAttack, bool heavyAttack, bool jumpAttack, bool uppercut)
{
	ResetFrame();
	if (lightAttack)	m_ScottStatus = Status::LightAttack;
	else if (heavyAttack && m_HeavyAttackCounter == 1) m_ScottStatus = Status::SpinKick;
	else if (heavyAttack) m_ScottStatus = Status::HeavyAttack;
	else if(uppercut) m_ScottStatus = Status::Uppercut;
	else if (jumpAttack)
	{
		m_ScottStatus = Status::JumpKick;
		m_IsJumpKicking = true;
		m_AnimationCounter = 0.f;
	}
	ResetFrame();
	m_IsAttacking = true;
}

void ScottPilgrim::Jump(bool moving, bool WasRunningBeforeJump)
{
	ResetFrame();

	const float JUMP_SPEED{ 1500.f };

	m_ScottStatus = Status::Jump;
	m_Velocity.y = JUMP_SPEED;
	m_InitialJumpPosition = m_Position;
	m_Position.y += 5.f;
	m_IsJumping = true;
	m_JumpStartUpAndLand = true;
	m_MovingWhenJumping = moving;
	m_WasRunningBeforeJump = WasRunningBeforeJump;
}

void ScottPilgrim::TranslateSprite() const
{
	// Player Transform
	glPushMatrix();

	if(m_IsLeft)
	{
		glTranslatef(m_Position.x + m_Width / 2.f, m_Position.y, 0.f);
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
	if(m_IsAttacking || m_IsJumping)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ScottPilgrim::CheckIfHitboxIsOn() const
{
	return m_IsHitboxOn;
}

void ScottPilgrim::LightAttackCounterIncrement(bool IsHit)
{
	if(IsHit)
	{
		m_DidLightAttackHit = true;
		/*std::cout << "Hit: " << m_LightAttackCounter << std::endl;*/
		m_LightAttackDelayCounter = 0.f;
	}
}

void ScottPilgrim::HeavyAttackCounterIncrement(bool IsHit)
{
	if(IsHit)
	{
		m_DidHeavyAttackHit = true;
		m_HeavyAttackDelayCounter = 0.f;
	}
}


bool ScottPilgrim::GetIsHit() const
{
	return (m_DidLightAttackHit || m_DidHeavyAttackHit);
}

bool ScottPilgrim::GetIsJumping() const
{
	return m_IsJumping;
}

bool ScottPilgrim::GetIsRunningTrigger() const
{
	return m_IsRunningTrigger;
}

int ScottPilgrim::GetHeavyAttackCounter() const
{
	return m_HeavyAttackCounter;
}

Point2f ScottPilgrim::GetPosition() const
{
	if (m_IsJumping) return m_InitialJumpPosition;
	else return m_Position;
	
}

float ScottPilgrim::GetWidth() const
{
	return m_Width;
}

std::vector<Point2f> ScottPilgrim::GetAttackBox() const
{
	return std::vector<Point2f>(m_AttackBoxTransformed);
}

void ScottPilgrim::ResetFrame()
{
	if(m_ChangedState != m_ScottStatus)
	{
		if(m_ScottStatus == Status::LightAttack)
		{
			if (m_LightAttackCounter == 1)
			{
				m_FrameNR = 3.f;
			}
			else if (m_LightAttackCounter == 2 || m_LightAttackCounter == 3)
			{
				m_FrameNR = 7.f;
			}
			else  m_FrameNR = 0.f;
		}
		else m_FrameNR = 0.f;

		m_ChangedState = m_ScottStatus;
	}
}

void ScottPilgrim::SetIsRunningTrigger(bool IsRunningTrigger)
{
	m_IsRunningTrigger = IsRunningTrigger;
	//std::cout << "RunningTrigger: " << m_IsRunningTrigger << std::endl;
}


//void ScottPilgrim::CheckKeys(float elapsedSec, bool KeyPressed)
//{
//	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
//
//	if ((m_IsAttacking == false) && KeyPressed)
//	{
//		ResetFrame();
//
//		//Check Keys
//		
//		if (pStates[SDL_SCANCODE_UP])
//		{
//			m_ScottStatus = Status::MovingRight;
//			m_Position.y += m_Velocity.y * elapsedSec;
//		}
//		if (pStates[SDL_SCANCODE_DOWN])
//		{
//			m_ScottStatus = Status::MovingLeft;
//			m_Position.y -= m_Velocity.y * elapsedSec;
//		}
//		if (pStates[SDL_SCANCODE_LEFT])
//		{
//			m_ScottStatus = Status::MovingLeft;
//			m_IsLeft = true;
//			m_Position.x -= m_Velocity.x * elapsedSec;
//		}
//		if (pStates[SDL_SCANCODE_RIGHT])
//		{
//			m_ScottStatus = Status::MovingRight;
//			m_IsLeft = false;
//			m_Position.x += m_Velocity.x * elapsedSec;
//		}
//		if (pStates[SDL_SCANCODE_J])
//		{
//			Attack(true);
//		}
//
//		if (m_Position.y <= -3.0f)
//		{
//			m_Position.y = -3.0f;
//		}
//		else if (m_Position.y >= 430.0f)
//		{
//			m_Position.y = 430.0f;
//		}
//
//		if (m_Position.x <= 0.0f)
//		{
//			m_Position.x = 0.0f;
//		}
//
//	}
//	else
//	{
//		m_ScottStatus = Status::Idle;
//	}
//}