#pragma once
#include "Animator.h"
#include <vector>
class Texture;
class BulletManager;
class UIManager;

class Cuphead final
{
public:
	explicit Cuphead(const Vector2f& position, bool playIntro, int hp, const Texture* peaShooter, const Texture* peaSpecial);
	Cuphead(const Cuphead& character) = delete; // deleted because too much work to be honest + I don't need them for now
	Cuphead& operator=(const Cuphead& rhs) = delete;
	Cuphead(Cuphead&& character) = delete;
	Cuphead& operator=(Cuphead&& rhs) = delete;
	~Cuphead();

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates, const std::vector<Vector2f>& vertices, BulletManager& bulletManager, UIManager& uiManager);
	
	void StartDash();
	void ToggleParryState();

	void Hit(); // taking damage

	int GetHealth() const; // used for UI
	bool IsParrying() const; // used to parry in other classes

	void Parry(UIManager& uiManager); // actual parry

	Vector2f GetPosition() const;
	Rectf GetBounds() const;
	Circlef GetHitbox() const;

	Vector2f GetPlaceOfDeath() const; // for camera reset
	
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
	int m_HP;
	bool m_IsAlive;
	float m_FrameWidth;
	float m_FrameHeight;

	bool m_PlayingIntro;

	Movement m_CupheadMovementState;
	Shoot m_CupheadShootingState;

	Movement m_LastMovementState;
	Shoot m_LastShootState;

	bool m_KeyPressed;
	int m_FrameToResetAnimation;

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

	const Texture* m_CurrentTexture;
	int m_CurrentColNr;
	int m_CurrentRowNr;

	Vector2f m_Velocity;
	float m_FacingAngle;
	bool m_SpecialAllowed;
	bool m_FiringSpecial;

	Vector2f m_PlaceOfHit;

	const Texture* m_TexturePeaShooter;
	const Texture* m_TextureSpecialPeaShooter;

	Texture* m_TextureDash;
	Texture* m_TextureShoot;
	Texture* m_TextureShootSpecial;
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

	Animator m_Animator;

	// MEMBER FUNCTIONS
	void ProcessKeys(const Uint8* pStates, UIManager& uiManager);
	void AnimateCuphead(float elapsedSec);
	void ResetAnimation(int frameToReset);
	void HandleRaycast(float elapsedSec, const std::vector<Vector2f>& vertices);
	void Dash(float elapsedSec);
	void CreateProjectiles(float elapsedSec, BulletManager& bulletManager, UIManager& uiManager);

	void UpdateFacingDirection(const Uint8* pStates);

	void TakeDamage(float elapsedSec);
	void SetDeath();
	
	void IntializeTextures();
	void DeleteTextures();
};

