#include "FileHandler.h"

bool const FileHandler::saveWorldToFile(World const& t_world, std::string const& t_fileName)
{
	std::ofstream outputFile;

	outputFile.open("world_data/" + t_fileName + ".world");

	if (outputFile.is_open())
	{
		// Start from the bottom of the world and loop up
		for (int z = 0; z < Globals::WORLD_HEIGHT; z++)
		{
			// Start from north and loop south
			for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++)
			{
				// Start from west and loop east
				for (int x = 0; x < Globals::WORLD_WIDTH_X; x++)
				{
					outputFile << static_cast<int>(t_world.getTileType(x, y, z)) << " ";

					sf::IntRect textRect = t_world.getTileTextureRect(x, y, z);

					outputFile << textRect.left << " " << textRect.top << " " << textRect.width << " " << textRect.height << ",";
				}

				outputFile << std::endl;
			}

			if (z != Globals::WORLD_HEIGHT - 1)
			{
				outputFile << std::endl;
			}
		}

		outputFile.close();

		std::cout << "Write to file successful" << std::endl;

		return true;
	}
	else
	{
		std::cout << "ERROR: Failed to open file" << std::endl;
	}

	return false;
}

bool const FileHandler::readWorldFromFile(World& t_world, std::string const& t_fileName)
{
	std::ifstream inputFile; // Create an input file stream

	inputFile.open("world_data/" + t_fileName + ".world");  // Open the file

	if (inputFile.is_open()) // Check the file is open
	{
		// Create a variable to store a line data
		std::string line;

		// 
		int z = 0;
		int y = 0;

		while (std::getline(inputFile, line))
		{
			if (line != "")
			{
				std::stringstream lineStream(line);
				std::string tile;
				int x = 0;

				while (std::getline(lineStream, tile, ','))
				{
					// Create the storage variables for the data
					std::stringstream tileStream(tile);
					std::string dataBit;

					// Get the first bit (tile type)
					std::getline(tileStream, dataBit, ' ');

					TileType type = static_cast<TileType>(std::stoi(dataBit));

					// Get the texture rect
					sf::IntRect textureRect;

					// Left
					std::getline(tileStream, dataBit, ' ');
					textureRect.left = std::stoi(dataBit);

					// Top
					std::getline(tileStream, dataBit, ' ');
					textureRect.top = std::stoi(dataBit);

					// Width
					std::getline(tileStream, dataBit, ' ');
					textureRect.width = std::stoi(dataBit);

					// Height
					std::getline(tileStream, dataBit, ' ');
					textureRect.height = std::stoi(dataBit);

					// Set the tile
					t_world.setTile(type, textureRect, x, y, z);

					// Increment the x value
					x++;
				}

				if (x < Globals::WORLD_WIDTH_X)
				{
					std::cout << "ERROR: Line read too short. X: " << x << std::endl;
				}

				y++;
			}
			else
			{
				if (y < Globals::WORLD_WIDTH_Y)
				{
					std::cout << "ERROR: Line read too short. Y: " << y << std::endl;
				}

				y = 0;
				z++;
			}
		}

		if (z < Globals::WORLD_HEIGHT)
		{
			std::cout << "ERROR: Line read too short. Z: " << z << std::endl;
		}

		inputFile.close();

		std::cout << "File read finished" << std::endl;
		return true;
	}
	else
	{
		std::cout << "ERROR: Failed to open file" << std::endl;
	}

	return false;
}

std::vector<std::string> const FileHandler::getSaveFileNames()
{
	std::vector<std::string> fileNames;
	std::string item;

	for (const stdfs::directory_entry & entry : stdfs::directory_iterator("world_data"))
	{
		item = entry.path().string();

		if (item.find(".world") != std::string::npos)
		{
			int startPos = item.find_last_of('\\') + 1;
			int length = item.find('.', startPos) - startPos;

			std::string name = item.substr(startPos, length);

			if (name != "")
			{
				fileNames.push_back(name);
			}

			//std::cout << item << " = ";
			//std::cout << name << std::endl;
		}
	}

	return fileNames;
}
