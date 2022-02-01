#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <cstring>

struct FunctionDictionary;

#include "BET.h"
#include "number.h"
#include "expression.h"
#include "calculator.h"

typedef struct Function
{
    char *asString;
    unsigned int argsNumber;
    char **argsNames;
} Function;

typedef struct FunctionDictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **names;
    Function *functions;
} FunctionDictionary;

Function *createFunction(char *paramsAsString, char *body);
bool evaluateFunction(Number *args, unsigned int argsN, Function *func, FunctionDictionary *fdict, Number &result);

// typedef enum EArgType
// {
//     VARIABLE,
//     NUMBER,
//     FUNCTION_CALL
// }

// typedef struct Arg
// {

// } Arg;

// typedef struct functionCall
// {
//     int argsN;
//     Arg args[FUNCTION_MAX_ARGS_N];
//     char functionName[MAX_VARIABLE_NAME_LEN + 1];
// } functionCall;
//FUNCTION DICTIONARY

void print(FunctionDictionary *dict);
FunctionDictionary *createFunctionDictionary(unsigned int size);
void addFunction(char *funcName, Function func, FunctionDictionary *dict);
bool getFunction(char *funcName, FunctionDictionary *dict, Function &func);

#endif // __FUNCTION_H__