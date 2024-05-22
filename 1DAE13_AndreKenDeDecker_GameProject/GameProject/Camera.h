#pragma once
#include "Vector2f.h"
#include "utils.h"
#include <math.h>

class Camera
{
public:
	Camera(float screenWidth, float screenHeight, const Point2f& Position);

	void Update(float elapsedSec);
	void Aim(float levelWidth, float levelHeight, const Point2f& trackCenter);
	void Reset();

private:
	float m_ScreenWidth;
	float m_ScreenHeight;

	Point2f m_GoToPosition;
	Point2f m_CameraPosition;
	Vector2f m_Velocity;
};

