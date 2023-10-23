#pragma once

#include "vec2.h"

namespace Settings
{
	constexpr int BUTTON_SIZE_IN_CELLS = 2;
	
	constexpr double SIMULATION_TICK_RATE_MAX_SPEED = 0.03125;
	constexpr double SIMULATION_TICK_RATE_MIN_SPEED = 2;
	
	// Utils
	static Vec2<int> GetBoardScreenPosition(const int cellSizePixels)
	{
		return Vec2<int>{
			cellSizePixels / 2,
			cellSizePixels * 4
		};
	}

	static Vec2<int> GetSlowDownButtonScreenPosition(const int cellSizePixels, const int boardWidthCells)
	{
		return Vec2<int>{
			cellSizePixels * boardWidthCells / 2 - cellSizePixels * BUTTON_SIZE_IN_CELLS - cellSizePixels,
			cellSizePixels
		};
	}

	static Vec2<int> GetPlayPauseButtonScreenPosition(const int cellSizePixels, const int boardWidthCells)
	{
		return Vec2<int>{
			cellSizePixels * boardWidthCells / 2,
			cellSizePixels
		};
	}

	static Vec2<int> GetSpeedUpButtonScreenPosition(const int cellSizePixels, const int boardWidthCells)
	{
		return Vec2<int>{
			cellSizePixels * boardWidthCells / 2 + cellSizePixels * BUTTON_SIZE_IN_CELLS + cellSizePixels,
			cellSizePixels
		};
	}
}
