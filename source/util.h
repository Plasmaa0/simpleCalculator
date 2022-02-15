#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <string.h>
#include <cmath>

#include "constants.h"
#include "symbol.h"
#include "number.h"

//UTIL

bool solve(Number a, Number b, char op, Number &result);
double naturalPow(double a, int b);
Number numberFromDigits(const int *digitsArray, int digitsN, const int *decimalPart, int decimalPartLength);
bool isCorrectVariableName(char *var);
void deleteSpaces(char *expr);
int equalsSignIndex(char *expr);
bool hasCompoundAssignment(char *expr);
char getCompoundOperator(char *expr);
#endif // __UTIL_H__