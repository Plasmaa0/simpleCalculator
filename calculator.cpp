#include "calculator.h"

BETNode *exprToBET(Expression *expr, int nestLevel)
{
    int priorOpIndex = prioritizedOperatorIndex(expr);

    if (priorOpIndex == constants::PRIORITIZED_OPERATOR_NOT_FOUND)
    {
        expr = strip(expr);
    }

    BETNode *root = createNode();
    if (expr->length == 1)
    {
        insert(root, &expr->symbols[0]);
        return root;
    }

    priorOpIndex = prioritizedOperatorIndex(expr);

    if (priorOpIndex == constants::PRIORITIZED_OPERATOR_NOT_FOUND)
    {
        return nullptr;
    }

    Symbol *priorSymbol = expr->symbols + priorOpIndex;
    insert(root, priorSymbol);

    // printf("prior op index: %d\n", priorOpIndex);
    // printf("prior op: ");
    // print(*priorSymbol);
    // printf("\n");
    // printf("expression: ");
    // print(expr);
    // printf("left subtree:");
    // print(strip(slice(expr, 0, priorOpIndex)));
    // printf("right subtree:");
    // print(strip(slice(expr, priorOpIndex + 1, expr->length)));
    // printf("\n");

    Expression *leftSubExpression = slice(expr, 0, priorOpIndex);
    BETNode *leftSubtree = exprToBET(leftSubExpression, nestLevel + 1);
    root->left = leftSubtree;

    Expression *rightSubExpression = slice(expr, priorOpIndex + 1, expr->length);
    BETNode *rightSubtree = exprToBET(rightSubExpression, nestLevel + 1);
    root->right = rightSubtree;
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
    // printf("######################################################\n");
    Expression *e = strToExpr(str);
    // printf("strToExpr success: ");
    // print(e);
    // printf("\n");
    BETNode *root = exprToBET(e);
    // printf("exprToBET success\n");
    // prettyPrint(root);
    // print(root);
    bool evalSuccess = eval(root, dict, fdict, result);
    // if (evalSuccess)
    //     printf("OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK OK \n");
    // else
    //     printf("FAILURE FAILURE FAILURE FAILURE FAILURE FAILURE FAILURE FAILURE\n");

    // printf("|%d|\n", (evalSuccess ? 111111 : 999999));
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

    return evalSuccess;
}

