#include <iostream>
#include "calculator.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "program start" << endl;
    unsigned int dictionarySize = 30;
    CalculatorInit(dictionarySize);
    return 0;
}