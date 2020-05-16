#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nKnapsacks;
    int nItems;
    std::vector<int> capacity;
    std::vector<int> value;
    std::vector<int> weight;

public:
    int getValue(int i) { return value[i]; }
    int getWeight(int i) { return weight[i]; }
    int getCapacity(int i) { return capacity[i]; }	
    int getNItems() { return nItems; }
    int getNKnapsacks() { return nKnapsacks; }

    Data(std::string filePath);
};