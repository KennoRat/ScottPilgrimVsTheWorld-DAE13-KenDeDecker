#pragma once
#include "Texture.h"
#include "utils.h"
class Button final
{
public:

	explicit Button(Point2f Position, float Width, float Height, const std::string& Text, bool Active = false);
	~Button() noexcept;

	Button(const Button& other) = delete;
	Button& operator=(const Button& other) = delete;
	Button(Button&& other) = delete;
	Button& operator=(Button&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void SetIsActivated( bool Activated);

private:

	bool m_IsActivated;

	float m_Width;
	float m_Height;

	Point2f m_Position;
	Texture* m_ptrTextFont;
	Texture* m_ptrShadowTextFont;

	std::string m_TextPath;
	std::string m_Text;
};

