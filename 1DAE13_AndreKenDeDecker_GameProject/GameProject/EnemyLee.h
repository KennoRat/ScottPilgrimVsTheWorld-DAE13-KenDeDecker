#pragma once
#include "EnemyMike.h"

class EnemyLee final: public EnemyMike
{
public:
	explicit EnemyLee(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType = "Lee");
	virtual ~EnemyLee() noexcept override;

	EnemyLee(const EnemyLee& other) = delete;
	EnemyLee& operator=(const EnemyLee& other) = delete;
	EnemyLee(EnemyLee&& other) = delete;
	EnemyLee& operator=(EnemyLee&& other) = delete;

	virtual void Update(float elapsedSec, const Point2f& PlayerPosition, const std::vector<std::vector<Point2f>>& MapSvg, const Point2f& ObjectPosition = Point2f{ 0.f, 0.f }, bool ObjectIsLeft = false) override;
private:

	virtual void UpdateChoicesDelay(const Point2f& PlayerPosition, const Point2f& ObjectPosition, bool ObjectIsLeft = false) override;
};

