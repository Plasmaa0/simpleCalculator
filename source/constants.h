#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

namespace constants
{
    const short MAJOR_VERSION = 0;
    const short MINOR_VERSION = 4;

    const int PRIORITIZED_OPERATOR_NOT_FOUND = -1;

    const int EXPR_MAX_LEN = 100;

    const int MAX_VARIABLE_NAME_LEN = 60;
    const int UNINITIALIZED_VARIABLE = 0;

    const int MAX_INT_LENGTH = 10;
    const int MAX_FLOAT_LENGTH = 307;
    const int MAX_NUMBER_LENGTH = MAX_FLOAT_LENGTH;

    const int FUNCTION_MAX_ARGS_N = 10;
    const int PRIORITY_MAX = 10000000;

    const double EPSILON = 1e-50;
} // namespace constants

#endif // __CONSTANTS_H__