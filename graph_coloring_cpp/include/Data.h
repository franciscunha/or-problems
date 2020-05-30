#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nVertices;
    std::vector<std::vector<int>> adjacency;

public:
    int isAdjacent(int k, int j) { return adjacency[k][j]; }
    int getNVertices() { return nVertices; }	
    int getMaxColors() { return nVertices; }
    
    Data(std::string filePath);
};