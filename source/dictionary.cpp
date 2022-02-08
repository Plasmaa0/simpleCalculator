#include "dictionary.h"

VariableDictionary *createVariableDictionary(unsigned int size)
{
    VariableDictionary *dict = new VariableDictionary;
    dict->size = size;
    dict->freeIndex = 0;
    dict->values = new Number[size];
    dict->keys = new char *[size];
    for (unsigned int i = 0; i < size; i++)
    {
        dict->keys[i] = new char[constants::MAX_VARIABLE_NAME_LEN + 1];
        dict->values[i].type = EnumberType::NAN;
    }
    return dict;
}

void setVariable(char *variableName, Number number, VariableDictionary *dict)
{
    if ((not isCorrectVariableName(variableName)) and strcmp(variableName, "_") != 0)
    {
        printf("'%s' is invalid variable name\n", variableName);
        return;
    }
    bool alreadyExist = false;
    for (unsigned int i = 0; i < dict->freeIndex; i++)
    {
        if (strncmp(dict->keys[i], variableName, constants::MAX_VARIABLE_NAME_LEN) == 0)
        {
            dict->values[i] = number;
            alreadyExist = true;
            break;
        }
    }
    if (dict->freeIndex < dict->size and (not alreadyExist))
    {
        strncpy(dict->keys[dict->freeIndex], variableName, constants::MAX_VARIABLE_NAME_LEN);
        dict->values[dict->freeIndex] = number;
        dict->freeIndex++;
    }
    else if (not alreadyExist)
    {
        printf("dictionary overflow, can't add variable %s = ", variableName);
        print(number, true);
        printf("\n");
    }
}

bool getVariable(char *variableName, VariableDictionary *dict, Number &num)
{
    for (unsigned int i = 0; i < dict->size; i++)
    {
        if (strncmp(dict->keys[i], variableName, constants::MAX_VARIABLE_NAME_LEN) == 0)
        {
            num = dict->values[i];
            return true;
        }
    }
    printf("VARIABLE %s NOT EXIST\n", variableName);
    return false;
}
