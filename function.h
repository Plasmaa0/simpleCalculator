#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <string.h>

#include "BET.h"
#include "expression.h"
#include "calculator.h"

typedef struct Function
{
    int argsNumber;
    char **argsNames;
    BETNode *body;
} Function;

Function *createFunction(char *paramsAsString, char *body);

#endif // __FUNCTION_H__