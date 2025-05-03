#pragma once
#include <vector>
class Texture;
class Projectile;

class BulletManager
{
public:
	explicit BulletManager();
	BulletManager(const BulletManager& other) = delete;
	BulletManager& operator=(const BulletManager& rhs) = delete;
	BulletManager(BulletManager&& other) = delete;
	BulletManager& operator=(BulletManager&& rhs) = delete;
	~BulletManager();

	void DrawActiveBullets() const;
	void UpdateActiveBullets(float elapsedSec, const Rectf& cameraBox, const std::vector<Vector2f>& vertices);
	void AnimateActiveBullets(float elapsedSec);

	void AddProjectile(Projectile* projectile);
	Projectile* operator[](int index) const;

	size_t GetVectorSize() const;
	
	void RemoveProjectile(int index);

private:
	std::vector<Projectile*> m_Projectiles;
};

