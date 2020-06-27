#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nOrigins;
	file >> nDestinations;

	offer = std::vector<int>(nOrigins, 0);
	for(int i = 0; i < nOrigins; i++)
	{
		file >> offer[i];
		if(file.bad() || file.fail()) throw DataException();
	}
	
	demand = std::vector<int>(nDestinations, 0);
	for(int j = 0; j < nDestinations; j++)
	{
		file >> demand[j];
		if(file.bad() || file.fail()) throw DataException();
	}

    cost = std::vector<std::vector<int>>(nOrigins, std::vector<int>(nDestinations));
	for(int i = 0; i < nOrigins; i++)
	{
		for(int j = 0; j < nDestinations; j++)
		{
			file >> cost[i][j];
			if(file.bad() || file.fail()) throw DataException();
		}
	}
}
