#pragma once

#include "game_object.h"

#include "raylib_cpp.h"
#include "vec2.h"

#include <vector>
#include <functional>

class Board final : GameObject
{
private:
	class Cell
	{
	public:
		explicit Cell(const Vec2<int>&& pos);
		~Cell() = default;
		Cell(Cell&) = delete;
		Cell& operator=(Cell&) = delete;
		Cell(Cell&&) = default;
		Cell& operator=(Cell&&) = default;

		[[nodiscard]] bool IsAlive() const { return isAlive; }
		[[nodiscard]] Vec2<int> GetCellPosition() const { return cellPosition; }
		[[nodiscard]] bool CanBeToggled() const { return canBeToggled; }
		[[nodiscard]] bool IsMarkedForAlive() const { return shouldBeAlive; }
		[[nodiscard]] bool IsMarkedForDead() const { return shouldBeDead; }

		void Initialize(const std::vector<Cell*>& neighborCells) { adjacentCells = neighborCells; }
		void Highlight(const bool& isActive) { isHighlighted = isActive; }
		void SetCellStatus(const bool& alive) { isAlive = alive; }
		void SetCellCanBeToggled(const bool& canCellBeToggled) { canBeToggled = canCellBeToggled; }
		void MarkForAlive() { shouldBeAlive = true; }
		void MarkForDead() { shouldBeDead = true; }
		void ResetCellMarkedStatus() { shouldBeAlive = false; shouldBeDead = false; }

		void Draw(const Vec2<int>& boardOriginPixels, const int& cellSizePixels) const;

	private:
		bool isAlive = false;
		bool isHighlighted = false;
		bool canBeToggled = true;
		bool shouldBeAlive = false;
		bool shouldBeDead = false;

		Vec2<int> cellPosition = {};
		std::vector<Cell*> adjacentCells = {};

		[[nodiscard]] Rectangle GetCellSprite() const;
	};

public:
	Board(int cellSize, int boardWidthCells, int boardHeightCells);

	void Update() override;
	void Draw() const override;
	void SimulationTick(const double& tickRate);

private:
	std::vector<Cell> cells = {};
	Cell* ptr_mouseSelectedCell = nullptr;
	int widthCells;
	int heightCells;
	double gameStartTime = 0;

	typedef std::function<void (Cell& adjacentCell)> CallbackOnAdjacentCells;

	void ResetCellsSpriteState() const;
	void ProcessInput() const;
	[[nodiscard]] Vec2<int> GetCellPositionUnderMouse(const Vec2<int>& mousePosPixels) const;
	static void ToggleClickedCell(Cell& currentCell);

	void ProcessCellNeighbors(const Cell& cellToProcess, const CallbackOnAdjacentCells& callback);
};
