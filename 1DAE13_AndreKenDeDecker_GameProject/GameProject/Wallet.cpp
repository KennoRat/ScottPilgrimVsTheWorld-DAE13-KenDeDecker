#include "pch.h"
#include "Wallet.h"

Wallet::Wallet(float size): m_Size{size}
{
	m_Position = Point2f(0, 0);
	m_AboveDecimalPoint = 0;
	m_BelowDecimalPoint = 0;
	m_FrameNR = 0;
	m_AnimationCounter = 0.f;
	m_ptrSpriteSheet = new Texture("MoneyNumbers_Sprite.png");
}

Wallet::~Wallet()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
}

Wallet::Wallet(const Wallet& other): Wallet(other.m_Size)
{

}

Wallet& Wallet::operator=(const Wallet& other)
{
	if (this != &other)
	{
		m_Size = other.m_Size;
	}
	return *this;
}

Wallet::Wallet(Wallet&& other) noexcept : m_Size{std::move(other.m_Size)}
{
	other.m_ptrSpriteSheet = nullptr;
}

Wallet& Wallet::operator=(Wallet&& other) noexcept
{
	if (this != &other)
	{
		m_Size = std::move(other.m_Size);
		other.m_ptrSpriteSheet = nullptr;
	}
	return *this;
}

void Wallet::Draw() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 15.f };
	float RowHeigth{ m_ptrSpriteSheet->GetHeight() / 3.f };
	float DollarSignIndex{ 0.f };

	Rectf srcRect = Rectf{ m_FrameNR * CollumnWidth - 1.f, RowHeigth * DollarSignIndex, CollumnWidth, RowHeigth };
	Rectf dstRect = Rectf{ 0.f, 0.f, m_Size, m_Size };

	float NumbersIndex{ 1.f };

	Rectf LowestBelowDecimalDigitSrcRect = Rectf{ float(m_BelowDecimalPoint % 10 ) * CollumnWidth - 1.f, RowHeigth* NumbersIndex, CollumnWidth, RowHeigth};
	Rectf HighestBelowDecimalDigitSrcRect = Rectf{ float(m_BelowDecimalPoint / 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf DecimalDigitSrcRect = Rectf{ 10.f * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf LowestAboveDecimalDigitSrcRect = Rectf{ float(m_AboveDecimalPoint % 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };
	Rectf HighestAboveDecimalDigitSrcRect = Rectf{ float(m_AboveDecimalPoint / 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	TranslateSprite();
	m_ptrSpriteSheet->Draw(dstRect, srcRect);

	float NumbersDistance{ m_Size / 2.f };

	dstRect = Rectf{ NumbersDistance * 6.6f, 0.f, m_Size, m_Size };
	m_ptrSpriteSheet->Draw(dstRect, LowestBelowDecimalDigitSrcRect);
	dstRect = Rectf{ NumbersDistance * 5.3f, 0.f, m_Size, m_Size };
	m_ptrSpriteSheet->Draw(dstRect, HighestBelowDecimalDigitSrcRect);

	dstRect = Rectf{ NumbersDistance * 4.65f, 0.f, m_Size, m_Size };
	m_ptrSpriteSheet->Draw(dstRect, DecimalDigitSrcRect);

	dstRect = Rectf{ NumbersDistance * 3.3f, 0.f, m_Size, m_Size };
	m_ptrSpriteSheet->Draw(dstRect, LowestAboveDecimalDigitSrcRect);
	dstRect = Rectf{ NumbersDistance * 2.f, 0.f, m_Size, m_Size };
	m_ptrSpriteSheet->Draw(dstRect, HighestAboveDecimalDigitSrcRect);
	ResetSprite();
}

void Wallet::Update(float elapsedSec, Point2f Position, int AboveDecimal, int BelowDecimal)
{
	m_AnimationCounter += elapsedSec;
	m_Position = Position;
	m_AboveDecimalPoint = AboveDecimal;
	m_BelowDecimalPoint = BelowDecimal;

	if (m_AnimationCounter >= m_MAX_ANIMATION)
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
}

void Wallet::TranslateSprite() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
}

void Wallet::ResetSprite() const
{
	glPopMatrix();
}