EExpressionType recognizeExpressionType(char *expr)
{
    if (expr[0] == '\0' or expr[0] == '#')
    {
        return EExpressionType::DO_NOTHING;
    }
    if (strcmp(expr, "exit") == 0)
    {
        return EExpressionType::EXIT;
    }
    if (strcmp(expr, "help") == 0)
    {
        return EExpressionType::HELP;
    }
    if (strcmp(expr, "vars") == 0)
    {
        return EExpressionType::SHOW_VARIABLES;
    }
    if (strcmp(expr, "funcs") == 0)
    {
        return EExpressionType::SHOW_FUNCTIONS;
    }
    if (strcmp(expr, "saveB") == 0)
    {
        return EExpressionType::SAVE_VARIABLES_BIN;
    }
    if (strcmp(expr, "save") == 0)
    {
        return EExpressionType::SAVE_VARIABLES_TXT;
    }
    if (strcmp(expr, "load") == 0)
    {
        return EExpressionType::LOAD_VARIABLES;
    }
    if (strchr(expr, '='))
    {
        if (strncmp(expr, "def:", 4) == 0)
        {
            // printf("f create\n");
            return EExpressionType::CREATE_FUNCTION;
        }
        else
        {
            // printf("not f\n");
            return EExpressionType::EVALUATE_AND_ASSIGN;
        }
    }
    else
    {
        return EExpressionType::EVALUATE;
    }

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

bool isCorrectVariableName(char *var)
{
    // printf("var: %s\n", var);
    if (recognizeSymbol(var[0]) == ESymbolType::NUMBER)
    {
        return false;
    }

    for (unsigned int i = 0; i < strlen(var); i++)
    {
        ESymbolType t = recognizeSymbol(var[i]);
        if (not(t == ESymbolType::NUMBER or t == ESymbolType::VARIABLE or var[i] == ' '))
        {
            return false;
        }
    }
    return true;
}

void deleteSpaces(char *expr)
{
    expr[strcspn(expr, "\n")] = '\0';
    char *result = new char[constants::EXPR_MAX_LEN + 1];
    int resultLength = 0;
    for (unsigned int i = 0; i < strlen(expr); i++)
    {
        if (expr[i] != ' ')
        {
            result[resultLength] = expr[i];
            resultLength++;
        }
    }
    result[resultLength] = '\0';
    strncpy(expr, result, constants::EXPR_MAX_LEN + 1);
}

void smartLineNumberPrint(char *expr, int lineNumber)
{
    switch (recognizeExpressionType(expr))
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

void CalculatorInit(unsigned int dictionarySize, char *filename)
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
        printf("filemode off\n");
    }

    printf("\nsimpleCalculator version %d.%d\nType 'help' for help.\n", constants::MAJOR_VERSION, constants::MINOR_VERSION);
    if (isFileModeOn)
    {
        printf("Started in file mode. Reading file '%s'\n", filename);
    }
    VariableDictionary *dict = createVariableDictionary(dictionarySize);
    FunctionDictionary *functions = createFunctionDictionary(dictionarySize);
    char *lastResult = new char[2];
    lastResult[0] = '_';
    lastResult[1] = '\0';
    char *expr = new char[constants::EXPR_MAX_LEN + 1];
    bool running = true;
    int lineNumber = 0;
    while (running)
    {
        lineNumber++;
        // memset(expr, 0, EXPR_MAX_LEN + 1);
        if (isFileModeOn)
        {
            // printf("reading..\n");
            running = fgets(expr, constants::EXPR_MAX_LEN, sourceFile) != nullptr;
            if (not running)
            {
                break;
            }

            // printf("read |%s|\n", expr);
            // fgetc(sourceFile);
        }
        else
        {
            printf(">>> ");
            fgets(expr, constants::EXPR_MAX_LEN, stdin);
        }

        deleteSpaces(expr);
        // printf("got |%s|\n", expr);
        if (isFileModeOn)
        {
            smartLineNumberPrint(expr, lineNumber);
        }

        switch (recognizeExpressionType(expr))
        {
        case EExpressionType::DO_NOTHING:
        {
            //doing nothing
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
            VariableDictionary *newDict = loadDictionary(dictionarySize);
            if (newDict != nullptr)
            {
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
                // std::cout << "res: ";
                print(evaluationResult);
                printf("\n");
            }
            break;
        }

        case EExpressionType::EVALUATE_AND_ASSIGN:
        {
            char var[constants::MAX_VARIABLE_NAME_LEN + 1];
            bool isCompound = hasCompoundAssignment(expr);
            char op;
            if (isCompound)
            {
                op = getCompoundOperator(expr);
                // printf("op: %c\n", op);
                char opDivider[2];
                opDivider[0] = op;
                opDivider[1] = ' ';
                strncpy(var, strtok(expr, opDivider), constants::MAX_VARIABLE_NAME_LEN);
                // if (strchr(var, ' ') != nullptr) //strip trailing whitespace if exist
                //     var[var - strchr(var, ' ') - 1] = '\0';
                // strtok(NULL, "="); //skip the '='
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
            // printf("var: |%s|\nexpr: %s\n", var, expr);
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
                    print(evaluationResult);
                    printf("\n");
                    setVariable(var, evaluationResult, dict);
                }
            }
            break;
        }

        case EExpressionType::CREATE_FUNCTION:
        {
            char *functionDeclaration = expr + 4; //strip function declaration keyword
            // printf("|%s|\n", functionDeclaration);
            char *functionName = strtok(functionDeclaration, "(");
            char *variablesList = strtok(nullptr, ")=");
            char *functionBody = strtok(nullptr, "=");
            // printf("b:");
            // print(strToExpr(functionBody));
            // printf("\n");
            // printf("name: |%s|\nargs: |%s|\nbody: |%s|\n", functionName, variablesList, functionBody);
            Function *func = createFunction(variablesList, functionBody);
            // printf("func entity created\n");
            addFunction(functionName, func, functions);
            delete functionDeclaration;
            delete functionName;
            delete functionBody;
            delete variablesList;
            break;
        }

        case EExpressionType::SHOW_FUNCTIONS:
        {
            print(functions);
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
}