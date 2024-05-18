#include "pch.h"
#include "EnemyLee.h"

EnemyLee::EnemyLee(Point2f position, float width, float height, const std::string& EnemyType): EnemyMike(position, width, height, EnemyType)
{
	m_Health = 35;
}

EnemyLee::~EnemyLee() noexcept
{
	if (EnemyMike::m_LeeInstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_LeeInstanceCounter;
}

void EnemyLee::Update(float elapsedSec, const Point2f& PlayerPosition, const std::vector<Point2f>& MapSvg, const Point2f& ObjectPosition, bool ObjectIsLeft)
{
	EnemyMike::Update(elapsedSec, PlayerPosition, MapSvg, ObjectPosition, ObjectIsLeft);
}

void EnemyLee::UpdateChoicesDelay(const Point2f& PlayerPosition, const Point2f& ObjectPosition, bool ObjectIsLeft)
{
	if (m_ChoicesDelayCounter >= m_MaxChoiceDelay)
	{
		int RandomChoice{ rand() % 100 };

		if (m_HasPickUpObject) RandomChoice -= 10;

		m_GotLightHitAmount = 0;

		if (RandomChoice >= 80)
		{
			int MoveXDistance{ 300 };
			int MoveYDistance{ 50 };

			m_IsMoving = true;
			if (m_Position.x < PlayerPosition.x) m_NewPosition.x = PlayerPosition.x - float(rand() % MoveXDistance + 350.f);
			else m_NewPosition.x = PlayerPosition.x + float(rand() % MoveXDistance + 350.f);
			m_NewPosition.y = PlayerPosition.y + float(rand() % MoveYDistance - (MoveYDistance / 2.f));
			if (m_HasPickUpObject) m_EnemyStatus = Status::PickUpWalk;
			else m_EnemyStatus = Status::Walking;
		}
		else if (RandomChoice >= 50 && (ObjectPosition.x != 0.f && ObjectPosition.y != 0.f) && m_HasPickUpObject == false)
		{
			float ObjectDistance{ 90.f };

			m_IsMoving = true;
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
		else if (RandomChoice >= 25)
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
			if (m_HasPickUpObject)
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
		m_MaxChoiceDelay = float(rand() % 5 + 3);
		//std::cout << "Moving Is True" << std::endl;
	}
}
