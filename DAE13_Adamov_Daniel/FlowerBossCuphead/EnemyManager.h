#pragma once
#include <vector>
class Enemy;
class BulletManager;
class Cuphead;
class UIManager;

class EnemyManager final
{
public:
	explicit EnemyManager();
	EnemyManager(const EnemyManager& enemyManager) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&& enemyManager) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) = delete;
	~EnemyManager();

	void DrawEnemies(const Rectf& cameraBox) const;
	void UpdateEnemies(float elapsedSec, const Rectf& cameraBox, BulletManager& bulletManager, Cuphead& cuphead, UIManager& uiManager);
	void AnimateEnemies(float elapsedSec, const Rectf& cameraBox);

	void AddEnemy(Enemy* enemy);
	Enemy* operator[](int index) const;

	size_t GetVectorSize() const;

private:
	std::vector<Enemy*> m_EnemiesVector;
};

