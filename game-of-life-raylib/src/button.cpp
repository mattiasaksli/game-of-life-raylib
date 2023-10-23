#include "button.h"

#include "raylib_cpp.h"
#include "settings.h"

Button::Button()
	: GameObject({}, 0),
      spriteRect({}),
      spriteRectHighlighted({}),
      onButtonClicked(nullptr)
{
}

Button::Button(const Vec2<int> &screenPos,
               const int &cellSize,
               const Rectangle &buttonSprite,
               const Rectangle &buttonSpriteHighlighted,
               CallbackOnButtonClick callback)
    : GameObject(screenPos, cellSize),
      spriteRect(buttonSprite),
      spriteRectHighlighted(buttonSpriteHighlighted),
      onButtonClicked(std::move(callback))
{
}

void Button::Update()
{
	isHighlighted = false;
	
	auto [mouseX, mouseY] = GetMousePosition();
    const Vec2<int> mousePos =
    		Vec2<int>(static_cast<int>(mouseX), static_cast<int>(mouseY))
    		- screenPositionPixels;

    // Check if the mouse is within the button area
    if (mousePos.x <= 0 || mousePos.x >= Settings::BUTTON_SIZE_IN_CELLS * cellSizePixels
    	|| mousePos.y <= 0 || mousePos.y >= Settings::BUTTON_SIZE_IN_CELLS * cellSizePixels)
    {
    	return;
    }
    
    isHighlighted = true;
    
    // Button clicked
    if (IsMouseButtonReleased(RaylibCpp::MOUSE_LEFT_CLICK))
    {
		onButtonClicked();
    }
    // Preview click
    else if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK))
    {
    	isHighlighted = false;
    }
}

void Button::Draw() const 
{
	Rectangle spriteRectToDraw;
	if (isHighlighted)
	{
		spriteRectToDraw = spriteRectHighlighted;
	}
	else
	{
		spriteRectToDraw = spriteRect;
    }
    
    RaylibCpp::DrawTexturePro(
        ResourcesSingleton::GetInstance().GetCellAtlasTexture(),
        spriteRectToDraw,
        screenPositionPixels,
        Vec2<int>{cellSizePixels * Settings::BUTTON_SIZE_IN_CELLS},
        Vec2<int>{0, 0},
        0,
        WHITE
    );
}
