#pragma once

#include <raylib.h>

// Rectangles correspond to sprites in resources/sprite_atlas.png
namespace Sprite
{
	constexpr Rectangle DEAD{64, 0, 32, 32};
	constexpr Rectangle DEAD_HIGHLIGHTED{96, 0, 32, 32};

	constexpr Rectangle ALIVE{0, 0, 32, 32};
	constexpr Rectangle ALIVE_HIGHLIGHTED{32, 0, 32, 32};

	constexpr Rectangle PAUSE{0, 32, 32, 32};
	constexpr Rectangle PAUSE_HIGHLIGHTED{0, 64, 32, 32};

	constexpr Rectangle PLAY{32, 32, 32, 32};
	constexpr Rectangle PLAY_HIGHLIGHTED{32, 64, 32, 32};

	constexpr Rectangle SPEED_UP{64, 32, 32, 32};
	constexpr Rectangle SPEED_UP_HIGHLIGHTED{64, 64, 32, 32};
	
	constexpr Rectangle SLOW_DOWN{96, 32, 32, 32};
	constexpr Rectangle SLOW_DOWN_HIGHLIGHTED{96, 64, 32, 32};
}

class ResourcesSingleton
{
public:
	ResourcesSingleton(const ResourcesSingleton&) = delete;
	ResourcesSingleton& operator=(const ResourcesSingleton&) = delete;
	ResourcesSingleton(const ResourcesSingleton&&) = delete;
	ResourcesSingleton& operator=(const ResourcesSingleton&&) = delete;

	static ResourcesSingleton& GetInstance();

	Texture2D& GetCellAtlasTexture();

private:
	Texture2D cellAtlasTexture{};

	ResourcesSingleton();
	~ResourcesSingleton();
};
