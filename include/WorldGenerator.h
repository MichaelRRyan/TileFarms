#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "World.h"
#include "PerlinNoiseGenerator.h"

#include <iostream>

class WorldGenerator
{
public:

	static void generateWorld(World& t_world);

};

#endif // !WORLD_GENERATOR_H