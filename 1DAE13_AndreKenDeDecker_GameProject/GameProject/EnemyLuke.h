#pragma once
#include "EnemyMike.h"

class EnemyLuke final: public EnemyMike
{
public:
	explicit EnemyLuke(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType = "Luke");
	virtual ~EnemyLuke() noexcept override;

	EnemyLuke(const EnemyLuke& other) = delete;
	EnemyLuke& operator=(const EnemyLuke& other) = delete;
	EnemyLuke(EnemyLuke&& other) = delete;
	EnemyLuke& operator=(EnemyLuke&& other) = delete;

};

	