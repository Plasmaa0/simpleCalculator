#ifndef __TYPES_H__
#define __TYPES_H__

#include <string.h>
#include "symbol.h"
#include "util.h"
// #include "function.h"
// #include "output.h"

typedef struct Expression
{
    int length;
    Symbol *symbols;
    ~Expression()
    {
        // for (int i = 0; i < length; i++)
        // {
        //     if (symbols[i].type == ESymbolType::FUNCTION_CALL)
        //     {
        //         delete symbols[i].entity.functionCall;
        //     }
        // }
        delete[] symbols;
    }
} Expression;

void print(Expression *expr);

//EXPRESSION

Expression *strToExpr(char *str);
Expression *slice(Expression *expr, int a, int b);
Expression *strip(Expression *expr);
unsigned int prioritizedOperatorIndex(Expression *expr);
void setPriorities(Expression *expr);

Expression *handleNumberSequences(Expression *expr);
Expression *handleLetterSequences(Expression *expr);
Expression *handleUnaryMinus(Expression *expr);
Expression *deleteNAS(Expression *expr);
Expression *handleFunctions(Expression *expr);

#endif // __TYPES_H__