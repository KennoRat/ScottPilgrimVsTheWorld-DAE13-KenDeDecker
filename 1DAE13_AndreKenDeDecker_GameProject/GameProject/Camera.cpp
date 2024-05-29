#include "pch.h"
#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight, const Point2f& Position) : m_ScreenWidth{ screenWidth }, m_ScreenHeight{screenHeight}, m_CameraPosition{Position}
{
	m_IsMoving = false;
}

void Camera::Update(float elapsedSec)
{
	Vector2f CameraDirection{ (m_GoToPosition.x - m_CameraPosition.x), (m_GoToPosition.y - m_CameraPosition.y) };
	m_Velocity = Vector2f{ CameraDirection.x / sqrtf(CameraDirection.x * CameraDirection.x + CameraDirection.y * CameraDirection.y) , CameraDirection.y / sqrtf(CameraDirection.x * CameraDirection.x + CameraDirection.y * CameraDirection.y) };

	float Speed{ 1000.f };
	float Range{ 15.f };

	if (m_CameraPosition.x <= m_GoToPosition.x + Range && m_CameraPosition.x >= m_GoToPosition.x - Range) m_CameraPosition.x = m_GoToPosition.x;
	else m_CameraPosition.x += m_Velocity.x * Speed * elapsedSec;

	if (m_CameraPosition.y <= m_GoToPosition.y + Range && m_CameraPosition.y >= m_GoToPosition.y - Range) m_CameraPosition.y = m_GoToPosition.y;
	else 	m_CameraPosition.y += m_Velocity.y * Speed * elapsedSec;
}

void Camera::Aim(float levelWidth, float levelHeight, const Point2f& trackCenter)
{
	if (m_GoToPosition.x != trackCenter.x || m_GoToPosition.y != trackCenter.y)
	{
		m_GoToPosition = trackCenter;
		m_IsMoving = true;
	}
	else m_IsMoving = false;

	Point2f bottomLeftPosition{ m_CameraPosition.x - m_ScreenWidth/2.f, m_CameraPosition.y - m_ScreenHeight/3.3f};

	if(bottomLeftPosition.x + m_ScreenWidth >= levelWidth)
	{
		bottomLeftPosition.x = levelWidth - m_ScreenWidth;
	}
	else if(bottomLeftPosition.x <= 0.f)
	{
		bottomLeftPosition.x = 0.f;
	}

	if(bottomLeftPosition.y + m_ScreenHeight >= levelHeight)
	{
		bottomLeftPosition.y = levelHeight - m_ScreenHeight;
	}
	else if (bottomLeftPosition.y <= 0.f)
	{
		bottomLeftPosition.y = 0.f;
	}


	glPushMatrix();
	glTranslatef(-bottomLeftPosition.x, -bottomLeftPosition.y, 0);
}

void Camera::Reset()
{
	glPopMatrix();
}

bool Camera::GetIsMoving() const
{
	return m_IsMoving;
}
