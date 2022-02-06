#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string.h>

#include "constants.h"
#include "number.h"
#include "util.h"

typedef struct VariableDictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **keys;
    Number *values;
    ~VariableDictionary()
    {
        printf("deleting VariableDictionary<%d>\n", size);
        for (size_t i = 0; i < size; i++)
        {
            delete[] keys[i];
        }
        delete[] keys;
        delete[] values; //no need for destructor for Number
    }
} VariableDictionary;

//VARIABLE DICTIONARY

VariableDictionary *createVariableDictionary(unsigned int size);
void setVariable(char *variableName, Number number, VariableDictionary *dict);
bool getVariable(char *variableName, VariableDictionary *dict, Number &num);

#endif // __DICTIONARY_H__