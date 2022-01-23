#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string.h>

#include "constants.h"
#include "number.h"

typedef struct VariableDictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **keys;
    Number *values;
} VariableDictionary;

//VARIABLE DICTIONARY

VariableDictionary *createVariableDictionary(unsigned int size);
void setVariable(char *variableName, Number number, VariableDictionary *dict);
bool getVariable(char *variableName, VariableDictionary *dict, Number &num);

#endif // __DICTIONARY_H__