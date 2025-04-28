#pragma once
#include "Animator.h"

class Enemy abstract
{
public:
	Enemy(const Vector2f& pos);
	virtual ~Enemy() = default;
	// no need for rule of 5 since constructos and operators aren't inherited

	virtual void Draw() const = 0;
	//virtual void Update() = 0;

	virtual Rectf GetBounds() const = 0;
protected:
	Animator m_Animator;
	Vector2f m_Positon;
private:
	
};

