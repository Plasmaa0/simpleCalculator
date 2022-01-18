#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "constants.h"

typedef enum SymbolType
{
    SYMBOL_TYPE_FIRST = 0,
    NUMBER = 0,
    DECIMAL_COMMA,
    OPERATOR,
    VARIABLE,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    NOT_A_SYMBOL,
    SYMBOL_TYPE_LAST
} SymbolType;

typedef union SymbolContainer
{
    double number;
    char operator_;
    char variable[MAX_VARIABLE_NAME_LEN + 1];
    char bracket;
} SymbolContainer;

typedef struct Symbol
{
    int priority;
    SymbolType type;
    SymbolContainer entity;
} Symbol;

//SYMBOL

Symbol charToSymbol(char ch);
SymbolType recognizeSymbol(char symbol);
unsigned int priority(char symb);

#endif // __SYMBOL_H__