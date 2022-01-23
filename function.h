#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <iostream>
#include <string.h>

#include "BET.h"
#include "number.h"
#include "expression.h"
#include "calculator.h"

typedef struct Function
{
    char *asString;
    unsigned int argsNumber;
    char **argsNames;
    BETNode *body;
} Function;

Function *createFunction(char *paramsAsString, char *body);
bool evaluateFunction(Number *args, unsigned int argsN, Function *func, Number &result);

typedef struct FunctionDictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **names;
    Function *functions;
} FunctionDictionary;

//FUNCTION DICTIONARY

void print(FunctionDictionary *dict);
FunctionDictionary *createFunctionDictionary(unsigned int size);
void addFunction(char *funcName, Function func, FunctionDictionary *dict);
bool getFunction(char *funcName, FunctionDictionary *dict, Function &func);

#endif // __FUNCTION_H__