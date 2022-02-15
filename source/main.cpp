#include <iostream>
#include "calculator.h"

using namespace std;

int main(int argc, char **argv)
{
    unsigned int variableDictionarySize = 100;
    unsigned int functionDictionarySize = 20;
    char filename[FILENAME_MAX];
    if (argc == 2)
    {
        strncpy(filename, argv[1], FILENAME_MAX);
        CalculatorInit(variableDictionarySize, functionDictionarySize, filename);
    }
    else
    {
        CalculatorInit(variableDictionarySize, functionDictionarySize);
    }
    return 0;
}