#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nItems;
	file >> capacity;
	
    weight = std::vector<int>(nItems, 0);
	for(int i = 0; i < nItems; i++)
	{
		file >> weight[i];
		if(file.bad() || file.fail()) throw DataException();
	}
}
