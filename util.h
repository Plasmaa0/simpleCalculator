#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>

#include "constants.h"
#include "number.h"

//UTIL

bool solve(Number a, Number b, char op, Number &result);
double naturalPow(double a, int b);
Number numberFromDigits(const int *digitsArray, int digitsN, const int *decimalPart, int decimalPartLength);

#endif // __UTIL_H__