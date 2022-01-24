#include "calculator.h"

BETNode *exprToAET(Expression *expr, int nestLevel)
{
    int priorOpIndex = prioritizedOperatorIndex(expr);

    if (priorOpIndex == PRIORITIZED_OPERATOR_NOT_FOUND)
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

    if (priorOpIndex == PRIORITIZED_OPERATOR_NOT_FOUND)
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

    BETNode *leftSubtree = exprToAET(slice(expr, 0, priorOpIndex), nestLevel);
    root->left = leftSubtree;

    BETNode *rightSubtree = exprToAET(slice(expr, priorOpIndex + 1, expr->length), nestLevel);
    root->right = rightSubtree;
    return root;
}

BETNode *exprToAET(Expression *expr)
{
    return exprToAET(expr, 0);
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

bool eval(char *str, VariableDictionary *dict, Number &result)
{
    Expression *e = strToExpr(str);
    // printf("strToExpr success: ");
    // print(e);
    // printf("\n");
    BETNode *root = exprToAET(e);
    // printf("exprToAET success\n");
    // prettyPrint(root);
    // print(root);
    bool evalSuccess = eval(root, dict, result);
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
    if (expr[0] == '\0')
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
            printf("f create\n");
            return EExpressionType::CREATE_FUNCTION;
        }
        else
        {
            printf("not f\n");
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
    char result[EXPR_MAX_LEN + 1];
    int resultLength = 0;
    for (unsigned int i = 0; i < strlen(expr) - 1; i++)
    {
        if (expr[i] != ' ')
        {
            result[resultLength] = expr[i];
            resultLength++;
        }
    }
    result[resultLength] = '\0';
    strncpy(expr, result, EXPR_MAX_LEN + 1);
}

void consoleModeStart(unsigned int dictionarySize)
{
    printf("\nsimpleCalculator version %d.%d\nType 'help' for help.\n", MAJOR_VERSION, MINOR_VERSION);
    VariableDictionary *dict = createVariableDictionary(dictionarySize);
    FunctionDictionary *functions = createFunctionDictionary(dictionarySize);
    char expr[EXPR_MAX_LEN + 1];
    bool running = true;
    while (running)
    {
        memset(expr, 0, EXPR_MAX_LEN + 1);
        printf(">>> ");
        fgets(expr, EXPR_MAX_LEN, stdin);

        deleteSpaces(expr);

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
            bool evalSuccess = eval(expr, dict, evaluationResult);
            if (evalSuccess)
            {
                setVariable("_", evaluationResult, dict);
                print(evaluationResult);
                printf("\n");
            }
            break;
        }

        case EExpressionType::EVALUATE_AND_ASSIGN:
        {
            char var[MAX_VARIABLE_NAME_LEN + 1];
            bool isCompound = hasCompoundAssignment(expr);
            char op;
            if (isCompound)
            {
                op = getCompoundOperator(expr);
                // printf("op: %c\n", op);
                char opDivider[2];
                opDivider[0] = op;
                opDivider[1] = ' ';
                strncpy(var, strtok(expr, opDivider), MAX_VARIABLE_NAME_LEN);
                // if (strchr(var, ' ') != nullptr) //strip trailing whitespace if exist
                //     var[var - strchr(var, ' ') - 1] = '\0';
                // strtok(NULL, "="); //skip the '='
            }
            else
            {
                strncpy(var, strtok(expr, " ="), MAX_VARIABLE_NAME_LEN);
            }

            if (not isCorrectVariableName(var))
            {
                printf("invalid variable name\n");
                break;
            }

            strncpy(expr, strtok(NULL, "="), EXPR_MAX_LEN);
            // printf("var: |%s|\nexpr: %s\n", var, expr);
            Number evaluationResult;
            bool evalSuccess = eval(expr, dict, evaluationResult);
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
                    setVariable("_", evaluationResult, dict);
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
            char *variablesList = strtok(NULL, ")=");
            char *functionBody = strtok(NULL, "=");
            // printf("b:");
            // print(strToExpr(functionBody));
            // printf("\n");
            // printf("name: |%s|\nargs: |%s|\nbody: |%s|\n", functionName, variablesList, functionBody);
            Function *func = createFunction(variablesList, functionBody);
            // printf("func entity created\n");
            addFunction(functionName, *func, functions);
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