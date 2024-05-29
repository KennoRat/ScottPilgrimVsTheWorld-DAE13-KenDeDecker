#pragma once
#include "EnemyMike.h"

class EnemyLuke final: public EnemyMike
{
public:
	explicit EnemyLuke(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType = "Luke");
	virtual ~EnemyLuke() noexcept override;

private:
};

	