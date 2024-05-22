#pragma once
#include "SoundEffect.h"
#include <vector>
#include <iostream>
class SoundEffects
{
public:
	SoundEffects();
	~SoundEffects();
	
	enum class SoundEffectType
	{
		Block = 0, CoinPickUp = 1, CoinSpawn = 2, HeavyAttack = 3, HeavyAttackFollow = 4, HitAir = 5, HitTheGround = 6, JumpLand = 7, LightAttackHIt = 8,
		RecycleDrop = 9, RecycleHit = 10, Uppercut = 11
	};

	void Play(SoundEffectType Effect);
	void Stop(SoundEffectType Effect);
	void Stop();

private:
	std::vector<SoundEffect*> m_ptrSoundEffects;
};

