#pragma once

#include "board.h"
#include "button.h"

#include <string>

class Game final
{
public:
	Game(const std::string& title, int cellSize, int boardWidth, int boardHeight);
	~Game() noexcept;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(const Game&&) = delete;
	Game& operator=(const Game&&) = delete;

	[[nodiscard]] static bool GameShouldClose();
	void Tick();

private:
	int cellSizePixels;
	int borderThicknessPixels;
	int boardWidthCells;
	int boardHeightCells;
	Board board;
	Button slowDownButton;
	Button playButton;
	Button pauseButton;
	Button speedUpButton;

	double simulationTickRate = 1.0;	// How many seconds to wait until the next tick
	bool isGamePaused = true;

	void Update();
	void Draw() const;
};
