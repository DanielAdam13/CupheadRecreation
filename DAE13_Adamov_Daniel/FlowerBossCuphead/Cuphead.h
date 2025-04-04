#pragma once
#include "Animator.h"
#include <vector>
class Texture;

class Cuphead final
{
public:
	enum class Movement
	{
		idle,
		runLeft,
		runRight,
		lock,
		jump,
		dash,
		duck,
		parry
	};

	enum class Shoot
	{
		notShooting,
		shootUp,
		shootDiagonalUpRight,
		shootRight,
		shootDiagonalUpLeft,
		shootLeft,
		shootDown,
		specialUp,
		specialRight,
		specialLeft,
		specialDown
	};

	explicit Cuphead(const Vector2f& position);
	~Cuphead();

	void Draw() const;
	void ProcessKeys(const Uint8* pStates);
	void AnimateCuphead(float elapsedSec);
	void HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices);

	int GetHealth() const;
	
private:
	//MEMBER VARIABLES
	Vector2f m_Position;
	static float m_FrameWidth;
	static float m_FrameHeight;
	const float m_Speed;
	int m_HP;
	bool m_PlayingIntro;

	Movement m_CupheadMovementState;
	Shoot m_CupheadShootingState;

	// important for reseting animations between each other
	Movement m_LastMovementState;
	Shoot m_LastShootState;

	bool m_IsHit;

	float m_MaxFrameSec;

	Texture* m_CurrentTexture;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	Vector2f m_Velocity;

	Texture* m_TextureDash;
	Texture* m_TextureShoot;
	Texture* m_TextureDeath;
	Texture* m_TextureDuck;
	Texture* m_TextureHit;
	Texture* m_TextureIdle;
	Texture* m_TextureIntro;
	Texture* m_TextureJump;
	Texture* m_TextureParry;
	Texture* m_TextureRun;
	Texture* m_TextureRunShootDiagonal;
	Texture* m_TextureRunShootStraight;
	Texture* m_TextureSpecialAttack;

	Animator m_Animator;


	// MEMBER FUNCTIONS
	Rectf GetBounds() const;

	void IntializeTextures();
	void DeleteTextures();
};

