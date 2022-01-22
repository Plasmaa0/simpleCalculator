#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "io.h"
#include "expression.h"
#include "dictionary.h"
#include "BET.h"
#include "number.h"

typedef enum ECommandType
{
    E_COMMAND_TYPE_FIRST = 0,
    E_COMAND_TYPE_BREAK = 0,
    E_COMMAND_TYPE_LAST
} ECommandType;

BETNode *exprToAET(Expression *expr, int nestLevel);
BETNode *exprToAET(Expression *expr);

//THE MAIN PURPOSE OF ALL THIS SHIT
bool eval(char *str, Dictionary *dict, Number &result);

void consoleModeStart(unsigned int dictionarySize);
#endif // __CALCULATOR_H__