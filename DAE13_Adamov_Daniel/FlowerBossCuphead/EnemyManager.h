#pragma once
#include <vector>
class Texture;
class Enemy;

class BulletManager;
class Cuphead;

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
	void UpdateEnemies(float elapsedSec, const Rectf& cameraBox, BulletManager& bulletManager, Cuphead& cuphead);
	void AnimateEnemies(float elapsedSec, const Rectf& cameraBox);

	Enemy* operator[](int index) const;
	size_t GetVectorSize() const;

private:
	std::vector<Enemy*> m_EnemiesVector;

	const Texture* m_SpikeSprite;
	const Texture* m_ChomperSprite;

	const Texture* m_TulipIdle;
	const Texture* m_TulipAttack;

	static int m_NREnemies;

	void IntializeEnemies();
};

