#include "../include/game_object.h"

GameObject::GameObject(const Vec2<int>& screenPos, const int& cellSize)
	: screenPositionPixels(screenPos),
	  cellSizePixels(cellSize)
{
}
