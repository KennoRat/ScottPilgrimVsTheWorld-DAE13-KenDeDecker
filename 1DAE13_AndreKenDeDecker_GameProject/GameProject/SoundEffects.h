#pragma once
#include "SoundEffect.h"
#include <vector>
#include <iostream>
class SoundEffects
{
public:
	explicit SoundEffects();
	~SoundEffects();

	SoundEffects(const SoundEffects& other) = delete;
	SoundEffects& operator=(const SoundEffects& other) = delete;
	SoundEffects(SoundEffects&& other) = delete;
	SoundEffects& operator=(SoundEffects&& other) = delete;
	
	enum class SoundEffectType
	{
		Block = 0, CoinPickUp = 1, CoinSpawn = 2, HeavyAttack = 3, HeavyAttackFollow = 4, HitAir = 5, HitTheGround = 6, JumpLand = 7, LightAttackHIt = 8,
		RecycleDrop = 9, RecycleHit = 10, Uppercut = 11, HeavyAttackAir = 12, GoSound = 13
	};

	void Play(SoundEffectType Effect);
	void Stop(SoundEffectType Effect);
	void Stop();

private:
	std::vector<SoundEffect*> m_ptrSoundEffects;
};

