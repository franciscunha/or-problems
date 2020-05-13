#pragma once

#include <vector>
#include <string>

class Data
{
private:
    int capacity;
    int nItems;
    std::vector<int> value;
    std::vector<int> weight;

public:
    int getValue(int i) { return value[i]; }
    int getWeight(int i) { return weight[i]; }
    int getNItems() { return nItems; }
    int getCapacity() { return capacity; }

    Data(std::string filePath);
};