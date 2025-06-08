#pragma once
#include <vector>
class Enemy;
class BulletManager;
class Cuphead;
class UIManager;
class VisualEffectManager;

class Texture;

class EnemyManager final
{
public:
	explicit EnemyManager(const Texture* enemyDeathVFX);
	EnemyManager(const EnemyManager& enemyManager) = delete;
	EnemyManager& operator=(const EnemyManager& rhs) = delete;
	EnemyManager(EnemyManager&& enemyManager) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) = delete;
	~EnemyManager() noexcept;

	void DrawEnemies(const Rectf& cameraBox) const;
	void UpdateEnemies(float elapsedSec, const Rectf& cameraBox, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager, VisualEffectManager& vfxManager);
	void AnimateEnemies(float elapsedSec, const Rectf& cameraBox);
	void DeleteAllEnemies() noexcept;

	void AddEnemy(Enemy* enemy);
	Enemy* operator[](int index) const;

	size_t GetVectorSize() const;

private:
	std::vector<Enemy*> m_EnemyVector;

	const Texture* m_EnemyDeathSpriteVFX;
};

