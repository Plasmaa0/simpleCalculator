#include "symbol.h"

Symbol charToSymbol(char ch)
{
    Symbol symb;
    symb.type = recognizeSymbol(ch);
    switch (symb.type)
    {
    case ESymbolType::NUMBER:
        symb.entity.number.type = EnumberType::INTEGER;
        symb.entity.number.value.integer = atoi(&ch);
        break;
    case ESymbolType::OPERATOR:
        symb.entity.operator_ = ch;
        break;
    case ESymbolType::OPENING_BRACKET:
        symb.entity.bracket = ch;
        break;
    case ESymbolType::CLOSING_BRACKET:
        symb.entity.bracket = ch;
        break;
    case ESymbolType::VARIABLE:
        symb.entity.variable[0] = ch;
        symb.entity.variable[1] = '\0';
        break;
    case ESymbolType::DECIMAL_COMMA:
        break;
    case ESymbolType::NOT_A_SYMBOL:
    //проваливаемся в общий случай
    default:
        printf("%c is not a symbol\n", ch);
        break;
    }
    return symb;
}

ESymbolType recognizeSymbol(char symbol)
{
    if ('0' <= symbol && symbol <= '9')
    {
        return ESymbolType::NUMBER;
    }
    else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^' || symbol == '%')
    {
        return ESymbolType::OPERATOR;
    }
    else if (symbol == '(')
    {
        return ESymbolType::OPENING_BRACKET;
    }
    else if (symbol == ')')
    {
        return ESymbolType::CLOSING_BRACKET;
    }
    else if (('a' <= symbol && symbol <= 'z') || symbol == '_')
    {
        return ESymbolType::VARIABLE;
    }
    else if (symbol == '.')
    {
        return ESymbolType::DECIMAL_COMMA;
    }
    return ESymbolType::NOT_A_SYMBOL;
}

unsigned int priority(char symb)
{
    switch (symb)
    {
    case '+':
    case '-':
        return 1;
        break;
    case '%':
        return 2;
        break;
    case '*':
    case '/':
        return 3;
        break;
    case '^':
        return 4;
        break;
    default:
        printf("invalid op - %c\n", symb);
        return 0;
        break;
    }
}
