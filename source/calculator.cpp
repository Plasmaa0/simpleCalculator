#include "calculator.h"

BETNode *exprToBET(Expression *input, int nestLevel)
{
    bool wasStripped = false;
    int priorOpIndex = prioritizedOperatorIndex(input);

    Expression *expr = input;
    if (priorOpIndex == constants::PRIORITIZED_OPERATOR_NOT_FOUND)
    {
        expr = strip(input);
        wasStripped = true;
    }

    BETNode *root = createNode();
    if (expr->length == 1)
    {
        insert(root, expr->symbols + 0);
        delete expr;
        return root;
    }

    priorOpIndex = prioritizedOperatorIndex(expr);

    if (priorOpIndex == constants::PRIORITIZED_OPERATOR_NOT_FOUND)
    {
        // destroyBET(root);
        if (nestLevel > 0)
            delete expr;
        return nullptr;
    }

    Symbol *priorSymbol = expr->symbols + priorOpIndex;
    insert(root, priorSymbol);

    Expression *leftSubExpression = slice(expr, 0, priorOpIndex);
    BETNode *leftSubtree = exprToBET(leftSubExpression, nestLevel + 1);
    root->left = leftSubtree;
    delete leftSubExpression;

    Expression *rightSubExpression = slice(expr, priorOpIndex + 1, expr->length);
    BETNode *rightSubtree = exprToBET(rightSubExpression, nestLevel + 1);
    root->right = rightSubtree;
    delete rightSubExpression;
    if (wasStripped)
        delete expr;
    return root;
}

BETNode *exprToBET(Expression *expr)
{
    BETNode *result = exprToBET(expr, 0);
    return result;
}

void showHelp()
{
    printf("HELP:\n\
    vars - show variables\n\
    funcs - show functions\n\
    save - save variables in text file\n\
    saveB - save variables in binary file (for loading them in future)\n\
    load - load variables from binary file\n\
    define a function: 'def: name(arg1, arg2, ...) = expression'\n\
    exit - close program\n");
}

bool eval(char *str, VariableDictionary *dict, FunctionDictionary *fdict, Number &result)
{
    Expression *asExpression = strToExpr(str);
    // printf("strToExpr success: ");
    // print(asExpression);
    // printf("\n");
    BETNode *root = exprToBET(asExpression);
    // printf("exprToBET success\n");
    // prettyPrint(root);
    // print(root);
    bool evalSuccess = eval(root, dict, fdict, result);

    // printf("eval success\n");
    // printf("pre-order: ");
    // pre_order(root);
    // printf("\n");
    // printf("in-order: ");
    // in_order(root);
    // printf("\n");
    // printf("post-order: ");
    // post_order(root);
    // printf("\n");

    for (int i = 0; i < asExpression->length; i++)
    {
        if (asExpression->symbols[i].type == ESymbolType::FUNCTION_CALL)
        {
            delete asExpression->symbols[i].entity.functionCall;
        }
    }

    delete asExpression;
    destroyBET(root);
    return evalSuccess;
}

EExpressionType recognizeExpressionType(char *expr, bool importRunning)
{
    if (expr[0] == '\0' or expr[0] == '#')
    {
        return EExpressionType::DO_NOTHING;
    }
    if (strncmp(expr, "exit", 4) == 0 and (not importRunning))
    {
        return EExpressionType::EXIT;
    }
    if (strncmp(expr, "help", 4) == 0 and (not importRunning))
    {
        return EExpressionType::HELP;
    }
    if (strncmp(expr, "vars", 4) == 0 and (not importRunning))
    {
        return EExpressionType::SHOW_VARIABLES;
    }
    if (strncmp(expr, "funcs", 5) == 0 and (not importRunning))
    {
        return EExpressionType::SHOW_FUNCTIONS;
    }
    if (strncmp(expr, "saveB", 5) == 0 and (not importRunning))
    {
        return EExpressionType::SAVE_VARIABLES_BIN;
    }
    if (strncmp(expr, "save", 4) == 0 and (not importRunning))
    {
        return EExpressionType::SAVE_VARIABLES_TXT;
    }
    if (strncmp(expr, "load", 4) == 0 and (not importRunning))
    {
        return EExpressionType::LOAD_VARIABLES;
    }
    if (strncmp(expr, "import", 6) == 0)
    {
        return EExpressionType::IMPORT;
    }
    if (strchr(expr, '='))
    {
        if (strncmp(expr, "def", 3) == 0)
        {
            return EExpressionType::CREATE_FUNCTION;
        }
        else
        {
            return EExpressionType::EVALUATE_AND_ASSIGN;
        }
    }
    else if (not importRunning)
    {
        return EExpressionType::EVALUATE;
    }
    if (not importRunning)
        printf("unrecognized expression\n");
    return EExpressionType::DO_NOTHING;
}

