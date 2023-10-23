#pragma once

#include "vec2.h"

class GameObject
{
protected:
	GameObject(const Vec2<int>& screenPos, const int& cellSize);
	virtual ~GameObject() = default;

	virtual void Update() = 0;
	virtual void Draw() const = 0;

	Vec2<int> screenPositionPixels;
	int cellSizePixels;
};
