#include "pch.h"
#include "SoundBarSetting.h"

SoundBarSetting::SoundBarSetting(Point2f Position, float Width, float Height, int& Music): m_Position{Position}, m_Width{Width}, m_Height{Height}, m_MusicVolume{Music}
{
	m_IsActivated = false;
}

void SoundBarSetting::Update(int Music)
{
	m_MusicVolume = Music;
}

void SoundBarSetting::Draw() const
{
	int BarsAmount{ m_MusicVolume / 10 };
	float BarWidth{ m_Width / 11.f };

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::FillRect(m_Position.x, m_Position.y, m_Width, m_Height);

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(m_Position.x + m_Width / 20.f, m_Position.y + m_Height / 5.f, m_Width - m_Width / 10.f, m_Height - m_Height / 2.5f);

	//Green bar
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillRect(m_Position.x + m_Width / 20.f, m_Position.y + m_Height / 5.f, BarWidth * BarsAmount, m_Height - m_Height / 2.5f);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	if(m_IsActivated)
	{
		utils::FillEllipse(m_Position.x - 30.f, m_Position.y + m_Height/2.f, m_Height / 4.f, m_Height / 4.f);
		utils::FillEllipse(m_Position.x + 30.f + m_Width, m_Position.y + m_Height / 2.f, m_Height / 4.f, m_Height / 4.f);
	}
}

void SoundBarSetting::SetIsActivated(bool Activated)
{
	m_IsActivated = Activated;
}
