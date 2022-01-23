#include "function.h"

Function *createFunction(char *paramsAsString, char *body)
{
    Function *result = new Function;
    result->body = exprToAET(strToExpr(body));
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
