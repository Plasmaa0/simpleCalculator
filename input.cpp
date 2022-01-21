#include "input.h"

Dictionary *loadDictionary(unsigned int maxSize)
{
    FILE *f = fopen("variables", "rb");
    if (f == NULL)
    {
        printf("> nothing to load\n");
        fclose(f);
        return nullptr;
    }
    else
    {
        int size;
        fread(&size, 1, sizeof(int), f);
        Dictionary *result = createDictionary(maxSize);
        for (int i = 0; i < size; i++)
        {
            double number;
            char key[MAX_VARIABLE_NAME_LEN];
            fread(&key, sizeof(char), MAX_VARIABLE_NAME_LEN, f);
            fread(&number, 1, sizeof(double), f);
            Number n;
            n.type = FLOATING_POINT;
            n.value.decimal = number;
            setVariable(key, n, result);
            // printf("%s = %f\n", key, number);
        }
        fclose(f);
        printf("> successfully loaded %d variables\n", size);
        return result;
    }
    fclose(f);
    printf("> null\n");
    return nullptr;
}