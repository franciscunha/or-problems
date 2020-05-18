#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nAgents;
    int nJobs;
    std::vector<std::vector<int>> cost;

public:
    int getCost(int i, int j) { return cost[i][j]; }
    int getNAgents() { return nAgents; }
    int getNJobs() { return nJobs; }
    
    Data(std::string filePath);
};