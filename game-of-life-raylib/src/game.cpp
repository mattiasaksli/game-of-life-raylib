#include "game.h"
#include "raylib_cpp.h"
#include "resources_singleton.h"
#include "settings.h"

#include <cassert>

#if defined DEBUG
#include <iostream>
#endif

using namespace Settings;

Game::Game(const std::string& title,
           const int cellSize,
           const int boardWidth,
           const int boardHeight)
	: cellSizePixels(cellSize),
	  borderThicknessPixels(cellSize / 2),
	  boardWidthCells(boardWidth),
	  boardHeightCells(boardHeight),
	  
	  board(cellSizePixels, boardWidthCells, boardHeightCells)
{
	assert(!GetWindowHandle() && "Cannot initialize window, since window is already open");

	const Button::CallbackOnButtonClick onSlowDownButtonClicked = [this]() {
    	if (simulationTickRate < SIMULATION_TICK_RATE_MIN_SPEED)
    	{
    		simulationTickRate *= 2;
    	}
    };
	const Button::CallbackOnButtonClick onSpeedUpButtonClicked = [this]() {
		if (simulationTickRate > SIMULATION_TICK_RATE_MAX_SPEED)
		{
			simulationTickRate /= 2;
		}
	};
	const Button::CallbackOnButtonClick onPlayPauseButtonClicked = [this]() {
		isGamePaused = !isGamePaused;
	};

	// It's more inefficient to initialise the buttons here, since we use a default Button constructor first as part
	// of the initializer list stage above, but this doesn't affect performance (since it's only done at startup)
	// and is much more readable than having the lambdas in the initializer list
	slowDownButton	= Button(GetSlowDownButtonScreenPosition(cellSize, boardWidthCells),	cellSize, Sprite::SLOW_DOWN,	Sprite::SLOW_DOWN_HIGHLIGHTED,	onSlowDownButtonClicked);
	playButton		= Button(GetPlayPauseButtonScreenPosition(cellSize, boardWidthCells),	cellSize, Sprite::PLAY,			Sprite::PLAY_HIGHLIGHTED,		onPlayPauseButtonClicked);
	pauseButton		= Button(GetPlayPauseButtonScreenPosition(cellSize, boardWidthCells),	cellSize, Sprite::PAUSE,		Sprite::PAUSE_HIGHLIGHTED,		onPlayPauseButtonClicked);
	speedUpButton	= Button(GetSpeedUpButtonScreenPosition(cellSize, boardWidthCells),		cellSize, Sprite::SPEED_UP,		Sprite::SPEED_UP_HIGHLIGHTED,	onSpeedUpButtonClicked);
	
	// Initialize window
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	const int windowWidth = boardWidthCells * cellSizePixels + cellSizePixels;
	const int windowHeight = boardHeightCells * cellSizePixels + cellSizePixels * 4 + borderThicknessPixels;

	InitWindow(windowWidth, windowHeight, title.c_str());

	const Image gameIcon = LoadImage("resources/icon.png");
	SetWindowIcon(gameIcon);
}

Game::~Game() noexcept
{
	assert(GetWindowHandle() && "Cannot close window, since window is already closed");
	CloseWindow();
}

bool Game::GameShouldClose()
{
	return WindowShouldClose();
}

void Game::Tick()
{
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

void Game::Update()
{
	if (simulationTickRate < SIMULATION_TICK_RATE_MIN_SPEED)
	{
		slowDownButton.Update();
	}
	if (simulationTickRate > SIMULATION_TICK_RATE_MAX_SPEED)
	{
		speedUpButton.Update();
	}

	if (!isGamePaused)
	{
		pauseButton.Update();
		
		board.SimulationTick(simulationTickRate);
	}
	else
	{
		playButton.Update();

		board.Update();
	}
}

void Game::Draw() const
{
	ClearBackground(Color{50, 50, 50, 255});

	// Draw borders
	RaylibCpp::DrawRectangleLinesEx(
		Vec2<int>{0},
		Vec2{
			boardWidthCells * cellSizePixels,
			boardHeightCells * cellSizePixels + cellSizePixels * 3 + borderThicknessPixels
		} + cellSizePixels,
		borderThicknessPixels,
		DARKGRAY
	);
	RaylibCpp::DrawLineEx(
		Vec2<int>{borderThicknessPixels, cellSizePixels * 4 - borderThicknessPixels / 2},
		Vec2<int>{
			boardWidthCells * cellSizePixels + borderThicknessPixels,
			cellSizePixels * 4 - borderThicknessPixels / 2
		},
		borderThicknessPixels,
		DARKGRAY
	);

	if (simulationTickRate < SIMULATION_TICK_RATE_MIN_SPEED)
	{
		slowDownButton.Draw();
	}
	if (simulationTickRate > SIMULATION_TICK_RATE_MAX_SPEED)
	{
		speedUpButton.Draw();
	}
	
	if (!isGamePaused)
	{
		pauseButton.Draw();
	}
	else
	{
		playButton.Draw();
	}

	board.Draw();
}
