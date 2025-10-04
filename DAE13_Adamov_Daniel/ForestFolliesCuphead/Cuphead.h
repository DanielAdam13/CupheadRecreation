#pragma once
#include "Animator.h"
#include <vector>
#include <memory>
class Texture;
class BulletManager;
class UIManager;
class SoundEffect;

class Cuphead final // Character controller
{
public:
	explicit Cuphead(const Vector2f& position, bool playIntro, int hp, const std::unique_ptr<Texture>& peaShooter, const std::unique_ptr<Texture>& peaSpecial);
	Cuphead(const Cuphead& character) = delete;
	Cuphead& operator=(const Cuphead& rhs) = delete;
	Cuphead(Cuphead&& character) = delete;
	Cuphead& operator=(Cuphead&& rhs) = delete;
	~Cuphead() noexcept;

	void Draw() const;
	void Update(float elapsedSec, const Uint8* pStates, const std::vector<std::vector<Vector2f>>& vertices, BulletManager& bulletManager, UIManager& uiManager);
	
	void StartDash();
	void ToggleParryState();

	void Hit(); // taking damage

	int GetHealth() const; // used for UI
	bool IsParrying() const; // used to parry in other classes

	void Parry(UIManager& uiManager); // actual parry

	void ResetPlayer(const Vector2f& pos);

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
	bool m_CanFall;

	bool m_IsHit;
	bool m_AnimatingHit;
	const float m_InvincibilityDuration;
	bool m_ShouldDraw;
	bool m_CanPlayDeathSFX;

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

	const Texture* m_TexturePeaShooter; // reference to unique_ptr
	const Texture* m_TextureSpecialPeaShooter; // reference to unique_ptr
	
	std::unique_ptr<Texture> m_TextureDash;
	std::unique_ptr<Texture> m_TextureShoot;
	std::unique_ptr<Texture> m_TextureShootSpecial;
	std::unique_ptr<Texture> m_TextureDuck;
	std::unique_ptr<Texture> m_TextureGhost;
	std::unique_ptr<Texture> m_TextureHit;
	std::unique_ptr<Texture> m_TextureIdle;
	std::unique_ptr<Texture> m_TextureIntro;
	std::unique_ptr<Texture> m_TextureJump;
	std::unique_ptr<Texture> m_TextureParry;
	std::unique_ptr<Texture> m_TextureRun;
	std::unique_ptr<Texture> m_TextureRunShootDiagonal;
	std::unique_ptr<Texture> m_TextureRunShootStraight;

	std::unique_ptr<SoundEffect> m_DashSFX;
	std::unique_ptr<SoundEffect> m_JumpSFX;
	std::unique_ptr<SoundEffect> m_ParrySFX;
	std::unique_ptr<SoundEffect> m_SlapSFX;
	std::unique_ptr<SoundEffect> m_DeathSFX;
	std::unique_ptr<SoundEffect> m_CrackSFX;
	std::unique_ptr<SoundEffect> m_HitSFX1;
	std::unique_ptr<SoundEffect> m_HitSFX2;

	std::unique_ptr<SoundEffect> m_PeaShooterSFX1;
	std::unique_ptr<SoundEffect> m_PeaShooterSFX2;
	std::unique_ptr<SoundEffect> m_PeaShooterSFX3;
	std::unique_ptr<SoundEffect> m_SpecialPeaSFX1;

	Animator m_Animator;

	// MEMBER FUNCTIONS
	void ProcessKeys(const Uint8* pStates, UIManager& uiManager);
	void AnimateCuphead(float elapsedSec);
	void ResetAnimation(int frameToReset);
	void HandleRaycast(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices);
	void Dash(float elapsedSec);
	void CreateProjectiles(float elapsedSec, BulletManager& bulletManager, UIManager& uiManager);

	void UpdateFacingDirection(const Uint8* pStates);

	void TakeDamage(float elapsedSec);
	void FlickerWhenHit(float elapsedSec);
	void SetDeath();
	
	void IntializeTextures();
	void InitializeSoundEffects();
};

