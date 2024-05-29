#include "pch.h"
#include "EnemyMike.h"

Texture* EnemyMike::m_ptrMikeSpriteSheet{ nullptr };
Texture* EnemyMike::m_ptrLeeSpriteSheet{ nullptr };
Texture* EnemyMike::m_ptrLukeSpriteSheet{ nullptr };
Texture* EnemyMike::m_ptrRichardSpriteSheet{ nullptr };
int EnemyMike::m_MikeInstanceCounter{ 0 };
int EnemyMike::m_LeeInstanceCounter{ 0 };
int EnemyMike::m_LukeInstanceCounter{ 0 };
int EnemyMike::m_RichardInstanceCounter{ 0 };



EnemyMike::EnemyMike(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType): m_Position{position}, m_Width{width}, m_Height{height}, m_ptrSoundEffects{SoundEffects}, m_EnemyType{EnemyType}
{
	m_IsAlive = true;
	m_IsAttacking = false;
	m_IsDamaged = false;
	m_IsColliding = false;
	m_IsLeft = true;
	m_IsFalling = false;
	m_IsOnTheGround = false;
	m_IsGettingUp = false;
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
	m_HasPickUpObject = false;
	m_IsPickingUp = false;
	m_FlipObject = false;
	m_ThrowObject = false;
	m_IsGoingToThrow = false;
	m_IsHit = false;
	m_DamagedWhileStunned = false;

	m_Health = 20;
	m_GotLightHitAmount = 0;
	m_ObjectRumble = 0;
	m_BlockChancePercent = 15;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_InitialJumpPosition = Point2f{0.f, 0.f};
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;

	//Move Forward When Spawning
	m_IsMoving = true;
	m_JustSpawned = true;
	float RandomForwardPosition{float(rand() % 100 + 250)};
	m_NewPosition = Point2f{ m_Position.x - RandomForwardPosition, m_Position.y};

	//Class Association
	m_ptrHoldingObject = nullptr;

	//Time
	m_ChoicesDelayCounter = 0.f;
	m_MaxChoiceDelay = float(rand() % 5 + 3);
	m_StayOnTheGroundCounter = 0.f;
	m_StunnedCounter = 0.f;
	m_DeathCounter = 0.f;
	m_DissolveCounter = 0.f;

	m_ChangedState = Status::Walking;
	m_EnemyStatus = Status::Walking;


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


	//Textures and Enemy Type
	if (m_EnemyType == "Mike")
	{
		++m_MikeInstanceCounter;


		if (m_MikeInstanceCounter == 1)
		{
			m_ptrMikeSpriteSheet = new Texture("EnemyMike_Sprite.png");
		}

		m_ptrSpriteSheet = m_ptrMikeSpriteSheet;
	}
	else if(m_EnemyType == "Lee")
	{
		++m_LeeInstanceCounter;

		if(m_LeeInstanceCounter == 1)
		{
			m_ptrLeeSpriteSheet = new Texture("EnemyLee_Sprite.png");
		}

		m_ptrSpriteSheet = m_ptrLeeSpriteSheet;
	}
	else if(m_EnemyType == "Luke")
	{
		++m_LukeInstanceCounter;

		if (m_LukeInstanceCounter == 1)
		{
			m_ptrLukeSpriteSheet = new Texture("EnemyLuke_Sprite.png");
		}

		m_ptrSpriteSheet = m_ptrLukeSpriteSheet;
	}
	else if (m_EnemyType == "Richard")
	{
		++m_RichardInstanceCounter;

		if (m_RichardInstanceCounter == 1)
		{
			m_ptrRichardSpriteSheet = new Texture("EnemyRichard_Sprite.png");
		}

		m_ptrSpriteSheet = m_ptrRichardSpriteSheet;
	}
	
}

