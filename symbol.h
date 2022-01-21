#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <iostream>

#include "constants.h"
#include "number.h"

typedef enum ESymbolType
{
    ESYMBOL_TYPE_FIRST = 0,
    NUMBER = 0,
    DECIMAL_COMMA,
    OPERATOR,
    VARIABLE,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    NOT_A_SYMBOL,
    SYMBOL_TYPE_LAST
} ESymbolType;

typedef union SymbolContainer
{
    Number number;
    char operator_;
    char variable[MAX_VARIABLE_NAME_LEN + 1];
    char bracket;
} SymbolContainer;

typedef struct Symbol
{
    int priority;
    ESymbolType type;
    SymbolContainer entity;
} Symbol;

//SYMBOL

Symbol charToSymbol(char ch);
ESymbolType recognizeSymbol(char symbol);
unsigned int priority(char symb);

#endif // __SYMBOL_H__