#include <iostream>
#include <string> //getline, string.find
#include "hrml.h"

int main()
{
    std::string rawData;

    rawData = readFile("./test.hrml");
    createDataStruct(rawData);
    std::cin.get();
}