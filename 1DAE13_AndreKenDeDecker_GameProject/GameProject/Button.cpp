#include "pch.h"
#include "Button.h"

Button::Button(Point2f Position, float Width, float Height, const std::string& Text, bool Active) :m_Text{Text}, m_Position { Position }, m_Width{ Width }, m_Height{ Height }, m_IsActivated{ Active }
{
	m_TextPath = "Fonts/8-bit-madness-regular.ttf";
	m_ptrShadowTextFont = new Texture(m_Text, m_TextPath, m_Height, Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	m_ptrTextFont = new Texture(m_Text, m_TextPath, m_Height, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
}

Button::~Button() noexcept
{
	delete m_ptrShadowTextFont;
	m_ptrShadowTextFont = nullptr;
	delete m_ptrTextFont;
	m_ptrTextFont = nullptr;
}

void Button::Update(float elapsedSec)
{
	if(m_IsActivated)
	{

	}
}

void Button::Draw() const
{
	Point2f MiddleOfScreen{ Point2f(-m_ptrTextFont->GetWidth() / 2.f + 1280.f / 2.f, -m_ptrTextFont->GetHeight() / 2.f + 720.f / 2.f) };

	if (m_IsActivated)
	{
		utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
		utils::FillRect(Rectf(0.f, m_Position.y, 1281 , m_Height - 10.f));

		m_ptrTextFont->Draw(Rectf(MiddleOfScreen.x + m_Position.x, m_Position.y + 5.f, m_Width - 10.f, m_Height - 15.f));
	}
	else
	{
		m_ptrShadowTextFont->Draw(Rectf(MiddleOfScreen.x + m_Position.x, m_Position.y, m_Width + 0.5f, m_Height));

		m_ptrTextFont->Draw(Rectf(MiddleOfScreen.x + m_Position.x, m_Position.y + 5.f, m_Width - 10.f, m_Height - 15.f));
	}
}

void Button::SetIsActivated(bool Activated)
{
	m_IsActivated = Activated;
}
