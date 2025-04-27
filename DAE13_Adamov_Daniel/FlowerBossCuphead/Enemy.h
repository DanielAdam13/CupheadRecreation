#pragma once
#include "Animator.h"

class Enemy abstract
{
public:
	Enemy();
	virtual ~Enemy() = default;

	virtual void Draw() const = 0;
	//virtual void Update() = 0;

	virtual Rectf GetBounds() const = 0;
protected:
	Animator m_Animator;
private:
	
};

