#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "function.h"
#include "io.h"
#include "expression.h"
#include "dictionary.h"
#include "BET.h"
#include "number.h"

typedef enum EExpressionType
{
    E_COMMAND_TYPE_FIRST = 0,
    EXIT = 0,
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
    E_COMMAND_TYPE_LAST
} EExpressionType;

BETNode *exprToAET(Expression *expr, int nestLevel);
BETNode *exprToAET(Expression *expr);

//THE MAIN PURPOSE OF ALL THIS SHIT
bool eval(char *str, VariableDictionary *dict, Number &result);

EExpressionType recognizeExpressionType(char *expr);

void deleteSpaces(char *expr);
int equalsSignIndex(char *expr);
bool hasCompoundAssignment(char *expr);
bool isCorrectVariableName(char *var);
char getCompoundOperator(char *expr);

void consoleModeStart(unsigned int dictionarySize);
#endif // __CALCULATOR_H__