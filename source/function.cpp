#include "function.h"

Function *createFunction(char *paramsAsString, char *body)
{
    Function *result = new Function;
    result->asString = new char[constants::EXPR_MAX_LEN + 1];
    strncpy(result->asString, body, constants::EXPR_MAX_LEN + 1);
    result->argsNames = new char *[constants::FUNCTION_MAX_ARGS_N];
    for (int i = 0; i < constants::FUNCTION_MAX_ARGS_N; i++)
    {
        result->argsNames[i] = new char[constants::MAX_VARIABLE_NAME_LEN + 1];
    }

    result->argsNumber = 0;
    char *argName = strtok(paramsAsString, ",");
    while (argName != nullptr)
    {
        if (not isCorrectVariableName(argName))
        {
            printf("%s is not correct arg name\n", argName);
            delete result;
            return nullptr;
        }
        strncpy(result->argsNames[result->argsNumber], argName, constants::MAX_VARIABLE_NAME_LEN);
        result->argsNumber++;
        argName = strtok(nullptr, ",");
    }
    return result;
}

void print(FunctionDictionary *dict)
{
    printf("FUNCTIONS:\n");
    if (dict->freeIndex > 0)
    {
        for (unsigned int i = 0; i < dict->freeIndex; i++)
        {
            printf("   %s(", dict->names[i]);
            for (unsigned int argI = 0; argI < dict->functions[i].argsNumber; argI++)
            {
                printf("%s", dict->functions[i].argsNames[argI]);
                if (argI != dict->functions[i].argsNumber - 1)
                {
                    printf(", ");
                }
            }
            printf(") = %s\n", dict->functions[i].asString);
        }
        printf("END\n");
    }
    else
    {
        printf("NONE\n");
    }
}

FunctionDictionary *createFunctionDictionary(unsigned int size)
{
    FunctionDictionary *dict = new FunctionDictionary;
    dict->size = size;
    dict->freeIndex = 0;
    dict->functions = new Function[size];
    for (size_t i = 0; i < size; i++)
    {
        dict->functions[i].asString = nullptr;
        dict->functions[i].argsNames = nullptr;
    }

    dict->names = new char *[size];
    for (unsigned int i = 0; i < size; i++)
    {
        dict->names[i] = new char[constants::MAX_VARIABLE_NAME_LEN];
        memset(dict->names[i], 0, constants::MAX_VARIABLE_NAME_LEN);
    }
    return dict;
}

void addFunction(char *funcName, Function *func, FunctionDictionary *dict, bool displayMsg)
{
    bool alreadyExist = false;
    for (unsigned int i = 0; i < dict->freeIndex; i++)
    {
        if (strncmp(dict->names[i], funcName, constants::MAX_VARIABLE_NAME_LEN) == 0)
        {
            dict->functions[i] = *func;
            alreadyExist = true;
            if (displayMsg)
                printf("function '%s' redefinded\n", funcName);
            break;
        }
    }
    if (dict->freeIndex < dict->size and (not alreadyExist))
    {
        strncpy(dict->names[dict->freeIndex], funcName, constants::MAX_VARIABLE_NAME_LEN);
        dict->functions[dict->freeIndex] = *func;
        dict->freeIndex++;
        if (displayMsg)
            printf("new function '%s' added\n", funcName);
    }
    else if (not alreadyExist)
    {
        if (displayMsg)
            printf("dictionary overflow, can't add function '%s'\n", funcName);
    }
}

bool getFunction(char *funcName, FunctionDictionary *dict, Function **func)
{
    for (unsigned int i = 0; i < dict->size; i++)
    {
        if (strncmp(dict->names[i], funcName, constants::MAX_VARIABLE_NAME_LEN) == 0)
        {
            *func = dict->functions + i;
            return true;
        }
    }
    printf("FUNCTION %s NOT EXIST\n", funcName);
    return false;
}

bool evaluateFunction(Number *args, unsigned int argsN, Function *func, VariableDictionary *globals, FunctionDictionary *fdict, Number &result)
{
    if (func->argsNumber != argsN)
    {
        printf("incompatible argument number\n");
        return false;
    }
    VariableDictionary *localVariables = createVariableDictionary(argsN + globals->freeIndex);

    // globals first
    for (unsigned int i = 0; i < globals->freeIndex; i++)
    {
        setVariable(globals->keys[i], globals->values[i], localVariables);
    }

    // locals second (to override globals if there are variablese with repeating names)
    // so the local variables will be prioritized
    for (unsigned int i = 0; i < argsN; i++)
    {
        setVariable(func->argsNames[i], args[i], localVariables);
    }

    bool success = eval(func->asString, localVariables, fdict, result);
    delete localVariables;
    return success;
}