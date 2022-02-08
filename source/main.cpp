#include <iostream>
#include "calculator.h"

using namespace std;

int main(int argc, char **argv)
{
    unsigned int variableDictionarySize = 100;
    unsigned int functionDictionarySize = 20;
    CalculatorInit(variableDictionarySize, functionDictionarySize);
    return 0;
}