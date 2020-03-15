#ifndef GLOBALS_H
#define GLOBALS_H

namespace Globals
{

	unsigned const WORLD_WIDTH_X{ 20 };
	unsigned const WORLD_WIDTH_Y{ 16 };
	unsigned const WORLD_HEIGHT{ 3 };

	float const TILE_SIZE{ 16.0f };

	float const VIEW_SCALE{ 3.0f };

	unsigned const WINDOW_WIDTH{ static_cast<unsigned>(VIEW_SCALE * TILE_SIZE * WORLD_WIDTH_X) };
	unsigned const WINDOW_HEIGHT{ static_cast<unsigned>(VIEW_SCALE * TILE_SIZE * WORLD_WIDTH_Y) };

}

#endif // !GLOBALS_H