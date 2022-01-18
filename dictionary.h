#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <string.h>

#include "constants.h"

typedef struct Dictionary
{
    unsigned int size;
    unsigned int freeIndex;
    char **keys;
    double *values;
} Dictionary;

//DICTIONARY

Dictionary *createDictionary(unsigned int size);
void setVariable(char *variableName, double value, Dictionary *dict);
double getVariable(char *variableName, Dictionary *dict);

#endif // __DICTIONARY_H__