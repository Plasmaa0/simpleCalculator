#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <iostream>

#include "constants.h"
#include "number.h"

typedef enum ESymbolType
{
    ESYMBOL_TYPE_FIRST = 0,
    NUMBER = 0,
    COMMA,
    DECIMAL_COMMA,
    OPERATOR,
    VARIABLE,
    FUNCTION_CALL,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    NOT_A_SYMBOL,
    SYMBOL_TYPE_LAST
} ESymbolType;

struct functionCall;
// typedef struct functionCall functionCall;

typedef union SymbolContainer
{
    Number number;
    char operator_;
    char variable[constants::MAX_VARIABLE_NAME_LEN + 1];
    char bracket;
    struct functionCall *functionCall;
} SymbolContainer;

typedef struct Symbol
{
    int priority;
    ESymbolType type;
    SymbolContainer entity;
} Symbol;

typedef struct functionCall
{
    int argsN;
    Symbol args[constants::FUNCTION_MAX_ARGS_N];
    char functionName[constants::MAX_VARIABLE_NAME_LEN + 1];
} functionCall;

//SYMBOL

Symbol charToSymbol(char ch);
ESymbolType recognizeSymbol(char symbol);
unsigned int priority(char symb);
void print(Symbol symb, bool brackets = false);

#endif // __SYMBOL_H__