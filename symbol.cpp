#include "symbol.h"

Symbol charToSymbol(char ch)
{
    Symbol symb;
    symb.type = recognizeSymbol(ch);
    switch (symb.type)
    {
    case NUMBER:
        symb.entity.number = atoi(&ch);
        break;
    case OPERATOR:
        symb.entity.operator_ = ch;
        break;
    case OPENING_BRACKET:
        symb.entity.bracket = ch;
        break;
    case CLOSING_BRACKET:
        symb.entity.bracket = ch;
        break;
    case VARIABLE:
        symb.entity.variable[0] = ch;
        symb.entity.variable[1] = '\0';
        break;
    case DECIMAL_COMMA:
        break;
    case NOT_A_SYMBOL:
    //проваливаемся в общий случай
    default:
        printf("%c is not a symbol\n", ch);
        break;
    }
    return symb;
}

SymbolType recognizeSymbol(char symbol)
{
    if ('0' <= symbol && symbol <= '9')
    {
        return NUMBER;
    }
    else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^')
    {
        return OPERATOR;
    }
    else if (symbol == '(')
    {
        return OPENING_BRACKET;
    }
    else if (symbol == ')')
    {
        return CLOSING_BRACKET;
    }
    else if (('a' <= symbol && symbol <= 'z') || symbol == '_')
    {
        return VARIABLE;
    }
    else if (symbol == '.')
    {
        return DECIMAL_COMMA;
    }
    return NOT_A_SYMBOL;
}

unsigned int priority(char symb)
{
    switch (symb)
    {
    case '+':
    case '-':
        return 1;
        break;
    case '*':
    case '/':
        return 2;
        break;
    case '^':
        return 3;
        break;
    default:
        printf("invalid op - %c\n", symb);
        return 0;
        break;
    }
}