EnemyMike::~EnemyMike()
{
	if (m_EnemyType == "Mike")
	{
		if (m_MikeInstanceCounter == 1)
		{
			delete m_ptrSpriteSheet;
			m_ptrSpriteSheet = nullptr;
		}

		--m_MikeInstanceCounter;
	}
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

void EnemyMike::Update(float elapsedSec, const Point2f& PlayerPosition, const std::vector<std::vector<Point2f>>& MapSvg, const Point2f& ObjectPosition, bool ObjectIsLeft)
{
	m_AnimationCounter += elapsedSec;

	float HitboxHeight{ m_Height - m_Height / 6.f };

	switch (m_EnemyStatus)
	{
	case EnemyMike::Status::PickUpIdle:

		if (m_FrameNR == 2.f) m_ObjectRumble = 2.f;
		else m_ObjectRumble = 0.f;

	case EnemyMike::Status::Idle:
		m_MaxFrame = 3.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();

		if(m_IsMoving == false)
		{
			m_ChoicesDelayCounter += elapsedSec;
			if(ObjectPosition.x != 0.f && ObjectPosition.y != 0.f) UpdateChoicesDelay(PlayerPosition, ObjectPosition);
			else UpdateChoicesDelay(PlayerPosition);
		}
		break;
	case EnemyMike::Status::PickUpWalk:

		if (m_FrameNR == 5.f || m_FrameNR == 2.f) m_ObjectRumble = 2.f;
		if (m_FrameNR == 4.f || m_FrameNR == 1.f) m_ObjectRumble = 4.f;
		else m_ObjectRumble = 0.f;

	case EnemyMike::Status::Walking:
		m_MaxFrame = 5.f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::Hit:
		if(m_GotLightHitAmount == 3)
		{
			m_IsStunned = true;
			if(m_DamagedWhileStunned)
			{
				m_IsDamaged = false;
				m_DamagedWhileStunned = false;
			}
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
			m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::HitTheGround);
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
	case EnemyMike::Status::PickUpRun:

		if (m_FrameNR == 2.f || m_FrameNR == 4.f || m_FrameNR == 6.f) m_ObjectRumble = 20.f;
		else m_ObjectRumble = 10.f;

	case EnemyMike::Status::Sprinting:

		if(m_IsAggressive)
		{
			float PlayerDistance{ 180.f };
			if (m_Position.x < PlayerPosition.x)
			{
				m_NewPosition.x = PlayerPosition.x - PlayerDistance;
				m_IsLeft = false;
			}
			else
			{
				m_NewPosition.x = PlayerPosition.x + PlayerDistance;
				m_IsLeft = true; 
			}
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

		break;
	case EnemyMike::Status::PickUp:
		m_MaxFrame = 1.f;
		m_MaxAnimation = 0.15f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::PickUpAttack:
		m_MaxFrame = 6.f;
		m_MaxAnimation = 0.10f;

		if (m_FrameNR == 4.f)m_IsAttackBoxOn = true;
		else
		{
			m_AttackBoxReset = true;
			m_IsAttackBoxOn = false;
		}

		if (m_FrameNR >= 3.f) m_FlipObject = true;
		else m_FlipObject = false;

		UpdateAnimation();
		break;
	case EnemyMike::Status::PickUpThrow:
		m_MaxFrame = 6.f;
		m_MaxAnimation = 0.10f;

		if (m_FrameNR == 3) m_ThrowObject = true;
		else m_ThrowObject = false;

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

	if (m_IsLeft)
	{
		TranslationMat.SetAsTranslate(Vector2f(m_Position.x + m_Width / 2.f, m_Position.y));
		Matrix2x3 ScaleMat{};
		ScaleMat.SetAsScale(-1.f, 1.f);
		Matrix2x3 TransformMat{ TranslationMat * ScaleMat };
		m_AttackBoxTransformed = TransformMat.Transform(m_AttackBoxOnOrigin);
	}
	else m_AttackBoxTransformed = TranslationMat.Transform(m_AttackBoxOnOrigin);

	if(m_IsStunned)
	{
		m_StunnedCounter += elapsedSec;
		UpdateStunned();
	}

	if(m_IsBlocking)
	{
		m_BlockingCounter += elapsedSec;
		UpdateKeepBlocking(elapsedSec);
	}

	if (m_IsMoving) GoToRandomPosition(elapsedSec);

	if(m_JustSpawned == false) CheckIfGoingOutOfBounds(MapSvg);

	if (m_IsHit == true) m_IsHit = false;
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
			m_FlipObject = false;
			m_IsAttackBoxOn = false;
			
			if(m_EnemyStatus != Status::Sprinting) m_IsPickingUp = false;

			if(m_EnemyStatus == Status::PickUpThrow)
			{
				m_HasPickUpObject = false;
				m_ptrHoldingObject = nullptr;
				m_ThrowObject = false;
			}

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

			if(m_IsAlive && (m_EnemyStatus == Status::LightAttack || m_EnemyStatus == Status::SpinKick) && m_IsPickingUp == false)
			{
				int Randommove{ rand() % 2 };

				if(Randommove == 0)
				{
					int MoveDistance{ 100 };

					m_IsMoving = true;
					m_NewPosition.x = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.x;
					m_NewPosition.y = float(rand() % MoveDistance - MoveDistance / 2) + m_Position.y;
					if(m_HasPickUpObject) m_EnemyStatus = Status::PickUpWalk;
					else m_EnemyStatus = Status::Walking;
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

void EnemyMike::UpdateChoicesDelay(const Point2f& PlayerPosition, const Point2f& ObjectPosition, bool ObjectIsLeft)
{
	if (m_ChoicesDelayCounter >= m_MaxChoiceDelay)
	{
		int RandomChoice{ rand() % 100 };

		if (m_HasPickUpObject) RandomChoice -= 10;

		if (RandomChoice >= 70)
		{
			int MoveXDistance{ 250 };
			int MoveYDistance{ 50 };
			int RandomPos{ rand() % 2 };

			m_IsMoving = true;
			if (RandomPos == 0) m_NewPosition.x = PlayerPosition.x - float(rand() % MoveXDistance + 300.f);
			else m_NewPosition.x = PlayerPosition.x + float(rand() % MoveXDistance + 300.f);
			m_NewPosition.y = PlayerPosition.y + float(rand() % MoveYDistance - (MoveYDistance / 2.f));
			if (m_HasPickUpObject) m_EnemyStatus = Status::PickUpWalk;
			else m_EnemyStatus = Status::Walking;
		}
		else if (RandomChoice >= 40 && (ObjectPosition.x != 0.f && ObjectPosition.y != 0.f) && m_HasPickUpObject == false)
		{
			float ObjectDistance{ 90.f };

			m_IsMoving = true;
			m_IsLeft = true;
			if (ObjectIsLeft)
			{
				m_NewPosition.x = ObjectPosition.x - ObjectDistance;
			}
			else
			{
				m_NewPosition.x = ObjectPosition.x + ObjectDistance;
			}
			m_NewPosition.y = ObjectPosition.y;
			m_EnemyStatus = Status::Sprinting;
			m_IsPickingUp = true;
		}
		else if (RandomChoice >= 15)
		{
			int MoveDistance{ 500 };
			float PlayerDistance{ 180.f };
			int IsAggressive(rand() % 2);

			if (IsAggressive == 0) m_IsAggressive = true;
			else m_IsAggressive = false;

			m_IsMoving = true;
			if (m_Position.x < PlayerPosition.x) m_NewPosition.x = PlayerPosition.x - PlayerDistance;
			else m_NewPosition.x = PlayerPosition.x + PlayerDistance;
			m_NewPosition.y = PlayerPosition.y;
			if (m_HasPickUpObject) m_EnemyStatus = Status::PickUpRun;
			else m_EnemyStatus = Status::Sprinting;
		}
		else if (RandomChoice >= -10)
		{
			if(m_HasPickUpObject)
			{
				int MoveDistance{ 500 };
				float PlayerDistance{ 180.f };

				m_IsMoving = true;
				if (m_Position.x < PlayerPosition.x) m_NewPosition.x = PlayerPosition.x - PlayerDistance * 3.f;
				else m_NewPosition.x = PlayerPosition.x + PlayerDistance * 3.f;
				m_NewPosition.y = PlayerPosition.y;
				m_EnemyStatus = Status::PickUpRun;

				m_IsGoingToThrow = true;
			}
			else
			{
				m_FrameNR = 0.f;
				m_IsTaunting = true;
				m_EnemyStatus = Status::Taunt;
			}
		}



		m_ChoicesDelayCounter -= m_MaxChoiceDelay;
		m_MaxChoiceDelay = float(rand() % 5 + 4);
		//std::cout << "Moving Is True" << std::endl;
	}
}

void EnemyMike::UpdateChoicesDelay(const Point2f& PlayerPosition)
{
	UpdateChoicesDelay(PlayerPosition, Point2f{0.f, 0.f});
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
		m_FrameNR = 4.f;
		m_AnimationCounter = 0.f;
		m_GotLightHitAmount = 4;
		m_IsStunned = false;
		m_StunnedCounter -= m_MAX_STUNNED_DELAY;
		std::cout << "UpdateStunned check" << std::endl;
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

	if (m_EnemyStatus == Status::Sprinting || m_EnemyStatus == Status::PickUpRun)
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
		if(m_IsPickingUp)
		{
			m_IsAttacking = true;
			m_EnemyStatus = Status::LightAttack;
		}
		else if(m_IsGoingToThrow)
		{
			m_IsGoingToThrow = false;
			m_IsAttacking = true;
			m_EnemyStatus = Status::PickUpThrow;
		}
		else if(m_EnemyStatus == Status::PickUpRun)
		{
			m_IsAttacking = true;
			m_EnemyStatus = Status::PickUpAttack;
		}
		else if (m_EnemyStatus == Status::Sprinting)
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
		m_JustSpawned = false;
		m_IsAggressive = false;
		m_FrameNR = 0.f;
		m_GotLightHitAmount = 0;
	}
}

void EnemyMike::Block()
{
	m_EnemyStatus = Status::Block;
	m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::Block);
	m_IsBlocking = true;
	m_IsMoving = false;
	//m_IsDamaged = false;
}

void EnemyMike::CheckIfGoingOutOfBounds(const std::vector<std::vector<Point2f>>& MapSvg)
{

	float yLength{ 5.f };

	for (int VectorIndex{}; VectorIndex < (MapSvg.size()); ++VectorIndex)
	{
		if (m_IsFalling)
		{
			yLength -= m_Position.y - m_InitialJumpPosition.y;

			if (utils::Raycast(MapSvg[VectorIndex], Point2f{ m_HitboxTransformed[1].x - 1.f, m_HitboxTransformed[1].y + yLength }, Point2f{ m_HitboxTransformed[0].x + 1.f, m_HitboxTransformed[0].y + yLength }, m_Hitinfo))
			{
				if (m_IsLeft) m_Position.x = m_Hitinfo.intersectPoint.x - m_Width / 2.f;
				else m_Position.x = m_Hitinfo.intersectPoint.x + 2.f;
			}
		}
		else if (utils::Raycast(MapSvg[VectorIndex], Point2f{m_HitboxTransformed[1].x - 1.f, m_HitboxTransformed[1].y + yLength}, Point2f{m_HitboxTransformed[0].x + 1.f, m_HitboxTransformed[0].y + yLength}, m_Hitinfo))
		{
			m_IsMoving = false;
			if (m_IsLeft) m_Position.x = m_Hitinfo.intersectPoint.x - m_Width / 2.f - 3.f;
			else m_Position.x = m_Hitinfo.intersectPoint.x + 2.f;
			/*if (m_IsBlocking)
			{
				m_IsBlocking = false;
				m_IsDamaged = false;
				m_BlockingCounter = 0.f;
			}*/
		}


		if ((utils::Raycast(MapSvg[VectorIndex], Point2f{ m_HitboxTransformed[0].x, m_HitboxTransformed[0].y - 1.f }, Point2f{ m_HitboxTransformed[0].x, m_HitboxTransformed[0].y + yLength }, m_Hitinfo)
			|| utils::Raycast(MapSvg[VectorIndex], Point2f{ m_HitboxTransformed[1].x, m_HitboxTransformed[1].y - 1.f }, Point2f{ m_HitboxTransformed[1].x, m_HitboxTransformed[1].y + yLength }, m_Hitinfo))
			&& m_IsFalling == false)
		{
			m_IsMoving = false;
			if (m_Position.y >= 350.f) m_Position.y = m_Hitinfo.intersectPoint.y - 6.f;
			else m_Position.y = m_Hitinfo.intersectPoint.y;
		}

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

void EnemyMike::CheckHit(const std::vector<Point2f>& Attackbox, int GetDamage, bool JustToCheckCollision, bool GetThrownInTheAir, bool GetUppercut, bool IsAnObject)
{

	if (JustToCheckCollision && utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo) || (IsAnObject && utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[0]), Point2f(m_HitboxTransformed[3]), m_Hitinfo)))
	{
		m_IsColliding = true;
	}

	if((m_IsDamaged == false || ((GetUppercut || GetThrownInTheAir) && m_IsFalling == false)) && m_IsAlive && m_EnemyStatus != Status::OnTheGround && m_IsBlocking == false)
	{
		if (utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo) || (IsAnObject && utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[0]), Point2f(m_HitboxTransformed[3]), m_Hitinfo)))
		{
			int ChanceToBlock{ rand() % 100 };
			if (ChanceToBlock < m_BlockChancePercent && m_IsStunned == false && m_HasPickUpObject == false && JustToCheckCollision == false && GetUppercut == false)
			{
				Block();
			}
			else
			{
				ResetFrame();
				m_Health -= GetDamage;
				m_IsMoving = false;
				m_JustSpawned = false;
				m_IsAttacking = false;
				m_IsAggressive = false;
				m_HasPickUpObject = false;
				m_ptrHoldingObject = nullptr;

				if (m_Health <= 0) m_IsAlive = false;

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
					m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::HeavyAttack);
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
					m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::Uppercut);
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
					m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::HeavyAttackFollow);
				}
				else
				{
					m_EnemyStatus = Status::Hit;
					++m_GotLightHitAmount;
					if (m_GotLightHitAmount == 3)
					{
						m_DamagedWhileStunned = true;
						m_StunnedCounter = 0.f;
					}
					m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::LightAttackHIt);
					//std::cout << "LightAttack Hit" << std::endl;
				}
				m_IsDamaged = true;
				m_IsHit = true;
				ResetFrame();
			}
		}
	}
}

