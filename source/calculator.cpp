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
    if (expr[0] == '!')
    {
        return EExpressionType::ECHO;
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
    case EExpressionType::ECHO:
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

EInputBehaviour getInput(SystemState *state)
{
    if (state->isFileModeOn)
    {
        state->lineNumber++;
        if (state->importRunning)
            state->running = fgets(state->expr, constants::EXPR_MAX_LEN, state->libFile) != nullptr;
        else
            state->running = fgets(state->expr, constants::EXPR_MAX_LEN, state->sourceFile) != nullptr;
        if (not state->running)
        {
            if (state->importRunning)
            {
                fclose(state->libFile);
                if (state->fileName == nullptr)
                {
                    state->isFileModeOn = false;
                }
                else
                {
                    state->isFileModeOn = true;
                    state->lineNumber = state->sourceLineSaved;
                }

                state->importRunning = false;
                state->running = true;
                printf("import finished\n");
                return EInputBehaviour::CONTINUE;
            }
            return EInputBehaviour::BREAK;
        }
    }
    else
    {
        state->outputEnabled = true;
        printf(">>> ");
        fgets(state->expr, constants::EXPR_MAX_LEN, stdin);
    }
    return EInputBehaviour::NORMAL;
}

SystemState *setup(unsigned int variableDictionarySize, unsigned int functionDictionarySize, char *fileName)
{
    SystemState *state = new SystemState;
    state->isFileModeOn = fileName != nullptr;
    state->fileName = fileName;
    if (state->isFileModeOn)
    {
        state->sourceFile = fopen(fileName, "r");
        if (state->sourceFile == nullptr)
        {
            state->isFileModeOn = false;
            printf("failed to open the file '%s'. file mode will not be enabled\n", fileName);
        }
    }
    state->fileName = fileName;
    state->varDict = createVariableDictionary(variableDictionarySize);
    state->funcDict = createFunctionDictionary(functionDictionarySize);
    state->lastResult = new char[2];
    state->lastResult[0] = '_';
    state->lastResult[1] = '\0';
    state->expr = new char[constants::EXPR_MAX_LEN + 1];
    state->running = true;
    state->importRunning = false;
    state->outputEnabled = false;
    state->lineNumber = 0;
    state->sourceLineSaved = 0;
    return state;
}

EInputBehaviour handlerEvaluate(SystemState *state)
{
    Number evaluationResult;
    bool evalSuccess = eval(state->expr, state->varDict, state->funcDict, evaluationResult);
    if (evalSuccess)
    {
        setVariable(state->lastResult, evaluationResult, state->varDict);
        if (state->outputEnabled)
        {
            print(evaluationResult);
            printf("\n");
            if (state->isFileModeOn)
                state->outputEnabled = false;
        }
    }
    return EInputBehaviour::NORMAL;
}

EInputBehaviour handlerEvaluateAndAssign(SystemState *state)
{
    char var[constants::MAX_VARIABLE_NAME_LEN + 1];
    bool isCompound = hasCompoundAssignment(state->expr);
    if (isCompound and state->importRunning)
    {
        // not allowed
        printf("compound operator not allowed in libraries. ('%s':%d)\n", state->libName, state->lineNumber);
        return EInputBehaviour::BREAK;
    }

    char op;
    if (isCompound)
    {
        op = getCompoundOperator(state->expr);
        char opDivider[2];
        opDivider[0] = op;
        opDivider[1] = ' ';
        strncpy(var, strtok(state->expr, opDivider), constants::MAX_VARIABLE_NAME_LEN);
    }
    else
    {
        strncpy(var, strtok(state->expr, " ="), constants::MAX_VARIABLE_NAME_LEN);
    }

    if (not isCorrectVariableName(var))
    {
        printf("invalid variable name\n");
        return EInputBehaviour::BREAK;
    }

    char *expr = strtok(nullptr, "=");
    if (expr != nullptr)
    {
        strncpy(state->expr, expr, constants::EXPR_MAX_LEN);
    }
    else
    {
        printf("expected expression after =\n");
        return EInputBehaviour::BREAK;
    }

    Number evaluationResult;
    bool evalSuccess = eval(state->expr, state->varDict, state->funcDict, evaluationResult);
    if (evalSuccess)
    {
        if (isCompound)
        {
            Number oldVariableValue;
            Number newVariableValue;
            getVariable(var, state->varDict, oldVariableValue);
            solve(oldVariableValue, evaluationResult, op, newVariableValue);
            setVariable(var, newVariableValue, state->varDict);
        }
        else
        {
            setVariable(state->lastResult, evaluationResult, state->varDict);
            if ((not state->importRunning) and state->outputEnabled)
            {
                print(evaluationResult);
                printf("\n");
                if (state->isFileModeOn)
                    state->outputEnabled = false;
            }
            setVariable(var, evaluationResult, state->varDict);
        }
    }
    return EInputBehaviour::NORMAL;
}

EInputBehaviour handlerCreateFunction(SystemState *state)
{
    char *functionDeclaration = state->expr + 4; // strip function declaration keyword
    char *functionName = strtok(functionDeclaration, "(");
    char *variablesList = strtok(nullptr, ")=");
    if (strncmp(variablesList, "null", 4) == 0)
    {
        variablesList = nullptr;
    }
    char *functionBody = strtok(nullptr, "=");
    Function *func = createFunction(variablesList, functionBody);
    addFunction(functionName, func, state->funcDict, ((not state->importRunning) and state->outputEnabled));
    if (state->isFileModeOn)
        state->outputEnabled = false;
    return EInputBehaviour::NORMAL;
}

EInputBehaviour handlerImport(SystemState *state)
{
    state->sourceLineSaved = state->lineNumber;
    state->lineNumber = 0;
    char lib[FILENAME_MAX + 1];
    if (strlen(state->expr) <= 6)
    {
        printf("expected library name after import\n");
        return EInputBehaviour::BREAK;
    }

    strncpy(lib, state->expr + 6, FILENAME_MAX);
    strncat(lib, ".splc", FILENAME_MAX);
    state->libName = lib;
    printf("importing: '%s'\n", state->expr + 6);
    state->importRunning = true;
    state->isFileModeOn = true;
    state->libFile = fopen(lib, "r");
    if (state->libFile == nullptr)
    {
        state->isFileModeOn = false;
        state->importRunning = false;
        printf("failed to open the lib '%s'\n", state->expr + 6);
    }
    return EInputBehaviour::NORMAL;
}

EInputBehaviour handlerEcho(SystemState *state)
{
    if (state->isFileModeOn)
        smartLineNumberPrint("!", state->lineNumber);
    printf("%s", state->expr + 1);
    state->outputEnabled = true;
    return EInputBehaviour::NORMAL;
}

void CalculatorInit(unsigned int variableDictionarySize, unsigned int functionDictionarySize, char *fileName)
{
    SystemState *state = setup(variableDictionarySize, functionDictionarySize, fileName);
    printf("simpleCalculator version %d.%d\nType 'help' for help.\n", constants::MAJOR_VERSION, constants::MINOR_VERSION);
    if (state->isFileModeOn)
    {
        printf("Started in file mode. Reading file '%s'\n", fileName);
    }
    while (state->running)
    {
        EInputBehaviour inputBehaviour = getInput(state);
        if (inputBehaviour == EInputBehaviour::BREAK)
            break;
        else if (inputBehaviour == EInputBehaviour::CONTINUE)
            continue;

        if (state->expr[0] != '!')
            deleteSpaces(state->expr);

        if (state->isFileModeOn and (not state->importRunning) and state->outputEnabled)
            smartLineNumberPrint(state->expr, state->lineNumber);

        switch (recognizeExpressionType(state->expr, state->importRunning))
        {
        case EExpressionType::DO_NOTHING:
        {
            // doing nothing
            break;
        }

        case EExpressionType::EXIT:
        {
            state->running = false;
            break;
        }

        case EExpressionType::HELP:
        {
            showHelp();
            break;
        }

        case EExpressionType::SHOW_VARIABLES:
        {
            print(state->varDict);
            break;
        }

        case EExpressionType::SAVE_VARIABLES_BIN:
        {
            saveDictionary(state->varDict, true);
            break;
        }

        case EExpressionType::SAVE_VARIABLES_TXT:
        {
            saveDictionary(state->varDict);
            break;
        }

        case EExpressionType::LOAD_VARIABLES:
        {
            loadDictionary(variableDictionarySize, state->varDict);
            break;
        }

        case EExpressionType::EVALUATE:
        {
            handlerEvaluate(state);
            break;
        }

        case EExpressionType::EVALUATE_AND_ASSIGN:
        {
            inputBehaviour = handlerEvaluateAndAssign(state);
            if (inputBehaviour == EInputBehaviour::BREAK)
                break;
            break;
        }

        case EExpressionType::CREATE_FUNCTION:
        {
            handlerCreateFunction(state);
            break;
        }

        case EExpressionType::SHOW_FUNCTIONS:
        {
            print(state->funcDict);
            break;
        }

        case EExpressionType::IMPORT:
        {
            inputBehaviour = handlerImport(state);
            if (inputBehaviour == EInputBehaviour::BREAK)
                break;
            break;
        }

        case EExpressionType::ECHO:
        {
            handlerEcho(state);
            break;
        }

        default:
        {
            // printf("expression recognition error\n");
            break;
        }
        }
    }
    printf("bye\n");
    delete state;
}