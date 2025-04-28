#pragma once
#include <vector>
class Texture;
class Enemy;
class Spike;
class BigChomper;

class EnemyManager final
{
public:
	explicit EnemyManager();
	EnemyManager(const EnemyManager& enemyManager) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&& enemyManager) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) = delete;
	~EnemyManager();

	void DrawEnemies() const;
	void UpdateEnemies(float elapsedSec);

private:
	std::vector<Enemy*> m_EnemiesVector;

	Texture* m_SpikeSprite;
	Texture* m_ChomperSprite;

	static int m_NREnemies;

	void IntializeEnemies();
};

