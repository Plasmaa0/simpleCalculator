#include "function.h"

Function *createFunction(char *paramsAsString, char *body)
{
    Function *result = new Function;
    result->body = exprToAET(strToExpr(body));
    result->asString = new char[EXPR_MAX_LEN + 1];
    strncpy(result->asString, body, EXPR_MAX_LEN + 1);
    result->argsNames = new char *[FUNCTION_MAX_ARGS_N];
    for (int i = 0; i < FUNCTION_MAX_ARGS_N; i++)
    {
        result->argsNames[i] = new char[MAX_VARIABLE_NAME_LEN + 1];
    }

    result->argsNumber = 0;
    char *argName = strtok(paramsAsString, ",");
    while (argName != NULL)
    {
        if (not isCorrectVariableName(argName))
        {
            printf("%s is not correct arg name\n", argName);
            return nullptr;
        }

        printf("arg %d: %s\n", result->argsNumber, argName);
        strncpy(result->argsNames[result->argsNumber], argName, MAX_VARIABLE_NAME_LEN);
        result->argsNumber++;
        argName = strtok(NULL, ",");
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
    dict->names = new char *[size];
    for (unsigned int i = 0; i < size; i++)
    {
        dict->names[i] = new char[MAX_VARIABLE_NAME_LEN];
        memset(dict->names[i], 0, MAX_VARIABLE_NAME_LEN);
    }
    return dict;
}

void addFunction(char *funcName, Function func, FunctionDictionary *dict)
{
    bool alreadyExist = false;
    for (unsigned int i = 0; i < dict->freeIndex; i++)
    {
        if (strncmp(dict->names[i], funcName, MAX_VARIABLE_NAME_LEN) == 0)
        {
            // printf("reset %s from %f to %f\n", variableName, dict->values[i], value);
            dict->functions[i] = func;
            alreadyExist = true;
            printf("func redefinded\n");
            break;
        }
    }
    if (dict->freeIndex < dict->size and (not alreadyExist))
    {
        // if (not alreadyExist)
        // {
        strncpy(dict->names[dict->freeIndex], funcName, MAX_NUMBER_LENGTH);
        dict->functions[dict->freeIndex] = func;
        dict->freeIndex++;
        printf("new func added\n");
        // }
    }
    else if (not alreadyExist)
    {
        printf("dictionary overflow, can't add variable %s\n", funcName);
    }
}

bool getFunction(char *funcName, FunctionDictionary *dict, Function &func)
{
    for (unsigned int i = 0; i < dict->size; i++)
    {
        if (strncmp(dict->names[i], funcName, MAX_VARIABLE_NAME_LEN) == 0)
        {
            func = dict->functions[i];
            // printf("found %s=%d\n", variableName, value);
            return true;
        }
    }
    printf("FUNCTION %s NOT EXIST\n", funcName);
    return false;
}

bool evaluateFunction(Number *args, unsigned int argsN, Function *func, Number &result)
{
    if (func->argsNumber != argsN)
    {
        printf("incompatible argument number\n");
        return false;
    }
    VariableDictionary *localVariables = createVariableDictionary(argsN);
    for (unsigned int i = 0; i < argsN; i++)
    {
        setVariable(func->argsNames[i], args[i], localVariables);
    }
    return eval(func->body, localVariables, result);
}