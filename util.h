#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include "number.h"

//UTIL

Number solve(Number a, Number b, char op);
double naturalPow(double a, int b);
Number numberFromDigits(int *digitsArray, int digitsN, int *decimalPart, int decimalPartLength);

#endif // __UTIL_H__