#include <iostream>
#include "calculator.h"

using namespace std;

int main()
{
    char *a = new char;
    *a = '2';
    unsigned int dictionarySize = 30;
    consoleModeStart(dictionarySize);
    // Dictionary *dict = createVariableDictionary(3);
    // setVariable("var1", 10, dict);
    // setVariable("per", 5, dict);
    // print(dict);
    // // char str[] = "(34 * (-var1 - 4.5) + .95) * (67 - (5^2)/per)";
    // char str[EXPR_MAX_LEN] = "var1+1";
    // printf("EXPRESSION: %s\n", str);
    // double value = eval(str, dict);
    // printf("%s = %f\n", str, value);
    return 0;
}