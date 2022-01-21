#include "util.h"

Number solve(Number a, Number b, char op)
{
    bool modulusAvailable = (a.type == INTEGER and b.type == INTEGER);
    auto aValue = (a.type == INTEGER ? a.value.integer : a.value.decimal);
    auto bValue = (b.type == INTEGER ? b.value.integer : b.value.decimal);
    bool isResultTypeInteger = ((a.type == FLOATING_POINT or b.type == FLOATING_POINT) ? false : true);
    Number result;
    result.type = (isResultTypeInteger ? INTEGER : FLOATING_POINT);

    switch (op)
    {
    case '+':
        if (isResultTypeInteger)
            result.value.integer = aValue + bValue;
        else
            result.value.decimal = aValue + bValue;
        return result;
        break;
    case '-':
        if (isResultTypeInteger)
            result.value.integer = aValue - bValue;
        else
            result.value.decimal = aValue - bValue;
        return result;
        break;
    case '*':
        if (isResultTypeInteger)
            result.value.integer = aValue * bValue;
        else
            result.value.decimal = aValue * bValue;
        return result;
        break;
    case '/':
        if (isResultTypeInteger)
            result.value.integer = aValue / bValue; // integer division
        else
            result.value.decimal = aValue / bValue;
        return result;
        break;
    case '^':
        if (b.type == INTEGER)
        {
            if (isResultTypeInteger)
                result.value.integer = naturalPow(aValue, bValue); // integer division
            else
                result.value.decimal = naturalPow(aValue, bValue);
        }
        else
        {
            printf("> unavailable natural power due to operand types\n");
        }
        return result;
        break;
    case '%':
        if (modulusAvailable)
        {
            result.value.integer = a.value.integer % b.value.integer;
        }
        else
        {
            result.value.integer = 0;
            result.value.decimal = 0.0;
            printf("> modulus operation not available due to operand types\n");
        }

        return result;
        break;
    default:
        printf("> invalid operator [%c] [SOLVE]\n", op);
        break;
    }
    return result; // may be uninitialized result
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

Number numberFromDigits(int *digitsArray, int digitsN, int *decimalPart, int decimalPartLength)
{
    Number resultNumber;
    if (decimalPartLength > 0)
    {
        resultNumber.type = FLOATING_POINT;
    }
    else
    {
        resultNumber.type = INTEGER;
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
    if (resultNumber.type == FLOATING_POINT)
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

    // printf("NFD: %f\n", result);
    return resultNumber;
}