void EnemyMike::HasPickedUpObject(bool HasPickUp, Objects* Object)
{
	if(HasPickUp)
	{
		m_IsPickingUp = false;
		m_IsMoving = false;
		m_EnemyStatus = Status::PickUp;
		ResetFrame();
		m_HasPickUpObject = true;
		m_ptrHoldingObject = Object;
		m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::RecycleHit);
	}
}

bool EnemyMike::CheckIdle() const
{
	if (m_IsAttacking || m_IsDamaged || m_IsFalling || m_IsOnTheGround || m_IsAlive == false || m_IsGettingUp || m_IsMoving || m_IsTaunting || m_IsBlocking || m_IsStunned || m_IsPickingUp)
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

bool EnemyMike::GetIsBlocking() const
{
	return m_IsBlocking;
}

bool EnemyMike::GetIsColliding() const
{
	std::cout << "Get Is colliding check: " << m_IsColliding << std::endl;
	return m_IsColliding;
}

bool EnemyMike::GetIsLeft() const
{
	return m_IsLeft;
}

bool EnemyMike::GetFlipBox() const
{
	return m_FlipObject;
}

bool EnemyMike::GetThrowObject() const
{
	return m_ThrowObject;
}

bool EnemyMike::GetSpawnCoins() const
{
	return m_SpawnCoins;
}

bool EnemyMike::GetHasPickedUp() const
{
	return m_HasPickUpObject;
}

bool EnemyMike::GetIsPickingUp() const
{
	return m_IsPickingUp;
}

bool EnemyMike::GetIsHit() const
{
	return m_IsHit;
}

bool EnemyMike::GetIsStunned() const
{
	return m_IsStunned;
}

int EnemyMike::GetHealth() const
{
	return m_Health;
}

int EnemyMike::GetObjectRumble() const
{
	return m_ObjectRumble;
}

Point2f EnemyMike::GetPosition() const
{
	return m_Position;
}

bool EnemyMike::CheckIfAttackBoxIsOn()
{
	if (m_IsPickingUp) m_IsAttackBoxOn;
	else
	{
		if (m_AttackBoxReset && m_IsAttackBoxOn)
		{
			// Resets attackbox so it doesn't hit player multiple times
			m_AttackBoxReset = false;
			return true;
		}
		else false;
	}
}

std::vector<Point2f> EnemyMike::GetAttackBox() const
{
	return std::vector<Point2f>(m_AttackBoxTransformed);
}

Objects* EnemyMike::GetHoldingObject() const
{
	return m_ptrHoldingObject;
}

std::string EnemyMike::GetEnemyType() const
{
	return m_EnemyType;
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
