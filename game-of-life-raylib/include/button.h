#pragma once

#include "game_object.h"
#include "resources_singleton.h"

#include <functional>

class Button final : GameObject
{
public:
    typedef std::function<void()> CallbackOnButtonClick;

    Button();
    Button(const Vec2<int>& screenPos, const int& cellSize, const Rectangle& buttonSprite, const Rectangle& buttonSpriteHighlighted, CallbackOnButtonClick callback);

    void Update() override;
    void Draw() const override;
    
private:
    Rectangle spriteRect;
    Rectangle spriteRectHighlighted;
    bool isHighlighted = false;

    CallbackOnButtonClick onButtonClicked;
};
