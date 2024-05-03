#include "pch.h"
#include "Coins.h"

Texture* Coins::m_ptrSpriteSheet{ nullptr };
int Coins::m_InstanceCounter{0};

Coins::Coins(Point2f position, float size, Type coin): m_Position{position}, m_Size{size}, m_CoinType{coin}
{
	++m_InstanceCounter;

	if(m_InstanceCounter == 1)
	{
		m_ptrSpriteSheet = new Texture("Coins_Sprite.png");
	}

	m_IsFlying = true;
	m_IsFalling = false;
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_Bounce = 0;

	m_NewYPosition = m_Position.y - float(rand() % 200 - 100);
	m_Velocity = Vector2f{ float(rand() % 100 + 50), float(rand() % 100 + 150)};

	float HitboxSize{ m_Size / 2.f };
	m_HitboxOrigin.push_back(Point2f{0.f, 0.f});
	m_HitboxOrigin.push_back(Point2f{ HitboxSize, HitboxSize });
	m_HitboxOrigin.push_back(Point2f{ 0.f, HitboxSize });
	m_HitboxOrigin.push_back(Point2f{ HitboxSize, 0.f });
}

Coins::~Coins()
{
	if(m_InstanceCounter == 1)
	{
		delete m_ptrSpriteSheet;
		m_ptrSpriteSheet = nullptr;
	}

	--m_InstanceCounter;
}

Coins::Coins(const Coins& other): Coins(other.m_Position, other.m_Size, other.m_CoinType)
{

}

Coins& Coins::operator=(const Coins& other)
{
	if (this != &other)
	{
		m_Position = other.m_Position;
		m_Size = other.m_Size;
		m_CoinType = other.m_CoinType;
	}
	return *this;
}

Coins::Coins(Coins&& other) noexcept
	:m_Position{std::move(other.m_Position)}
	,m_Size{std::move(other.m_Size)}
	,m_CoinType{std::move(other.m_CoinType)}
{
	other.m_ptrSpriteSheet = nullptr;
}

Coins& Coins::operator=(Coins&& other) noexcept
{
	if (this != &other)
	{
		m_Position = std::move(other.m_Position);
		m_Size = std::move(other.m_Size);
		m_CoinType = std::move(other.m_CoinType);
		other.m_ptrSpriteSheet = nullptr;
	}
	return *this;
}

void Coins::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 10.f };
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 6.f;
	float RowIndex = float(m_CoinType);

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth, RowHeigth * RowIndex, CollumnWidth, RowHeigth};
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Size, m_Size };

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect, srcRect);
	ResetSprite();

	//utils::SetColor(Color4f(0, 1.0f, 0, 1.0f));
	//utils::DrawPolygon(m_HitboxTransformed);
}

void Coins::Update(float elapsedSec)
{
	m_AnimationCounter += elapsedSec;

	if(m_AnimationCounter >= m_MAX_ANIMATION)
	{
		if (m_FrameNR < m_MAX_FRAME)
		{
			++m_FrameNR;
		}
		else
		{
			m_FrameNR = 0.f;
		}

		m_AnimationCounter -= m_MAX_ANIMATION;
	}

	if (m_IsFalling || m_IsFlying)UpdatePositionAfterSpawn(elapsedSec);

	// Transform Hitbox
	Matrix2x3 TranslationMat{};
	TranslationMat.SetAsTranslate(Vector2f(m_Position));

	m_HitboxTransformed = TranslationMat.Transform(m_HitboxOrigin);
}

int Coins::GetValue() const
{
	switch (m_CoinType)
	{
	case Coins::Type::Cents5:
		return 5;
		break;
	case Coins::Type::Cents10:
		return 10;
		break;
	case Coins::Type::Cents25:
		return 25;
		break;
	case Coins::Type::Dollar1:
		return 1;
		break;
	case Coins::Type::Dollars2:
		return 2;
		break;
	}

}

std::vector<Point2f> Coins::GetHitbox() const
{
	return m_HitboxTransformed;
}

void Coins::TranslateSprite() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
}

void Coins::ResetSprite() const
{
	glPopMatrix();
}

void Coins::UpdatePositionAfterSpawn(float elapsedSec)
{
	m_Velocity.y -= 500.f * elapsedSec;
	if(m_IsFlying)
	{
		if (m_Velocity.y >= 0.f)
		{
			m_Position.x += m_Velocity.x * elapsedSec;
			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else
		{
			m_IsFlying = false;
			m_IsFalling = true;
		}
	}
	else if(m_IsFalling)
	{
		if (m_Position.y >= m_NewYPosition)
		{
			m_Position.x += m_Velocity.x * elapsedSec;
			m_Position.y += m_Velocity.y * elapsedSec;
		}
		else
		{
			if(m_Bounce == 0)
			{
				m_Velocity.y = -m_Velocity.y;
				m_IsFalling = false;
				m_IsFlying = true;
				++m_Bounce;
			}
			else
			{
				m_IsFalling = false;
			}
		}
	}

}
