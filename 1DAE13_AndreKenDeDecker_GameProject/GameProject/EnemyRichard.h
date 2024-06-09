#pragma once
#include "EnemyMike.h"

class EnemyRichard final : public EnemyMike
{
public:
	explicit EnemyRichard(Point2f position, float width, float height, SoundEffects* SoundEffects, const std::string& EnemyType = "Richard");
	virtual ~EnemyRichard() noexcept override;

	EnemyRichard(const EnemyRichard& other) = delete;
	EnemyRichard& operator=(const EnemyRichard& other) = delete;
	EnemyRichard(EnemyRichard&& other) = delete;
	EnemyRichard& operator=(EnemyRichard&& other) = delete;

private:
};