int equalsSignIndex(char *expr)
{
    return strchr(expr, '=') - expr;
}

bool hasCompoundAssignment(char *expr)
{
    return recognizeSymbol(expr[equalsSignIndex(expr) - 1]) == ESymbolType::OPERATOR;
}

char getCompoundOperator(char *expr)
{
    return expr[equalsSignIndex(expr) - 1];
}

void deleteSpaces(char *expr)
{
    expr[strcspn(expr, "\n")] = '\0';
    char result[constants::EXPR_MAX_LEN + 1];
    int resultLength = 0;
    for (unsigned int i = 0; i < strlen(expr); i++)
    {
        if (expr[i] != ' ')
        {
            result[resultLength] = expr[i];
            resultLength++;
        }
    }
    if (resultLength == (int)strlen(expr))
    {
        return;
    }
    result[resultLength] = '\0';
    strncpy(expr, result, constants::EXPR_MAX_LEN + 1);
}

void smartLineNumberPrint(char *expr, int lineNumber)
{
    switch (recognizeExpressionType(expr, false))
    {
    case EExpressionType::HELP:
    case EExpressionType::SHOW_VARIABLES:
    case EExpressionType::SAVE_VARIABLES_BIN:
    case EExpressionType::SAVE_VARIABLES_TXT:
    case EExpressionType::EVALUATE:
    case EExpressionType::EVALUATE_AND_ASSIGN:
    case EExpressionType::LOAD_VARIABLES:
    case EExpressionType::CREATE_FUNCTION:
    case EExpressionType::SHOW_FUNCTIONS:
    {
        printf("[%d]: ", lineNumber);
        break;
    }
    case EExpressionType::EXIT:
        break;
    case EExpressionType::DO_NOTHING:
        break;
    default:
    {
        break;
    }
    }
}

