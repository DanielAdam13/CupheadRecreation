#pragma once
#include "Animator.h"
#include <vector>
class Texture;
class BulletManager;

class Cuphead final
{
public:
	explicit Cuphead(const Vector2f& position, bool playIntro, int hp);
	Cuphead(const Cuphead& character) = delete; // deleted because too much work to be honest + I don't need them for now
	Cuphead& operator=(const Cuphead& rhs) = delete;
	Cuphead(Cuphead&& character) = delete;
	Cuphead& operator=(Cuphead&& rhs) = delete;
	~Cuphead();

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates, const std::vector<Vector2f>& vertices, BulletManager& bulletManager);
	
	void StartDash();
	void ToggleParryState();

	void Hit();

	int GetHealth() const;
	bool IsShooting() const;
	bool IsParrying() const;

	void Parry();

	Vector2f GetPosition() const;
	Rectf GetBounds() const;

	Vector2f GetPlaceOfDeath() const;
	
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
	bool m_IsAlive;

	bool m_PlayingIntro;

	Movement m_CupheadMovementState;
	Shoot m_CupheadShootingState;

	bool m_KeyPressed;

	float m_ShootAngle;

	bool m_IsGrounded;
	bool m_IsHit;
	bool m_AnimatingHit;
	const float m_InvincibilityDuration;

	bool m_ClickedDash;
	bool m_IsDashing;
	float m_DashCooldownAcuuSec;
	float m_DashAnimAccuSec;

	const float m_MaxFrameSec;

	Texture* m_CurrentTexture;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	Vector2f m_Velocity;
	float m_FacingAngle;

	Vector2f m_PlaceOfHit;

	const Texture* m_TexturePeaShooter;
	const Texture* m_TextureSpecialPeaShooter;

	Texture* m_TextureDash;
	Texture* m_TextureShoot;
	Texture* m_TextureDeath;
	Texture* m_TextureDuck;
	Texture* m_TextureGhost;
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
	void CreateProjectiles(float elapsedSec, BulletManager& bulletManager) const;

	void UpdateFacingDirection(const Uint8* pStates);
	void ResetParry();

	void TakeDamage(float elapsedSec);
	void SetDeath();

	void IntializeTextures();
	void DeleteTextures();
};

