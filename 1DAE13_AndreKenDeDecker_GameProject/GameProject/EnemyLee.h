#pragma once
#include "EnemyMike.h"

class EnemyLee : public EnemyMike
{
public:
	EnemyLee(Point2f position, float width, float height, const std::string& EnemyType = "Lee");
	~EnemyLee() noexcept;

	virtual void Update(float elapsedSec, const Point2f& PlayerPosition, const std::vector<Point2f>& MapSvg, const Point2f& ObjectPosition = Point2f{ 0.f, 0.f }, bool ObjectIsLeft = false) override;
private:

	virtual void UpdateChoicesDelay(const Point2f& PlayerPosition, const Point2f& ObjectPosition, bool ObjectIsLeft = false) override;
};

