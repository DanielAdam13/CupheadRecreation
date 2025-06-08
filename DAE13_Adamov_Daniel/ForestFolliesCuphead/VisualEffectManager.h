#pragma once
#include <vector>
class Effect;

class VisualEffectManager final
{
public:
	explicit VisualEffectManager();
	VisualEffectManager(const VisualEffectManager& effectManager) = delete;
	VisualEffectManager operator=(const VisualEffectManager& rhs) = delete;
	VisualEffectManager(VisualEffectManager&& effectManager) = delete;
	VisualEffectManager& operator=(VisualEffectManager&& rhs) = delete;
	~VisualEffectManager() noexcept;

	void DrawEffects() const;
	void UpdateEffects(float elapsedSec);

	void AddEffect(Effect* effect);

	//size_t GetVectorSize() const;

private:
	std::vector<Effect*> m_EffectsVector;

};

