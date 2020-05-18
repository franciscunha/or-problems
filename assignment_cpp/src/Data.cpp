#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nAgents;
	file >> nJobs;
	
    cost = std::vector<std::vector<int>>(nAgents, std::vector<int>(nJobs));
	for(int i = 0; i < nAgents; i++)
	{
		for(int j = 0; j < nJobs; j++)
		{
			file >> cost[i][j];
			if(file.bad() || file.fail()) throw DataException();
		}
	}
}