void CalculatorInit(unsigned int variableDictionarySize, unsigned int functionDictionarySize, char *filename)
{
    bool isFileModeOn = filename != nullptr;
    FILE *sourceFile;
    if (isFileModeOn)
    {
        sourceFile = fopen(filename, "r");
        if (sourceFile == nullptr)
        {
            isFileModeOn = false;
            printf("failed to open the file '%s'. file mode will not be enabled\n", filename);
        }
    }
    else
    {
        // printf("filemode off\n");
    }

    printf("simpleCalculator version %d.%d\nType 'help' for help.\n", constants::MAJOR_VERSION, constants::MINOR_VERSION);
    if (isFileModeOn)
    {
        printf("Started in file mode. Reading file '%s'\n", filename);
    }
    VariableDictionary *dict = createVariableDictionary(variableDictionarySize);
    FunctionDictionary *functions = createFunctionDictionary(functionDictionarySize);
    char *lastResult = new char[2];
    lastResult[0] = '_';
    lastResult[1] = '\0';
    char *expr = new char[constants::EXPR_MAX_LEN + 1];
    bool running = true;
    bool importRunning = false;
    int lineNumber = 0;
    while (running)
    {
        if (isFileModeOn)
        {
            lineNumber++;
            running = fgets(expr, constants::EXPR_MAX_LEN, sourceFile) != nullptr;
            if (not running)
            {
                if (importRunning)
                {
                    isFileModeOn = false;
                    importRunning = false;
                    running = true;
                    printf("import finished\n");
                    fclose(sourceFile);
                    continue;
                }
                break;
            }
        }
        else
        {
            printf(">>> ");
            fgets(expr, constants::EXPR_MAX_LEN, stdin);
        }

        deleteSpaces(expr);
        if (isFileModeOn and (not importRunning))
        {
            smartLineNumberPrint(expr, lineNumber);
        }

        switch (recognizeExpressionType(expr, importRunning))
        {
        case EExpressionType::DO_NOTHING:
        {
            // doing nothing
            break;
        }

        case EExpressionType::EXIT:
        {
            running = false;
            break;
        }

        case EExpressionType::HELP:
        {
            showHelp();
            break;
        }

        case EExpressionType::SHOW_VARIABLES:
        {
            print(dict);
            break;
        }

        case EExpressionType::SAVE_VARIABLES_BIN:
        {
            saveDictionary(dict, true);
            break;
        }

        case EExpressionType::SAVE_VARIABLES_TXT:
        {
            saveDictionary(dict);
            break;
        }

        case EExpressionType::LOAD_VARIABLES:
        {
            VariableDictionary *newDict = loadDictionary(variableDictionarySize);
            if (newDict != nullptr)
            {
                delete dict;
                dict = newDict;
            }
            break;
        }

        case EExpressionType::EVALUATE:
        {
            Number evaluationResult;
            bool evalSuccess = eval(expr, dict, functions, evaluationResult);
            if (evalSuccess)
            {
                setVariable(lastResult, evaluationResult, dict);
                print(evaluationResult);
                printf("\n");
            }
            break;
        }

        case EExpressionType::EVALUATE_AND_ASSIGN:
        {
            char var[constants::MAX_VARIABLE_NAME_LEN + 1];
            bool isCompound = hasCompoundAssignment(expr);
            if (isCompound and importRunning)
            {
                break;
            }

            char op;
            if (isCompound)
            {
                op = getCompoundOperator(expr);
                char opDivider[2];
                opDivider[0] = op;
                opDivider[1] = ' ';
                strncpy(var, strtok(expr, opDivider), constants::MAX_VARIABLE_NAME_LEN);
            }
            else
            {
                strncpy(var, strtok(expr, " ="), constants::MAX_VARIABLE_NAME_LEN);
            }

            if (not isCorrectVariableName(var))
            {
                printf("invalid variable name\n");
                break;
            }

            strncpy(expr, strtok(nullptr, "="), constants::EXPR_MAX_LEN);
            Number evaluationResult;
            bool evalSuccess = eval(expr, dict, functions, evaluationResult);
            if (evalSuccess)
            {
                if (isCompound)
                {
                    Number oldVariableValue;
                    Number newVariableValue;
                    getVariable(var, dict, oldVariableValue);
                    solve(oldVariableValue, evaluationResult, op, newVariableValue);
                    setVariable(var, newVariableValue, dict);
                }
                else
                {
                    setVariable(lastResult, evaluationResult, dict);
                    if (not importRunning)
                    {
                        print(evaluationResult);
                        printf("\n");
                    }
                    setVariable(var, evaluationResult, dict);
                }
            }
            break;
        }

        case EExpressionType::CREATE_FUNCTION:
        {
            char *functionDeclaration = expr + 4; // strip function declaration keyword
            char *functionName = strtok(functionDeclaration, "(");
            char *variablesList = strtok(nullptr, ")=");
            if (strncmp(variablesList, "null", 4) == 0)
            {
                variablesList = nullptr;
            }
            char *functionBody = strtok(nullptr, "=");
            Function *func = createFunction(variablesList, functionBody);
            addFunction(functionName, func, functions, not importRunning);
            break;
        }

        case EExpressionType::SHOW_FUNCTIONS:
        {
            print(functions);
            break;
        }

        case EExpressionType::IMPORT:
        {
            char lib[FILENAME_MAX + 1];
            if (strlen(expr) <= 6)
            {
                printf("expected library name after import\n");
                break;
            }

            strncpy(lib, expr + 6, FILENAME_MAX);
            strncat(lib, ".splc", FILENAME_MAX);
            printf("importing: '%s'\n", expr + 6);
            importRunning = true;
            isFileModeOn = true;
            sourceFile = fopen(lib, "r");
            if (sourceFile == nullptr)
            {
                isFileModeOn = false;
                importRunning = false;
                printf("failed to open the lib '%s'\n", expr + 6);
            }
            break;
        }

        default:
        {
            printf("expression recognition error\n");
            break;
        }
        }
    }
    printf("bye\n");
    delete[] lastResult;
    delete[] expr;
    delete dict;
    delete functions;
}