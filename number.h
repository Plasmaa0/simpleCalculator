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
    ENUMBER_TYPE_FIRST = 0,
    INTEGER = 0,
    FLOATING_POINT,
    NAN,
    ENUMBER_TYPE_LAST
} EnumberType;

typedef struct Number
{
    EnumberType type;
    NumberContainer value;
} Number;

void print(Number n, bool printType = false);
#endif // __NUMBER_H__