#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <iostream>

typedef union NumberContainer
{
    double decimal;
    int integer;
} NumberContainer;

typedef enum EnumberType
{
    INTEGER,
    FLOATING_POINT,
    NOT_A_NUMBER
} EnumberType;

typedef struct Number
{
    EnumberType type;
    NumberContainer value;
} Number;

void print(Number n, bool printType = false);
#endif // __NUMBER_H__