#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);

	file >> nItems >> capacity;

	value = std::vector<int>(nItems, 0);
    weight = std::vector<int>(nItems, 0);
	for(int i = 0; i < nItems; i++)
	{
		file >> value[i] >> weight[i];
		if(file.bad() || file.fail()) throw DataException();
	}
}
