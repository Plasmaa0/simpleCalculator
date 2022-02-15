#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#ifndef FILENAME_MAX
#define FILENAME_MAX (260)
#endif

#include "function.h"
#include "io.h"
#include "expression.h"
#include "dictionary.h"
#include "BET.h"
#include "number.h"
#include "util.h"

typedef enum EExpressionType
{
    EXIT,
    HELP,
    DO_NOTHING,
    SHOW_VARIABLES,
    SAVE_VARIABLES_TXT,
    SAVE_VARIABLES_BIN,
    LOAD_VARIABLES,
    EVALUATE,
    EVALUATE_AND_ASSIGN,
    CREATE_FUNCTION,
    SHOW_FUNCTIONS,
    IMPORT,
    ECHO
} EExpressionType;

typedef enum EInputBehaviour
{
    NORMAL,
    BREAK,
    CONTINUE
} EInputBehaviour;

typedef struct SystemState
{
    bool isFileModeOn;
    bool running;
    bool importRunning;
    bool outputEnabled;
    int lineNumber;
    int sourceLineSaved;
    FILE *sourceFile;
    FILE *libFile;
    VariableDictionary *varDict;
    FunctionDictionary *funcDict;
    char *fileName;
    char *libName;
    char *lastResult;
    char *expr;
    ~SystemState()
    {
        delete varDict;
        delete funcDict;
        delete[] lastResult;
        delete[] expr;
    }
} SystemState;

BETNode *exprToBET(Expression *expr, int nestLevel);
BETNode *exprToBET(Expression *expr);

// THE MAIN PURPOSE OF ALL THIS SHIT
bool eval(char *str, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);

EExpressionType recognizeExpressionType(char *expr, bool importRunning);

void deleteSpaces(char *expr);
int equalsSignIndex(char *expr);
bool hasCompoundAssignment(char *expr);
char getCompoundOperator(char *expr);

// HANDLERS

void showHelp();
EInputBehaviour handlerEvaluate(SystemState *state);
EInputBehaviour handlerEvaluateAndAssign(SystemState *state);
EInputBehaviour handlerCreateFunction(SystemState *state);
EInputBehaviour handlerImport(SystemState *state);
EInputBehaviour handlerEcho(SystemState *state);

EInputBehaviour getInput(SystemState *state);
SystemState *setup(unsigned int variableDictionarySize, unsigned int functionDictionarySize, char *fileName = nullptr);
void CalculatorInit(unsigned int variableDictionarySize, unsigned int functionDictionarySize, char *fileName = nullptr);
void smartLineNumberPrint(char *expr, int lineNumber);
#endif // __CALCULATOR_H__