#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nOrigins;
    int nDestinations;
    std::vector<int> offer;
    std::vector<int> demand;
    std::vector<std::vector<int>> cost;

public:
    int getCost(int i, int j) { return cost[i][j]; }
    int getOffer(int i) { return offer[i]; }
    int getDemand(int j) { return demand[j]; }
    int getNOrigins() { return nOrigins; }
    int getNDestinations() { return nDestinations; }
    
    Data(std::string filePath);
};