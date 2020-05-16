#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nKnapsacks >> nItems;

	value = std::vector<int>(nItems, 0);
    weight = std::vector<int>(nItems, 0);
    capacity = std::vector<int>(nKnapsacks, 0);

	for(int j = 0; j < nKnapsacks; j++)
	{
		file >> capacity[j];
		if(file.bad() || file.fail()) throw DataException();
	}

	for(int i = 0; i < nItems; i++)
	{
		file >> weight[i];
		if(file.bad() || file.fail()) throw DataException();
	}

	for(int i = 0; i < nItems; i++)
	{
		file >> value[i];
		if(file.bad() || file.fail()) throw DataException();
	}
}
