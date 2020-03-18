#ifndef GLOBALS_H
#define GLOBALS_H

//#define GENERATOR_DEBUG

namespace Globals
{

	unsigned const WORLD_WIDTH_X{ 50 };
	unsigned const WORLD_WIDTH_Y{ 50 };
	unsigned const WORLD_HEIGHT{ 6 };

	float const TILE_SIZE{ 16.0f };

	float const VIEW_SCALE{ 4.0f };

	unsigned const WINDOW_WIDTH{ 1280u };
	unsigned const WINDOW_HEIGHT{ 720u };

	std::string const GAME_TITLE{ "Tile Farms" };

}

#endif // !GLOBALS_H