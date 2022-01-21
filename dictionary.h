#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string.h>

#include "constants.h"
#include "number.h"

typedef struct Dictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **keys;
    Number *values;
} Dictionary;

//DICTIONARY

Dictionary *createDictionary(unsigned int size);
void setVariable(char *variableName, Number number, Dictionary *dict);
bool getVariable(char *variableName, Dictionary *dict, Number &num);

#endif // __DICTIONARY_H__