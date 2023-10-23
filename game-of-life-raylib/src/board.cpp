#include "board.h"

#include <cassert>
#include <chrono>

#include "settings.h"


//	####################
//	# Public functions #
//	####################


Board::Board(const int cellSize,
             const int boardWidthCells,
             const int boardHeightCells)
	: GameObject(Settings::GetBoardScreenPosition(cellSize), cellSize),
	  widthCells(boardWidthCells),
	  heightCells(boardHeightCells)
{
	assert(widthCells > 0 && heightCells > 0 && "The board width or height cannot be smaller than 1");
	assert(cellSizePixels > 0 && "The board cell size cannot be smaller than 1");

	cells.reserve(static_cast<size_t>(widthCells) * heightCells);
	for (int y = 0; y < heightCells; ++y)
	{
		for (int x = 0; x < widthCells; ++x)
		{
			cells.emplace_back(Vec2<int>(x, y));
		}
	}
}

void Board::Update()
{
	ResetCellsSpriteState();

	auto [mouseX, mouseY] = GetMousePosition();
	const Vec2<int> mousePos =
			Vec2<int>(static_cast<int>(mouseX), static_cast<int>(mouseY))
			- screenPositionPixels;

	// Check if the mouse is within the board area
	if (mousePos.x <= 0 || mousePos.x >= widthCells * cellSizePixels
		|| mousePos.y <= 0 || mousePos.y >= heightCells * cellSizePixels)
	{
		return;
	}

	const Vec2<int> selectedCellPos = GetCellPositionUnderMouse(mousePos);
	Cell& newlySelectedCell = cells[selectedCellPos.y * widthCells + selectedCellPos.x];

	if (ptr_mouseSelectedCell != &newlySelectedCell)
	{
		// Reset previous if the previous cell can be toggled
		if (ptr_mouseSelectedCell != nullptr && !ptr_mouseSelectedCell->CanBeToggled())
		{
			ptr_mouseSelectedCell->SetCellCanBeToggled(true);
		}
		
		ptr_mouseSelectedCell = &newlySelectedCell;
	}
	
	ProcessInput();
}

void Board::Draw() const
{
	for (const auto& cell : cells)
	{
		cell.Draw(
			screenPositionPixels,
			cellSizePixels
		);
	}
}

void Board::SimulationTick(const double& tickRate)
{
	const double timeElapsed = GetTime() - gameStartTime;
	if (timeElapsed < tickRate)
	{
		return;
	}
	gameStartTime = GetTime();
	
	for (Cell& cell : cells)
	{
		// Check how many adjacent cells are alive
		int numOfAdjacentCellsAlive = 0;
		const CallbackOnAdjacentCells checkAdjacentFlaggedCells = [&numOfAdjacentCellsAlive](const Cell& adjacentCell)
		{
			if (adjacentCell.IsAlive())
			{
				numOfAdjacentCellsAlive++;
			}
		};
		ProcessCellNeighbors(cell, checkAdjacentFlaggedCells);

		// Mark any cell status changes
		if (cell.IsAlive())
		{
			if (numOfAdjacentCellsAlive > 3 || numOfAdjacentCellsAlive < 2)
			{
				cell.MarkForDead();
			}
		}
		else
		{
			if (numOfAdjacentCellsAlive == 3)
			{
				cell.MarkForAlive();
			}
		}
	}

	// Set new cell status for any marked cells
	for (Cell& cell : cells)
	{
		if (cell.IsMarkedForAlive())
		{
			cell.SetCellStatus(true);
		}
		else if (cell.IsMarkedForDead())
		{
			cell.SetCellStatus(false);
		}

		cell.ResetCellMarkedStatus();
	}
}


//	#####################
//	# Private functions #
//	#####################

void Board::ResetCellsSpriteState() const
{
	// Resets old cell highlighting
	if (ptr_mouseSelectedCell != nullptr)
	{
		ptr_mouseSelectedCell->Highlight(false);
	}
}

void Board::ProcessInput() const
{
	if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK) && !ptr_mouseSelectedCell->IsAlive()
		|| IsMouseButtonDown(RaylibCpp::MOUSE_RIGHT_CLICK) && ptr_mouseSelectedCell->IsAlive())
	{
		ToggleClickedCell(*ptr_mouseSelectedCell);
		ptr_mouseSelectedCell->SetCellCanBeToggled(false); // Lock the cell so that we don't toggle the status every frame that the mouse button is held down
	}
	else if (ptr_mouseSelectedCell->CanBeToggled())
	{
		ptr_mouseSelectedCell->Highlight(true);
	}
}

Vec2<int> Board::GetCellPositionUnderMouse(const Vec2<int>& mousePosPixels) const
{
	const Vec2<int> cellPos = {
		mousePosPixels.x / cellSizePixels,
		mousePosPixels.y / cellSizePixels
	};

	assert(cellPos.x < widthCells && cellPos.y < heightCells && "Trying to access cell position out of bounds");

	return cellPos;
}

void Board::ToggleClickedCell(Cell& currentCell)
{
	currentCell.SetCellStatus(!currentCell.IsAlive());
}

void Board::ProcessCellNeighbors(const Cell& cellToProcess, const CallbackOnAdjacentCells& callback)
{
	Cell* adjacentCell;
	auto [x, y] = cellToProcess.GetCellPosition();

	//[o] [ ] [ ]
	//[ ]  x  [ ]
	//[ ] [ ] [ ]
	x -= 1;
	y -= 1;
	if (x >= 0 && y >= 0)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [o] [ ]
	//[ ]  x  [ ]
	//[ ] [ ] [ ]
	x += 1;
	if (y >= 0)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [o]
	//[ ]  x  [ ]
	//[ ] [ ] [ ]
	x += 1;
	if (x < widthCells && y >= 0)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [ ]
	//[o]  x  [ ]
	//[ ] [ ] [ ]
	x -= 2;
	y += 1;
	if (x >= 0)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [ ]
	//[ ]  x  [o]
	//[ ] [ ] [ ]
	x += 2;
	if (x < widthCells)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [ ]
	//[ ]  x  [ ]
	//[o] [ ] [ ]
	x -= 2;
	y += 1;
	if (x >= 0 && y < heightCells)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [ ]
	//[ ]  x  [ ]
	//[ ] [o] [ ]
	x += 1;
	if (y < heightCells)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}

	//[ ] [ ] [ ]
	//[ ]  x  [ ]
	//[ ] [ ] [o]
	x += 1;
	if (x < widthCells && y < heightCells)
	{
		adjacentCell = &cells[y * widthCells + x];
		callback(*adjacentCell);
	}
}
