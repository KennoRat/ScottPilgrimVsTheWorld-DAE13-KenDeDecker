#pragma once
#include "EnemyMike.h"

class EnemyLuke: public EnemyMike
{
public:
	EnemyLuke(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType = "Luke");
	~EnemyLuke() noexcept;

private:
};

