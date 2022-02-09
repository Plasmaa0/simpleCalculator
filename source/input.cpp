#include "input.h"

VariableDictionary *loadDictionary(unsigned int maxSize)
{
    FILE *f = fopen("variables", "rb");
    if (f == nullptr)
    {
        printf("nothing to load\n");
        fclose(f);
        return nullptr;
    }
    else
    {
        int size;
        fread(&size, 1, sizeof(int), f);
        VariableDictionary *result = createVariableDictionary(maxSize);
        for (int i = 0; i < size; i++)
        {
            Number n;
            char key[constants::MAX_VARIABLE_NAME_LEN];
            fread(&key, sizeof(char), constants::MAX_VARIABLE_NAME_LEN, f);
            fread(&n, 1, sizeof(Number), f);
            setVariable(key, n, result);
        }
        fclose(f);
        printf("successfully loaded %d variables\n", size);
        return result;
    }
    fclose(f);
    printf("null\n");
    return nullptr;
}