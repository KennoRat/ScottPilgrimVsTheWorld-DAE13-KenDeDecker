#include "pch.h"
#include "Objects.h"

Texture* Objects::m_ptrSpriteSheet{ nullptr };
int Objects::m_InstanceCounter{ 0 };

Objects::Objects(Point2f Position, float Width, float Height): m_Position{Position}, m_Width{Width}, m_Height{Height}
{
	m_IsInTheAir = false;
	m_IsSliding = false;
	m_IsPickedUp = false;
	m_IsLeft = false;
	m_IsFlipped = false;
	m_DoDamage = false;

	m_Velocity = Vector2f{ 0.f, 0.f };

	m_Rumble = 0;

	m_FallYPosition = 0.f;
	m_SlideCounter = 0.f;

	m_ptrPlayer = nullptr;
	m_ptrEnemy = nullptr;

	m_HitboxOrigin.push_back(Point2f{0.f, 0.f});
	m_HitboxOrigin.push_back(Point2f{m_Width, 0.f});
	m_HitboxOrigin.push_back(Point2f{m_Width, m_Height});
	m_HitboxOrigin.push_back(Point2f{ 0.f, m_Height });

	++m_InstanceCounter;

	if (m_InstanceCounter == 1)
	{
		m_ptrSpriteSheet = new Texture("RecyclingBox_Sprite.png");
	}
}

Objects::~Objects()
{

	if (m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_InstanceCounter;
}

void Objects::Draw() const
{
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Width, m_Height };

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect);
	ResetSprite();


	utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
	utils::DrawPolygon(m_HitboxTransformed);
}

void Objects::Update(float elapsedSec, const Point2f& Position, bool IsLeft, const std::vector<Point2f>& MapSvg)
{
	// Transform Hitboxes
	Matrix2x3 TranslationMat{};
	float LeftXDifference{ 17.f };

	if(m_IsLeft) TranslationMat.SetAsTranslate(Vector2f(m_Position.x + LeftXDifference, m_Position.y));
	else TranslationMat.SetAsTranslate(Vector2f(m_Position));
	

	m_HitboxTransformed = TranslationMat.Transform(m_HitboxOrigin);

	UpdateXPosition(elapsedSec);
	UpdateYPosition(elapsedSec);

	if(m_IsPickedUp) m_Position = Position;
	
	m_IsLeft = IsLeft;

	CheckIfGoingOutOfBounds(MapSvg);



}

void Objects::Update(float elapsedSec, const std::vector<Point2f>& MapSvg)
{
	Update(elapsedSec, m_Position, m_IsLeft, MapSvg);
}

void Objects::Collision(const std::vector<Point2f>& Hitbox, bool IsLeft)
{
	if (utils::Raycast(Hitbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo) && IsLeft)
	{
		m_IsSliding = true;
		m_IsLeft = true;
		m_Velocity.x = -500.f;
		m_SlideCounter = 0.f;

	}
	else if(utils::Raycast(Hitbox, Point2f(m_HitboxTransformed[0]), Point2f(m_HitboxTransformed[3]), m_Hitinfo) && IsLeft == false)
	{
		m_IsSliding = true;
		m_IsLeft = false;
		m_Velocity.x = 500.f;
		m_SlideCounter = 0.f;
	}
}

void Objects::PickUpPlayer(const std::vector<Point2f>& Hitbox, ScottPilgrim* Player)
{
	if (utils::Raycast(Hitbox, Point2f(m_HitboxTransformed[2]), Point2f(m_HitboxTransformed[3]), m_Hitinfo))
	{
		m_IsPickedUp = true;
		m_ptrPlayer = Player;
	}
}

void Objects::PickUpEnemy(const std::vector<Point2f>& Hitbox, EnemyMike* Enemy)
{
	if (utils::Raycast(Hitbox, Point2f(m_HitboxTransformed[2]), Point2f(m_HitboxTransformed[3]), m_Hitinfo))
	{
		m_ptrEnemy = Enemy;
		m_IsPickedUp = true;
	}
}

void Objects::DroppedObject(float YPosition)
{
	std::cout << "Object Dropped" << std::endl;
	m_IsPickedUp = false;
	m_IsFlipped = false;
	m_FallYPosition = YPosition;
	m_IsInTheAir = true;
}

void Objects::ThrownObject(float YPosition)
{
	float ThrowSpeed{ 1400.f };
	m_IsPickedUp = false;

	if (m_IsLeft) m_Velocity.x = -ThrowSpeed;
	else m_Velocity.x = ThrowSpeed;
	
	m_Position.y += 20.f;
	m_FallYPosition = YPosition;
	m_IsInTheAir = true;
	m_DoDamage = true;
}

void Objects::ObjectHit(bool IsHit)
{
	if(m_IsInTheAir && IsHit)
	{
		m_DoDamage = false;
		m_Velocity.x = -m_Velocity.x / 3.f;
		m_IsLeft = !m_IsLeft;
	}
}

bool Objects::CheckIfOverlapping(const std::vector<Point2f>& Hitbox)
{
	if (utils::Raycast(Hitbox, Point2f(m_HitboxTransformed[2]), Point2f(m_HitboxTransformed[3]), m_Hitinfo)) return true;
	else return false;
}

bool Objects::GetIsPickedUp() const
{
	return m_IsPickedUp;
}

bool Objects::GetIsFlipped() const
{
	return m_IsFlipped;
}

bool Objects::GetDoDamage() const
{
	return m_DoDamage;
}

bool Objects::GetIsLeft() const
{
	return m_IsLeft;
}

int Objects::GetRumble() const
{
	return m_Rumble;
}

float Objects::GetFallYPosition() const
{
	return m_FallYPosition;
}

