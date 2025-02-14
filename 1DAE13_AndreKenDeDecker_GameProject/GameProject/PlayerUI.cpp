#include "pch.h"
#include "PlayerUI.h"

PlayerUI::PlayerUI(float size): m_Size{size}
{
	m_Position = Point2f(0, 0);
	m_AboveDecimalPoint = 0;
	m_BelowDecimalPoint = 0;
	m_PlayerHealth = 0;
	m_FrameNR = 0;
	m_AnimationCounter = 0.f;
	m_ptrSpriteSheet = new Texture("MoneyNumbers_Sprite.png");
	m_ptrSpriteUI = new Texture("ScottHealthMoney_Sprite.png");
}

PlayerUI::~PlayerUI()
{
	delete m_ptrSpriteSheet;
	m_ptrSpriteSheet = nullptr;
	delete m_ptrSpriteUI;
	m_ptrSpriteUI = nullptr;
}

void PlayerUI::Draw() const
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

void PlayerUI::DrawUI() const
{
	float CollumnWidth{ m_ptrSpriteSheet->GetWidth() / 15.f };
	float RowHeigth{ m_ptrSpriteSheet->GetHeight() / 3.f };
	float DollarSignIndex{ 0.f };
	float NumbersIndex{ 1.f };

	Rectf LowestBelowDecimalDigitSrcRect = Rectf{ float(m_BelowDecimalPoint % 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };
	Rectf HighestBelowDecimalDigitSrcRect = Rectf{ float(m_BelowDecimalPoint / 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf DecimalDigitSrcRect = Rectf{ 10.f * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf LowestAboveDecimalDigitSrcRect = Rectf{ float(m_AboveDecimalPoint % 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };
	Rectf HighestAboveDecimalDigitSrcRect = Rectf{ float(m_AboveDecimalPoint / 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf HealthLowestDecimalDigitSrcRect = Rectf{ float(m_PlayerHealth % 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };
	Rectf HealthHighestDecimalDigitSrcRect = Rectf{ float(m_PlayerHealth / 10) * CollumnWidth - 1.f, RowHeigth * NumbersIndex, CollumnWidth, RowHeigth };

	Rectf srcRect = Rectf{ 0.f, 0.f, m_ptrSpriteUI->GetWidth(), m_ptrSpriteUI->GetHeight()};
	Rectf dstRect = Rectf{ 0.f, 0.f, m_ptrSpriteUI->GetWidth() * 1.5f, m_ptrSpriteUI->GetHeight() * 1.5f };

	glPushMatrix();
	glTranslatef( 60.f , 550.f, 0.f);

	m_ptrSpriteUI->Draw(dstRect, srcRect);

	float NumbersXDistance{ 190.f };
	float NumbersYDistance{ 12.f };
	float NumbersDistance{ m_Size / 4.f };
	float NumbersSize{ m_Size / 2.f };

	dstRect = Rectf{ NumbersDistance * 6.6f + NumbersXDistance, NumbersYDistance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, LowestBelowDecimalDigitSrcRect);
	dstRect = Rectf{ NumbersDistance * 5.3f + NumbersXDistance, NumbersYDistance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, HighestBelowDecimalDigitSrcRect);

	dstRect = Rectf{ NumbersDistance * 4.65f + NumbersXDistance, NumbersYDistance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, DecimalDigitSrcRect);

	dstRect = Rectf{ NumbersDistance * 3.3f + NumbersXDistance, NumbersYDistance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, LowestAboveDecimalDigitSrcRect);
	dstRect = Rectf{ NumbersDistance * 2.f + NumbersXDistance, NumbersYDistance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, HighestAboveDecimalDigitSrcRect);

	float HealthYDisitance{35.f};

	dstRect = Rectf{ NumbersDistance * 4.4f + NumbersXDistance, NumbersYDistance + HealthYDisitance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, HealthHighestDecimalDigitSrcRect);
	dstRect = Rectf{ NumbersDistance * 5.7f + NumbersXDistance, NumbersYDistance + HealthYDisitance, NumbersSize, NumbersSize };
	m_ptrSpriteSheet->Draw(dstRect, HealthLowestDecimalDigitSrcRect);

	glPopMatrix();
}

void PlayerUI::Update(float elapsedSec, Point2f Position, int AboveDecimal, int BelowDecimal)
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

void PlayerUI::SetPlayerHealth(int Health)
{
	if (Health >= 0) m_PlayerHealth = Health;
	else m_PlayerHealth = 0;
}

void PlayerUI::TranslateSprite() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
}

void PlayerUI::ResetSprite() const
{
	glPopMatrix();
}
