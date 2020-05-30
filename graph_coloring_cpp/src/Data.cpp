#include "../include/Data.h"
#include "../include/DataException.h"
#include <fstream>

Data::Data(std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) throw DataException();

	file >> nVertices;

	adjacency = std::vector<std::vector<int>>(nVertices, std::vector<int>(nVertices));
	for(int k = 0; k < nVertices; k++)
	{
		for(int j = 0; j < nVertices; j++)
		{
			file >> adjacency[k][j];
			if(file.bad() || file.fail()) throw DataException();		
		}
	}
}
