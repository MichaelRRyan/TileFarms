#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "World.h"

namespace stdfs = std::filesystem;

class FileHandler
{
public:

	static bool const saveWorldToFile(World const & t_world, std::string const& t_fileName);

	static bool const readWorldFromFile(World & t_world, std::string const& t_fileName);

	static std::vector<std::string> const getSaveFileNames();

};

#endif // !FILE_HANDLER_H