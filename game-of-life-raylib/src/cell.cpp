#include "board.h"
#include "resources_singleton.h"


//	####################
//	# Public functions #
//	####################


Board::Cell::Cell(const Vec2<int>&& pos)
	: cellPosition(pos)
{
}

void Board::Cell::Draw(const Vec2<int>& boardOriginPixels, const int& cellSizePixels) const
{
	const Vec2<int> topLeft = boardOriginPixels + (cellPosition * cellSizePixels);
	const Rectangle spriteRect = GetCellSprite();

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetCellAtlasTexture(),
		spriteRect,
		topLeft,
		Vec2<int>{cellSizePixels},
		Vec2<int>{0, 0},
		0,
		WHITE
	);
}


//	#####################
//	# Private functions #
//	#####################


Rectangle Board::Cell::GetCellSprite() const
{
	if (!isAlive)
	{
		if (isHighlighted)
		{
			return Sprite::DEAD_HIGHLIGHTED;
		}

		return Sprite::DEAD;
	}

	if (isHighlighted)
	{
		return Sprite::ALIVE_HIGHLIGHTED;
	}

	return Sprite::ALIVE;
}
