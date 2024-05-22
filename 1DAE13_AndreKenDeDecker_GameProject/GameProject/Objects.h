#pragma once
#include <iostream>
#include "Texture.h"
#include <vector>
#include "utils.h"
#include "Matrix2x3.h"
#include "SoundEffects.h"
class ScottPilgrim;
class EnemyMike;

class Objects
{
public:
	Objects(Point2f Position, float Width, float Height, SoundEffects* SoundEffects);
	~Objects();

	void Draw() const;
	void Update(float elapsedSec, const Point2f& Position, bool IsLeft, const std::vector<std::vector<Point2f>>& MapSvg);
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& MapSvg);
	void Collision(const std::vector<Point2f>& Hitbox, bool IsLeft);
	void PickUpPlayer(const std::vector<Point2f>& Hitbox, ScottPilgrim* Player);
	void PickUpEnemy(const std::vector<Point2f>& Hitbox, EnemyMike* Enemy);
	void DroppedObject(float YPosition);
	void ThrownObject(float YPosition);
	void ObjectHit(bool IsHit);
	bool CheckIfOverlapping(const std::vector<Point2f>& Hitbox);

	bool GetIsPickedUp() const;
	bool GetIsFlipped() const;
	bool GetDoDamage() const;
	bool GetIsLeft() const;
	int GetRumble() const;
	float GetFallYPosition() const;
	Point2f GetPosition() const;
	std::vector<Point2f> GetHitbox() const;
	ScottPilgrim* GetPlayer() const;
	EnemyMike* GetEnemy() const;
	

	void SetIsFlipped(bool IsFlipped);
	void SetRumble(int Rumble);

private:
	static Texture* m_ptrSpriteSheet;
	static int m_InstanceCounter;

	Point2f m_Position;
	bool m_IsInTheAir;
	bool m_IsSliding;
	bool m_IsPickedUp;
	bool m_IsLeft;
	bool m_IsFlipped;
	bool m_DoDamage;
	int m_Rumble;
	float m_Width;
	float m_Height;
	float m_FallYPosition;


	std::vector<Point2f> m_HitboxOrigin;
	std::vector<Point2f> m_HitboxTransformed;
	utils::HitInfo m_Hitinfo;
	Vector2f m_Velocity;

	//Class Association
	ScottPilgrim* m_ptrPlayer;
	EnemyMike* m_ptrEnemy;

	//Time
	float m_SlideCounter;
	const float m_MAX_SLIDE_DELAY{1.5f};

	//Sound 
	SoundEffects* m_ptrSoundEffects;

	//Functions
	void TranslateSprite() const;
	void ResetSprite() const;
	void UpdateXPosition(float elapsedSec);
	void UpdateYPosition(float elapsedSec);
	void CheckIfGoingOutOfBounds(const std::vector<std::vector<Point2f>>& MapSvg);
};

