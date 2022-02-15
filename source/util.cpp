#include "util.h"

bool solve(Number a, Number b, char op, Number &result)
{
    bool modulusAvailable = (a.type == EnumberType::INTEGER and b.type == EnumberType::INTEGER);
    auto aValue = (a.type == EnumberType::INTEGER ? a.value.integer : a.value.decimal);
    auto bValue = (b.type == EnumberType::INTEGER ? b.value.integer : b.value.decimal);
    bool isResultTypeInteger = ((a.type == EnumberType::FLOATING_POINT or b.type == EnumberType::FLOATING_POINT) ? false : true);
    result.type = (isResultTypeInteger ? EnumberType::INTEGER : EnumberType::FLOATING_POINT);

    switch (op)
    {
    case '+':
        if (isResultTypeInteger)
            result.value.integer = aValue + bValue;
        else
            result.value.decimal = aValue + bValue;
        return true;
        break;
    case '-':
        if (isResultTypeInteger)
            result.value.integer = aValue - bValue;
        else
            result.value.decimal = aValue - bValue;
        return true;
        break;
    case '*':
        if (isResultTypeInteger)
            result.value.integer = aValue * bValue;
        else
            result.value.decimal = aValue * bValue;
        return true;
        break;
    case '/':
        if (fabs(bValue) < constants::EPSILON)
        {
            printf("zero division error\n");
            return false;
        }

        if (isResultTypeInteger)
            result.value.integer = aValue / bValue; // integer division
        else
            result.value.decimal = aValue / bValue;
        return true;
        break;
    case '^':
        if (b.type == EnumberType::INTEGER)
        {
            if (isResultTypeInteger)
                result.value.integer = naturalPow(aValue, bValue);
            else
                result.value.decimal = naturalPow(aValue, bValue);
            return true;
        }
        else
        {
            printf("unavailable natural power due to operand types\n");
            return false;
        }
        break;
    case '<':
        if (b.type == EnumberType::INTEGER)
        {
            if (isResultTypeInteger)
                result.value.integer = (int)aValue << (int)bValue;
            else
            {
                printf("unavailable bitwise shift < due to operand types\n");
                return false;
            }
            return true;
        }
        else
        {
            printf("unavailable bitwise shift < due to operand types\n");
            return false;
        }
        break;
    case '>':
        if (b.type == EnumberType::INTEGER)
        {
            if (isResultTypeInteger)
                result.value.integer = (int)aValue >> (int)bValue;
            else
            {
                printf("unavailable byte move > due to operand types\n");
                return false;
            }
            return true;
        }
        else
        {
            printf("unavailable byte move > due to operand types\n");
            return false;
        }
        break;
    case '%':
        if (modulusAvailable)
        {
            result.value.integer = a.value.integer % b.value.integer;
            return true;
        }
        else
        {
            printf("modulus operation not available due to operand types\n");
            return false;
        }
        break;
    default:
        printf("invalid operator [%c] [SOLVE]\n", op);
        break;
    }
    return false;
}

double naturalPow(double a, int b)
{
    double result = 1;
    if (b == 0)
    {
        return result;
    }
    else if (b > 0)
    {
        for (int i = 0; i < b; i++)
        {
            result *= a;
        }
        return result;
    }
    return result;
}

Number numberFromDigits(const int *digitsArray, int digitsN, const int *decimalPart, int decimalPartLength)
{
    if (digitsN + decimalPartLength > (decimalPartLength > 0 ? constants::MAX_FLOAT_LENGTH : constants::MAX_INT_LENGTH))
    {
        printf("too long number\n");
    }

    Number resultNumber;
    if (decimalPartLength > 0)
    {
        resultNumber.type = EnumberType::FLOATING_POINT;
    }
    else
    {
        resultNumber.type = EnumberType::INTEGER;
    }

    int integer = 0;
    {
        int integerMultiplier = 1;
        for (int i = digitsN - 1; i >= 0; i--)
        {
            integer += digitsArray[i] * integerMultiplier;
            integerMultiplier *= 10;
        }
    }
    double decimal = 0.0;
    if (resultNumber.type == EnumberType::FLOATING_POINT)
    {
        double decimalMultiplier = 0.1;
        for (int i = 0; i < decimalPartLength; i++)
        {
            decimal += decimalPart[i] * decimalMultiplier;
            decimalMultiplier /= 10;
        }
        resultNumber.value.decimal = (double)integer + decimal;
    }
    else
    {
        resultNumber.value.integer = integer;
    }
    return resultNumber;
}

bool isCorrectVariableName(char *var)
{
    if (recognizeSymbol(var[0]) == ESymbolType::NUMBER)
    {
        return false;
    }

    for (unsigned int i = 0; i < strlen(var); i++)
    {
        ESymbolType t = recognizeSymbol(var[i]);
        if (not(t == ESymbolType::NUMBER or t == ESymbolType::VARIABLE or var[i] == ' '))
        {
            return false;
        }
    }
    return true;
}

void deleteSpaces(char *expr)
{
    expr[strcspn(expr, "\n")] = '\0';
    char result[constants::EXPR_MAX_LEN + 1];
    int resultLength = 0;
    for (unsigned int i = 0; i < strlen(expr); i++)
    {
        if (expr[i] != ' ')
        {
            result[resultLength] = expr[i];
            resultLength++;
        }
    }
    if (resultLength == (int)strlen(expr))
    {
        return;
    }
    result[resultLength] = '\0';
    strncpy(expr, result, constants::EXPR_MAX_LEN + 1);
}

int equalsSignIndex(char *expr)
{
    return strchr(expr, '=') - expr;
}

bool hasCompoundAssignment(char *expr)
{
    return recognizeSymbol(expr[equalsSignIndex(expr) - 1]) == ESymbolType::OPERATOR;
}

char getCompoundOperator(char *expr)
{
    return expr[equalsSignIndex(expr) - 1];
}