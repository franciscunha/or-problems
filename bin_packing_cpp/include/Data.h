#pragma once

#include <vector>
#include <string>
#include "../include/DataException.h"

class Data
{
private:
    int nItems;
    int capacity;
    std::vector<int> weight;

public:
    int getWeight(int i) { return weight[i]; }
    int getCapacity() { return capacity; }	
    int getNItems() { return nItems; }
    
    Data(std::string filePath);
};