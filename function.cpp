#include "function.h"

Function *createFunction(char *paramsAsString, char *body)
{
    Function *result = new Function;
    // result->body = exprToAET(strToExpr(body));
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

        // printf("arg %d: %s\n", result->argsNumber, argName);
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
                printf("%s(%d)", dict->functions[i].argsNames[argI], strlen(dict->functions[i].argsNames[argI]));
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
            printf("function '%s' redefinded\n", funcName);
            break;
        }
    }
    if (dict->freeIndex < dict->size and (not alreadyExist))
    {
        // if (not alreadyExist)
        // {
        // printf("call strncpy func\n");
        strncpy(dict->names[dict->freeIndex], funcName, MAX_NUMBER_LENGTH);
        dict->functions[dict->freeIndex] = func;
        dict->freeIndex++;
        printf("new function '%s' added\n", funcName);
        // }
    }
    else if (not alreadyExist)
    {
        printf("dictionary overflow, can't add function %s\n", funcName);
    }
}

bool getFunction(char *funcName, FunctionDictionary *dict, Function &func)
{
    for (unsigned int i = 0; i < dict->size; i++)
    {
        if (strncmp(dict->names[i], funcName, MAX_VARIABLE_NAME_LEN) == 0)
        {
            // func = dict->functions[i];
            memcpy(&func, dict->functions + i, sizeof(Function));
            // printf("found %s=%d\n", variableName, value);
            return true;
        }
    }
    printf("FUNCTION %s NOT EXIST\n", funcName);
    return false;
}

bool evaluateFunction(Number *args, unsigned int argsN, Function *func, FunctionDictionary *fdict, Number &result)
{
    if (func->argsNumber != argsN)
    {
        printf("incompatible argument number\n");
        return false;
    }
    VariableDictionary *localVariables = createVariableDictionary(argsN + 1);
    for (unsigned int i = 0; i < argsN; i++)
    {
#ifdef DEBUF

        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>  %d  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", i);
        // printf("                                   BEFORE PRINT THE VARIABLE\n");
        // printf("local vdict and fdict:\n");
        // print(localVariables);
        // print(fdict);

        printf("%s on [%p] var set %s = ", func->asString, func, func->argsNames[i]);
        print(args[i]);
        printf("\n");

        // printf("                                   AFTER PRINT THE VARIABLE\n");
        // printf("local vdict and fdict:\n");
        // print(localVariables);
        // print(fdict);

        // printf("call set\n");
#endif
        setVariable(func->argsNames[i], args[i], localVariables);

        // printf("                                   AFTER SETTING THE VARIABLE\n");
        // printf("local vdict and fdict:\n");
        // print(localVariables);
        // print(fdict);
    }
#ifdef DEBUF
    printf("                         EVALUATE FUNCTION --->>> EVAL\n");
#endif
    Expression *functionAsExpression = strToExpr(func->asString);
    if (functionAsExpression == nullptr)
    {
#ifdef DEBUF
        printf("evaluateFunction error in strToExpr(func->asString)\n");
#endif
        return false;
    }
    else
    {
#ifdef DEBUF
        printf("evaluateFunction SUCCESS in strToExpr(func->asString)\n");
#endif
    }

    BETNode *functionAsBET = exprToAET(functionAsExpression);
    if (functionAsBET == nullptr)
    {
#ifdef DEBUF
        printf("evaluateFunction error in exprToAET(functionAsExpression)\n");
#endif
        return false;
    }
    else
    {
#ifdef DEBUF
            printf("evaluateFunction SUCCESS in exprToAET(functionAsExpression)\n");
#endif
    }

    bool success = eval(functionAsBET, localVariables, fdict, result);
    if (not success)
    {
#ifdef DEBUF
            printf("failure at eval(functionAsBET, localVariables, fdict, result)\n");
#endif
    }
    else
    {
#ifdef DEBUF
            printf("SUCCESS at eval(functionAsBET, localVariables, fdict, result)\n");
#endif
    }

    /**
     * 
     * CLEAR MEMORY FROM:
     *  - functionAsExpression
     *  - functionAsBET
     *  - localVariables
     * 
     */
    return success;
}