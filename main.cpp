#include <iostream>
#include "calculator.h"

using namespace std;

int main(int argc, char **argv)
{
    // printf("argc = %d\n", argc);
    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%d = %s\n", i, argv[i]);
    // }
    unsigned int dictionarySize = 30;
    char filename[] = "input.splc";
    CalculatorInit(dictionarySize, filename);

    // if (argc == 2)
    // {
    //     char *filename = new char[FILENAME_MAX];
    //     strcpy(filename, argv[1]);
    //     deleteSpaces(filename);
    //     printf("file: |%s|\n", filename);
    //     CalculatorInit(dictionarySize, filename);
    // }
    // else
    // {
    // CalculatorInit(dictionarySize);
    // }

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