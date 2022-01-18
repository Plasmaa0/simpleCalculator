#include "util.h"

double solve(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
        break;
    case '-':
        return a - b;
        break;
    case '*':
        return a * b;
        break;
    case '/':
        return a / b;
        break;
    case '^':
        return naturalPow(a, b);
        break;
    default:
        printf("invalid operator [%c] [SOLVE]\n", op);
        break;
    }
    return -999999;
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

double numberFromDigits(int *digitsArray, int digitsN, int *decimalPart, int decimalPartLength)
{
    double result = 0;
    double multiplier = 1;
    for (int i = digitsN - 1; i >= 0; i--)
    {
        result += digitsArray[i] * multiplier;
        multiplier *= 10;
    }
    multiplier = 0.1;
    for (int i = 0; i < decimalPartLength; i++)
    {
        result += decimalPart[i] * multiplier;
        multiplier /= 10;
    }
    // printf("NFD: %f\n", result);
    return result;
}
