#include "symbol.h"

Symbol charToSymbol(char ch)
{
    Symbol symb;
    symb.type = recognizeSymbol(ch);
    switch (symb.type)
    {
    case ESymbolType::NUMBER:
        symb.entity.number.type = EnumberType::INTEGER;
        symb.entity.number.value.integer = (int)ch - '0';
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
    // go to default case
    default:
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
    else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^' || symbol == '%' || symbol == '<' || symbol == '>')
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
    else if (('A' <= symbol && symbol <= 'Z') || ('a' <= symbol && symbol <= 'z') || symbol == '_')
    {
        return ESymbolType::VARIABLE;
    }
    else if (symbol == '.')
    {
        return ESymbolType::DECIMAL_COMMA;
    }
    else if (symbol == ',')
    {
        return ESymbolType::COMMA;
    }
    return ESymbolType::NOT_A_SYMBOL;
}

unsigned int priority(char symb)
{
    switch (symb)
    {
    case '<':
    case '>':
        return 1;
        break;
    case '+':
    case '-':
        return 2;
        break;
    case '%':
        return 3;
        break;
    case '*':
    case '/':
        return 4;
        break;
    case '^':
        return 5;
        break;
    default:
        printf("invalid op - %c\n", symb);
        return 0;
        break;
    }
}

void print(Symbol symb, bool brackets)
{
    if (brackets)
        printf("{");
    switch (symb.type)
    {
    case ESymbolType::NUMBER:
        print(symb.entity.number);
        break;
    case ESymbolType::OPERATOR:
        printf("%c", symb.entity.operator_);
        break;
    case ESymbolType::OPENING_BRACKET:
        printf("%c", symb.entity.bracket);
        break;
    case ESymbolType::CLOSING_BRACKET:
        printf("%c", symb.entity.bracket);
        break;
    case ESymbolType::VARIABLE:
        printf("%s", symb.entity.variable);
        break;
    case ESymbolType::DECIMAL_COMMA:
        printf(".");
        break;
    case ESymbolType::COMMA:
        printf(",");
        break;
    case ESymbolType::FUNCTION_CALL:
        printf("%s(", symb.entity.functionCall->functionName);
        for (int i = 0; i < symb.entity.functionCall->argsN; i++)
        {
            print(symb.entity.functionCall->args[i], false);
            if (i != symb.entity.functionCall->argsN - 1)
                printf(",");
        }
        printf(")");

        break;
    case ESymbolType::NOT_A_SYMBOL:
    // go to default case
    default:
        printf("NAS"); // Not A Symbol
        break;
    }
    if (brackets)
        printf("}");
}