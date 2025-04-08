#pragma once
#include "Animator.h"
#include <vector>
class Texture;
#include "Projectile.h"

class Cuphead final
{
public:
	explicit Cuphead(const Vector2f& position, bool playIntro);
	Cuphead(const Cuphead& character) = delete;
	Cuphead& operator=(const Cuphead& rhs) = delete;
	Cuphead(Cuphead&& character) = delete;
	Cuphead& operator=(Cuphead&& rhs) = delete;
	~Cuphead();

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates, const std::vector<Vector2f>& vertices);

	int GetHealth() const;
	
private:
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

	bool m_KeyPressed;

	bool m_IsShooting;
	float m_AccuSecProjectiles;
	float m_ShootAngle;

	bool m_IsGrounded;
	bool m_IsHit;

	const float m_MaxFrameSec;

	Texture* m_CurrentTexture;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	Vector2f m_Velocity;
	float m_FacingAngle;

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

	std::vector<Projectile*> m_ProjectileVector; // pointers -> control over lifetime, HAS A relationship


	// MEMBER FUNCTIONS
	void ProcessKeys(const Uint8* pStates);
	void AnimateCuphead(float elapsedSec);
	void UpdateProjectiles(float elapsedSec);
	void HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices);

	Rectf GetBounds() const;
	void UpdateFacingDirection(const Uint8* pStates);

	void IntializeTextures();
	void DeleteTextures();
};

