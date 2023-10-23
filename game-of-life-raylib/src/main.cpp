#include "game.h"

int main()
{
	constexpr int cellSize = 32;
	constexpr int boardWidthCells = 30;
	constexpr int boardHeightCells = 30;

	Game game{
		"Game of Life",
		cellSize,
		boardWidthCells,
		boardHeightCells,
	};

	while (!Game::GameShouldClose())
	{
		game.Tick();
	}

	return 0;
}
