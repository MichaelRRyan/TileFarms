#ifndef GLOBALS_H
#define GLOBALS_H

//#define GENERATOR_DEBUG

#include "TileType.h"

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

	/// <summary>
	/// To string function to get a string value of the tile type
	/// </summary>
	/// <param name="t_type">Type of tile</param>
	/// <returns>String version of tile</returns>
	static std::string const toString(TileType t_type)
	{
		switch (t_type)
		{
		case TileType::Null:
			return "Null";
			break;
		case TileType::Grass:
			return "Grass";
			break;
		case TileType::Rock:
			return "Rock";
			break;
		case TileType::Slope:
			return "Slope";
			break;
		case TileType::Water:
			return "Water";
			break;
		case TileType::Tree:
			return "Tree";
			break;
		case TileType::WaterLily:
			return "WaterLily";
			break;
		case TileType::Bush:
			return "Bush";
			break;
		default:
			return "Unkown";
			break;
		}
	}

}

#endif // !GLOBALS_H