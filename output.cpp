#include "output.h"

void tabulate(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("\t");
    }
}

void print(const BETNode *root, int depth)
{
    if (root != nullptr)
    {
        depth++;
        print(root->right, depth);
        tabulate(depth);
        print(*root->s, true);
        printf("\n");
        print(root->left, depth);
    }

    // if (root != nullptr)
    // {
    //     if (root->right != nullptr)
    //     {
    //         tabulate(depth);
    //         printf("right:");
    //         print(root->right, depth + 1);
    //     }

    //     if (depth > 0)
    //     {
    //         tabulate(1);
    //     }
    //     print(*root->s, true);
    //     printf("\n");

    //     if (root->left != nullptr)
    //     {
    //         tabulate(depth);
    //         printf("left:");
    //         print(root->left, depth + 1);
    //     }
    // }

    // else
    // {
    //     if (depth > 0)
    //     {
    //         tabulate(1);
    //     }
    //     printf("EMPTY\n");
    // }
}

void print(const BETNode *root)
{
    printf("BINARY EXPRESSION TREE:\n");
    print(root, 1);
}

void print(Dictionary *dict)
{
    printf("VARIABLES:\n");
    if (dict->freeIndex > 0)
    {
        for (unsigned int i = 0; i < dict->size; i++)
        {
            if (dict->values[i].type != EnumberType::NAN)
            {
                printf("   %s = ", dict->keys[i]);
                print(dict->values[i], true);
                printf("\n");
            }
        }
        printf("END\n");
    }
    else
    {
        printf("NONE\n");
    }
}

void pre_order(const BETNode *root)
{
    if (root == nullptr)
    {
        return;
    }
    print(*root->s);
    printf(" ");
    pre_order(root->left);
    pre_order(root->right);
}

void in_order(const BETNode *root)
{
    if (root == nullptr)
    {
        return;
    }
    pre_order(root->left);
    print(*root->s);
    printf(" ");
    pre_order(root->right);
}

void post_order(const BETNode *root)
{
    if (root == nullptr)
    {
        return;
    }
    pre_order(root->left);
    pre_order(root->right);
    print(*root->s);
    printf(" ");
}

void prettyPrint(const std::string &prefix, const BETNode *node, bool isLeft)
{
    if (node != nullptr)
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "---");

        // print the value of the node
        print(*node->s, true);
        printf("\n");

        // enter the next tree level - left and right branch
        prettyPrint(prefix + (isLeft ? "|   " : "    "), node->left, true);
        prettyPrint(prefix + (isLeft ? "|   " : "    "), node->right, false);
    }
}

void prettyPrint(BETNode *node)
{
    printf("BINARY EXPRESSION TREE:\n");
    prettyPrint("", node, false);
}

void print(Symbol symb, bool brackets)
{
    if (brackets)
        printf("{");
    switch (symb.type)
    {
    case ESymbolType::NUMBER:
        print(symb.entity.number);
        break;
    case ESymbolType::OPERATOR:
        printf("%c", symb.entity.operator_);
        break;
    case ESymbolType::OPENING_BRACKET:
        printf("%c", symb.entity.bracket);
        break;
    case ESymbolType::CLOSING_BRACKET:
        printf("%c", symb.entity.bracket);
        break;
    case ESymbolType::VARIABLE:
        printf("%s", symb.entity.variable);
        break;
    case ESymbolType::DECIMAL_COMMA:
        printf(".");
        break;
    case ESymbolType::NOT_A_SYMBOL:
    //проваливаемся в общий случай
    default:
        printf("NAS"); //Not A Symbol
        break;
    }
    if (brackets)
        printf("}");
}

void print(Expression *expr)
{
    // setPriorities(expr);
    for (int i = 0; i < expr->length; i++)
    {
        print(expr->symbols[i], true);
        // printf("%d ", expr->symbols[i].priority);
    }
    printf("\n");
}

void print(Number n, bool printType)
{
    switch (n.type)
    {
    case EnumberType::INTEGER:
        if (printType)
            printf("int ");
        printf("%d", n.value.integer);
        break;
    case EnumberType::FLOATING_POINT:
        if (printType)
            printf("float ");
        printf("%.2f", n.value.decimal);
        break;
    default:
        break;
    }
}

void saveDictionary(Dictionary *dict, bool asBinary)
{
    if (dict == nullptr)
    {
        printf("dictionary is nullptr\n");
    }

    if (dict->freeIndex == 0)
    {
        printf("nothing to save\n");
        return;
    }

    FILE *f;
    if (asBinary)
    {
        f = fopen("variables", "wb");
        fwrite(&dict->freeIndex, 1, sizeof(int), f);
    }
    else
    {
        f = fopen("variables.txt", "w");
        fprintf(f, "total: %d\n", dict->freeIndex);
    }
    int savedSuccesfullyCounter = 0;
    if (dict->freeIndex > 0)
    {
        for (unsigned int i = 0; i < dict->size; i++)
        {
            if (dict->values[i].type != EnumberType::NAN)
            {
                // printf(">> %s = %.2f\n", dict->keys[i], dict->values[i]);
                if (asBinary)
                {
                    fwrite(dict->keys[i], MAX_VARIABLE_NAME_LEN, sizeof(char), f);
                    fwrite(&dict->values[i], 1, sizeof(Number), f);
                }
                else
                {
                    switch (dict->values[i].type)
                    {
                    case EnumberType::INTEGER:
                        fprintf(f, "%s = %d\n", dict->keys[i], dict->values[i].value.integer);
                        break;
                    case EnumberType::FLOATING_POINT:
                        fprintf(f, "%s = %.2f\n", dict->keys[i], dict->values[i].value.decimal);
                        break;
                    default:
                        break;
                    }
                    // fprintf(f, "%s=%f\n", dict->keys[i], dict->values[i]);
                }
                savedSuccesfullyCounter++;
            }
        }
    }
    fclose(f);
    printf("successfully saved %d variables\n", savedSuccesfullyCounter);
}