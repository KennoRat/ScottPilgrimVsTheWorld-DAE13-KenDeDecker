#pragma once
#include "utils.h"
class SoundBarSetting final
{
public:

	explicit SoundBarSetting(Point2f Position, float Width, float Height, int& Music);

	void Update(int Music);
	void Draw() const;

	void SetIsActivated(bool Activated);

private:
	bool m_IsActivated;

	int m_MusicVolume;
	float m_Width;
	float m_Height;

	Point2f m_Position;
};

