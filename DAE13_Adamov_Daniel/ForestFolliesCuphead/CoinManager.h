#pragma once
#include <vector>
class Coin;
class Texture;
class SoundEffect;

class CoinManager final
{
public:
	explicit CoinManager(const Texture* coinSprite, const SoundEffect* coinPickupSFX);
	CoinManager(const CoinManager& coinManager) = delete;
	CoinManager& operator=(const CoinManager& rhs) = delete;
	CoinManager(CoinManager&& coinManager) = delete;
	CoinManager& operator=(CoinManager&& rhs) = delete;
	~CoinManager() noexcept;

	void DrawCoins(const Rectf& cameraBox) const;
	void UpdateCoins(float elapsedSec, const Rectf& cameraBox, const Circlef& playerBounds);
	void DeleteAllCoins() noexcept;

	void AddCoin(const Vector2f& pos);
	Coin* operator[](int index) const;

	size_t GetVecorSize() const;

	int GetTakenCoinsNumber() const;

private:
	std::vector<Coin*> m_CoinVector;

	const Texture* m_CoinTexture;

	static int coinsTaken;

	const SoundEffect* m_CoinPickupSFX;
};

