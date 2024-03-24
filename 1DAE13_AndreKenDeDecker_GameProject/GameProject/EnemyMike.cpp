#include "pch.h"
#include "EnemyMike.h"

EnemyMike::EnemyMike(Point2f position, float width, float height): m_Position{position}, m_Width{width}, m_Height{height}
{
	m_IsAlive = false;
	m_IsAttacking = false;
	m_IsDamaged = false;
	m_IsLeft = true;
	m_Velocity = Vector2f{ 200.f, 150.f };
	m_FrameNR = 0.f;
	m_AnimationCounter = 0.f;
	m_MaxAnimation = 0.09f;
	m_MaxFrame = 7.f;
	m_ptrSpriteSheet = new Texture("EnemyMike_Sprite.png");
	m_ChangedState = Status::Idle;
	m_EnemyStatus = Status::Idle;


	// Make Enemy Hitbox
	float HitboxWidthLeft{ m_Width / 3.f};
	float HitboxHeight{ m_Height - m_Height / 6.f };
	m_HitboxOnOrigin.push_back(Point2f(m_Width, 0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidthLeft, 0.f));
	m_HitboxOnOrigin.push_back(Point2f(HitboxWidthLeft, HitboxHeight));
	m_HitboxOnOrigin.push_back(Point2f(m_Width, HitboxHeight));
}

EnemyMike::~EnemyMike()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

void EnemyMike::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 7.f};
	float RowHeigth = m_ptrSpriteSheet->GetHeight() / 5.f;
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
	switch (m_EnemyStatus)
	{
	case EnemyMike::Status::Idle:
		m_MaxFrame = 3.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();
		break;
	case EnemyMike::Status::Hit:
		m_MaxFrame = 2.f;
		m_MaxAnimation = 0.09f;
		UpdateAnimation();
		break;
	}

	// Transform Hitboxes
	Matrix2x3 TranslationMat{};
	TranslationMat.SetAsTranslate(Vector2f(m_Position));

	m_HitboxTransformed = TranslationMat.Transform(m_HitboxOnOrigin);
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
	
	if(m_IsDamaged && (m_FrameNR == 0 || m_FrameNR == 1 || m_FrameNR == 2))
	{
		glTranslatef(-m_Width / 7.f, 1.f, 0.f);
	}
}

void EnemyMike::ResetSprite() const
{
	glPopMatrix();
}

void EnemyMike::CheckHit(const std::vector<Point2f>& Attackbox)
{
	if(m_IsDamaged == false)
	{
		if (utils::Raycast(Attackbox, Point2f(m_HitboxTransformed[1]), Point2f(m_HitboxTransformed[2]), m_Hitinfo))
		{
			ResetFrame();
			m_EnemyStatus = Status::Hit;
			ResetFrame();
			m_IsDamaged = true;
		}
	}
}

bool EnemyMike::CheckIdle() const
{
	if (m_IsAttacking || m_IsDamaged)
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

Point2f EnemyMike::GetPosition() const
{
	return m_Position;
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
