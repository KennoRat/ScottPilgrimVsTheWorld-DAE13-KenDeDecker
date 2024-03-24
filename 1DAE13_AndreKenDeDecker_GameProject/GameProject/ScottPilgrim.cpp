#include "pch.h"
#include "ScottPilgrim.h"

ScottPilgrim::ScottPilgrim(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_IsAttacking = false;
	m_IsLeft = false;
	m_IsHitboxOn = false;
	m_LightAttackCounter = 0;
	m_Velocity = Vector2f{ 300.f, 150.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_AttackDelayCounter = 0.f;
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
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 17.f }; // Width that will be taken from spritesheet
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 5.f; // Row Height from spritesheet
	float RowIndex = float(m_ScottStatus);

	if(m_ScottStatus == Status::MovingRight)
	{
		RowIndex = float(Status::MovingLeft);
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
		m_MaxAnimation = 0.10f;
		if(m_LightAttackCounter <= 1)
		{
			m_MaxFrame = 3.f;
		}
		else if(m_LightAttackCounter >= 3)
		{
			m_MaxFrame = 7.f;
		}
		
		if (m_FrameNR == 1.f || m_FrameNR == 5.f)
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
		m_AttackDelayCounter += elapsedSec;
		UpdateAttackDeley();
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
			m_IsAttacking = false;
			/*m_LightAttackCounter = 0;*/
		}
		m_AnimationCounter -= m_MaxAnimation;
	}
}

void ScottPilgrim::UpdateAttackDeley()
{
	if(m_AttackDelayCounter >= m_MAX_ATTACK_DELAY)
	{
		m_LightAttackCounter = 0;
		std::cout << "No After Hit" << std::endl;
		m_AttackDelayCounter -= m_MAX_ATTACK_DELAY;
	}
}

void ScottPilgrim::CheckKeys(float elapsedSec, bool moveRight, bool moveLeft, bool moveUp, bool moveDown)
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
	ResetFrame();
	if (m_LightAttackCounter >= 2)
	{
		m_FrameNR = 3.f;
	}
	m_IsAttacking = true;
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
	if(m_IsAttacking)
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

void ScottPilgrim::LightAttackCounter(bool IsHit)
{
	if(IsHit)
	{
		++m_LightAttackCounter;
		std::cout << "HIt: " << m_LightAttackCounter << std::endl;
		m_AttackDelayCounter = 0.f;
	}
}


Point2f ScottPilgrim::GetPosition() const
{
	return m_Position;
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
		m_FrameNR = 0.f;
		m_ChangedState = m_ScottStatus;
	}
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