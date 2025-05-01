#pragma once
#include "Animator.h"
#include <vector>
class Texture;
#include "Projectile.h"
class BulletManager;

class Cuphead final
{
public:
	explicit Cuphead(const Vector2f& position, bool playIntro);
	Cuphead(const Cuphead& character) = delete; // deleted because too much work to be honest + I don't need them for now
	Cuphead& operator=(const Cuphead& rhs) = delete;
	Cuphead(Cuphead&& character) = delete;
	Cuphead& operator=(Cuphead&& rhs) = delete;
	~Cuphead();

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates, const std::vector<Vector2f>& vertices, BulletManager& bulletManager);
	
	void StartDash();
	void Parry();

	int GetHealth() const;
	Vector2f GetPosition() const;

	bool IsShooting() const;
	bool IsParrying() const;
	
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
	int m_HP;
	bool m_PlayingIntro;

	Movement m_CupheadMovementState;
	Shoot m_CupheadShootingState;

	bool m_KeyPressed;

	float m_AccuSecProjectiles;
	float m_ShootAngle;

	bool m_IsGrounded;
	bool m_IsHit;

	bool m_ClickedDash;
	bool m_IsDashing;
	float m_DashCooldownAcuuSec;

	const float m_MaxFrameSec;

	Texture* m_CurrentTexture;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	Vector2f m_Velocity;
	float m_FacingAngle;

	Texture* m_TexturePeaShooter;
	Texture* m_TextureSpecialPeaShooter;

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
	void ProcessKeys(const Uint8* pStates);
	void AnimateCuphead(float elapsedSec);
	void HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices);
	void Dash(float elapsedSec);
	void CreateProjectiles(float elapsedSec, BulletManager& bulletManager);

	void UpdateFacingDirection(const Uint8* pStates);
	void ResetParry();

	Rectf GetBounds() const;

	void IntializeTextures();
	void DeleteTextures();
};

