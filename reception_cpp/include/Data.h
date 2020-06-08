#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nTables;
    int nPeople;
    std::vector<int> capacity;
    std::vector<std::vector<int>> benefit;

public:
    int getBenefit(int i, int k) { return benefit[i][k]; }
    int getCapacity(int j) { return capacity[j]; }	
    int getNPeople() { return nPeople; }
    int getNTables() { return nTables; }

    Data(std::string filePath);
};