Point2f Objects::GetPosition() const
{
	return m_Position;
}

std::vector<Point2f> Objects::GetHitbox() const
{
	return m_HitboxTransformed;
}

ScottPilgrim* Objects::GetPlayer() const
{
	return m_ptrPlayer;
}

EnemyMike* Objects::GetEnemy() const
{
	return m_ptrEnemy;
}

void Objects::SetIsFlipped(bool IsFlipped)
{
	//std::cout << "Object Flipped" << std::endl;
	m_IsFlipped = IsFlipped;
}

void Objects::SetRumble(int Rumble)
{
	m_Rumble = Rumble;
}

void Objects::TranslateSprite() const
{
	glPushMatrix();
	
	float FlippedX{ 200.f };
	float FlippedY{ 50.f };

	if (m_IsLeft)
	{
		float LeftXDifference{ 17.f };
		if (m_IsFlipped)
		{
			glTranslatef(m_Position.x + m_Width - FlippedX + LeftXDifference, m_Position.y - FlippedY, 0.f);

			glScalef(-1.f, 1.f, 0.f);

			glRotatef(-90, 0, 0, 1);

			glTranslatef(-m_Width / 2.f, -m_Height / 2.f, 0.f);
		}
		else
		{
			glTranslatef(m_Position.x + m_Width + LeftXDifference, m_Position.y - m_Rumble, 0.f);
			glScalef(-1.f, 1.f, 0.f);
		}
	}
	else
	{
		if (m_IsFlipped)
		{
			glTranslatef(m_Position.x + FlippedX, m_Position.y - FlippedY, 0.f);

			glRotatef(-90, 0, 0, 1);

			glTranslatef(-m_Width / 2.f, -m_Height / 2.f, 0.f);
		}
		else glTranslatef(m_Position.x, m_Position.y - m_Rumble, 0.f);
	}

}

void Objects::ResetSprite() const
{
	glPopMatrix();
}

void Objects::UpdateXPosition(float elapsedSec)
{
	if (m_IsSliding)
	{
		m_SlideCounter += elapsedSec;
		if (m_SlideCounter >= m_MAX_SLIDE_DELAY)
		{
			m_IsSliding = false;
			m_SlideCounter -= m_MAX_SLIDE_DELAY;
			m_Velocity.x = 0.f;
		}
		else
		{
			if (m_IsLeft && m_Velocity.x < 0.f) m_Velocity.x += 250.f * elapsedSec;
			else if (m_Velocity.x > 0.f) m_Velocity.x -= 250.f * elapsedSec;
			
			m_Position.x += m_Velocity.x * elapsedSec;
		}
	}
	else if(m_IsInTheAir)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
	}
}

void Objects::UpdateYPosition(float elapsedSec)
{
	if(m_IsInTheAir)
	{
		if(m_FallYPosition <= m_Position.y)
		{
			m_Velocity.y -= 2000.f * elapsedSec;

			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else
		{
			m_Velocity.y = 0.f;
			m_IsPickedUp = false;
			m_IsInTheAir = false;
			m_DoDamage = false;
			m_ptrPlayer = nullptr;
			m_ptrEnemy = nullptr;
			if(m_Velocity.x > 200.f || m_Velocity.x < -200.f)
			{
				if(m_IsLeft) m_Velocity.x = -300.f;
				else m_Velocity.x = 300.f;
				m_IsSliding = true;
				m_SlideCounter = 0.f;
			}
		}
	}
}

void Objects::CheckIfGoingOutOfBounds(const std::vector<Point2f>& MapSvg)
{
	float yLength{ 5.f };

	if (utils::Raycast(MapSvg, Point2f{ m_HitboxTransformed[1].x - 1.f, m_HitboxTransformed[1].y + yLength }, Point2f{ m_HitboxTransformed[0].x + 1.f, m_HitboxTransformed[0].y + yLength }, m_Hitinfo))
	{
		if (m_IsLeft) m_Position.x = m_Hitinfo.intersectPoint.x + 2.f;
		else m_Position.x = m_Hitinfo.intersectPoint.x - m_Width / 2.f;
		m_Velocity.x = -m_Velocity.x / 3.f;
		m_IsLeft = !m_IsLeft;
	}
	else if (m_IsInTheAir)
	{
		yLength -= m_Position.y - m_FallYPosition;

		if (utils::Raycast(MapSvg, Point2f{ m_HitboxTransformed[1].x - 1.f, m_HitboxTransformed[1].y + yLength }, Point2f{ m_HitboxTransformed[0].x + 1.f, m_HitboxTransformed[0].y + yLength }, m_Hitinfo))
		{
			if (m_IsLeft) m_Position.x = m_Hitinfo.intersectPoint.x + 2.f;
			else m_Position.x = m_Hitinfo.intersectPoint.x - m_Width / 2.f;
			m_Velocity.x = -m_Velocity.x / 3.f;
			m_IsLeft = !m_IsLeft;
		}
	}

	if ((utils::Raycast(MapSvg, Point2f{ m_HitboxTransformed[0].x, m_HitboxTransformed[0].y - 1.f }, Point2f{ m_HitboxTransformed[0].x, m_HitboxTransformed[0].y + yLength }, m_Hitinfo)
		|| utils::Raycast(MapSvg, Point2f{ m_HitboxTransformed[1].x, m_HitboxTransformed[1].y - 1.f }, Point2f{ m_HitboxTransformed[1].x, m_HitboxTransformed[1].y + yLength }, m_Hitinfo))
		&& m_IsInTheAir == false)
	{
		if (m_Position.y >= 350.f) m_Position.y = m_Hitinfo.intersectPoint.y - 6.f;
		else m_Position.y = m_Hitinfo.intersectPoint.y;
	}

}
