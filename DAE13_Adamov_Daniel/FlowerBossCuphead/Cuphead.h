#pragma once
#include "Animator.h"
class Texture;

class Cuphead final
{
public:
	enum class Movement
	{
		idle,
		runLeft,
		runRight,
		jump,
		dash,
		duck,
		parry
	};

	enum class Shoot
	{
		idle,
		shootUp,
		shootDiagonalUpRight,
		shootRight,
		shootDiagonalDownRight,
		shootDown,
		shootDiagonalUpLeft,
		shootLeft,
		shootDiagonalDownLeft,
		specialLeft,
		specialUp,
		specialRight,
		specialDown
	};

	explicit Cuphead(const Vector2f& position);
	~Cuphead();

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates);

	
private:
	//MEMBER VARIABLES
	Vector2f m_Position;
	const float m_Speed;
	int m_HP;
	bool m_PlayingIntro;

	Movement m_CupheadMovementState;
	Shoot m_CupheadShootingState;

	bool m_IsHit;

	float m_AccuSec;
	float m_MaxFrameSec;
	int m_CurrentFrame;

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
	void PlayIntro(float elapsedSec);
	Rectf GetBounds(float frameWidth, float frameHeight) const;

	void IntializeTextures();
	void DeleteTextures();
};

