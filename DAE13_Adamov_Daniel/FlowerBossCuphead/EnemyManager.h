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
	~EnemyManager();

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	void DrawEnemies() const;
	void UpdateEnemies(float elapsedSec);


private:
	std::vector<Enemy*> m_EnemiesVector;

	Texture* m_SpikeSprite;
	Texture* m_ChomperSprite;

	static int m_NREnemies;

	void IntializeEnemies();
};

