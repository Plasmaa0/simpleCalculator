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
        dict->keys[i] = new char[MAX_VARIABLE_NAME_LEN];
        memset(dict->keys[i], 0, MAX_VARIABLE_NAME_LEN);
        dict->values[i].type = EnumberType::NAN;
    }
    return dict;
}

void setVariable(char *variableName, Number number, VariableDictionary *dict)
{
    auto val = (number.type == EnumberType::INTEGER ? number.value.integer : number.value.decimal);
    bool alreadyExist = false;
    for (unsigned int i = 0; i < dict->freeIndex; i++)
    {
        if (strncmp(dict->keys[i], variableName, MAX_VARIABLE_NAME_LEN) == 0)
        {
            // printf("reset %s from %f to %f\n", variableName, dict->values[i], value);
            dict->values[i] = number;
            alreadyExist = true;
            break;
        }
    }
    if (dict->freeIndex < dict->size and (not alreadyExist))
    {
        // if (not alreadyExist)
        // {
        strncpy(dict->keys[dict->freeIndex], variableName, MAX_NUMBER_LENGTH);
        dict->values[dict->freeIndex] = number;
        dict->freeIndex++;
        // }
    }
    else if (not alreadyExist)
    {
        printf("dictionary overflow, can't add variable %s = %f\n", variableName, val);
    }
}

bool getVariable(char *variableName, VariableDictionary *dict, Number &num)
{
    for (unsigned int i = 0; i < dict->size; i++)
    {
        if (strncmp(dict->keys[i], variableName, MAX_VARIABLE_NAME_LEN) == 0)
        {
            num = dict->values[i];
            // printf("found %s=%d\n", variableName, value);
            return true;
        }
    }
    printf("VARIABLE %s NOT EXIST\n", variableName);
    return false;
}
