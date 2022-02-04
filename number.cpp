#include "number.h"

void print(Number n, bool printType)
{
    switch (n.type)
    {
    case EnumberType::INTEGER:
        if (printType)
            printf("int   ");
        printf("%d", n.value.integer);
        break;
    case EnumberType::FLOATING_POINT:
        if (printType)
            printf("float ");
        printf("%.3f", n.value.decimal);
        break;
    default:
        break;
    }
}