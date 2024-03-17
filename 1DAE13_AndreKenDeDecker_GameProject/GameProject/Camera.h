#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight);

	void Aim(float levelWidth, float levelHeight, const Point2f& trackCenter);
	void Reset();

private:
	float m_ScreenWidth;
	float m_ScreenHeight;
};

