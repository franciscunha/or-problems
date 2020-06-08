#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nTables >> nPeople;

	benefit = std::vector<std::vector<int>>(nPeople, std::vector<int>(nPeople, 0));
    capacity = std::vector<int>(nTables, 0);

	for(int j = 0; j < nTables; j++)
	{
		file >> capacity[j];
		if(file.bad() || file.fail()) throw DataException();
	}

	for(int i = 0; i < nPeople; i++)
	{
		for(int k = 0; k < nPeople; k++)
		{
			file >> benefit[i][k];
			if(file.bad() || file.fail()) throw DataException();
		}
	}
}
