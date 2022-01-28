#include <iostream>
#include <string.h>

using namespace std;

const int EXPR_MAX_LEN = 100;

void deleteSpaces(char *expr)
{
    expr[strcspn(expr, "\n")] = '\0';
    char result[EXPR_MAX_LEN + 1];
    int resultLength = 0;
    for (unsigned int i = 0; i < strlen(expr); i++)
    {
        if (expr[i] != ' ')
        {
            result[resultLength] = expr[i];
            resultLength++;
        }
    }
    result[resultLength] = '\0';
    // printf("3\n");
    strncpy(expr, result, EXPR_MAX_LEN + 1);
}

int main()
{
    FILE *expectedSource = fopen("expected.txt", "r");
    char *expected = new char[EXPR_MAX_LEN + 1];

    FILE *gotSource = fopen("got.txt", "r");
    char *got = new char[EXPR_MAX_LEN + 1];

    bool running = true;
    int lineNumber = 0;

    //skip 2 first info lines
    fgets(expected, EXPR_MAX_LEN, expectedSource);
    fgets(expected, EXPR_MAX_LEN, expectedSource);
    fgets(got, EXPR_MAX_LEN, gotSource);
    fgets(got, EXPR_MAX_LEN, gotSource);

    int passed = 0;
    int failed = 0;
    int total = 0;
    while (running)
    {
        lineNumber++;
        running = fgets(expected, EXPR_MAX_LEN, expectedSource) != nullptr;
        running = fgets(got, EXPR_MAX_LEN, gotSource) != nullptr;
        if (not running)
            break;
        deleteSpaces(expected);
        deleteSpaces(got);
        if (strcmp(expected, got) != 0)
        {
            printf("\x1B[31m[%d] ERROR\nexpected: |%s|\ngot: |%s|\033[0m\n", lineNumber, expected, got);
            failed++;
        }
        else
        {
            // printf("line %d passed\n", lineNumber);
            passed++;
        }
        total++;
    }
    printf("\x1B[35mtotal\033[0m: %d\n", total);
    printf("\x1B[32mpassed\033[0m: %d\n", passed);
    printf("\x1B[31mfailed\033[0m: %d\n", failed);

    return 0;
}