#include "pch.h"
#include "SoundEffects.h"

SoundEffects::SoundEffects()
{
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/Block.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/CoinPickUp.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/CoinSpawn.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/HeavyAttack.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/HeavyAttackFollow.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/HitAir.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/HitTheGround.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/JumpLand.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/LightAttackHit.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/RecycleBoxDrop.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/RecycleBoxHit.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/Uppercut.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/HeavyAttackAir.wav"));
	m_ptrSoundEffects.push_back(new SoundEffect("Sounds/GoSound.wav"));

	for (SoundEffect* Effects : m_ptrSoundEffects)
	{
		if(Effects->IsLoaded() == false)
		{
			std::cout << "Error loading sound Effect" << std::endl;
		}
	}
}

SoundEffects::~SoundEffects()
{
	for(SoundEffect* Effects: m_ptrSoundEffects)
	{
		delete Effects;
		Effects = nullptr;
	}
}

void SoundEffects::Play(SoundEffectType Effect)
{
	m_ptrSoundEffects[int(Effect)]->Play(0);
}

void SoundEffects::Stop(SoundEffectType Effect)
{
	m_ptrSoundEffects[int(Effect)]->StopAll();
}

void SoundEffects::Stop()
{
	for (SoundEffect* Effects : m_ptrSoundEffects)
	{
		Effects->StopAll();
	}
}
