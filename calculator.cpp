#include "calculator.h"

BETNode *exprToAET(Expression *expr, int nestLevel)
{
    int priorOpIndex = prioritizedOperatorIndex(expr);

    if (priorOpIndex == PRIORITIZED_OPERATOR_NOT_FOUND)
    {
        expr = strip(expr);
    }

    BETNode *root = createNode();
    if (expr->length == 1)
    {
        insert(root, &expr->symbols[0]);
        return root;
    }

    priorOpIndex = prioritizedOperatorIndex(expr);
    Symbol *priorSymbol = expr->symbols + priorOpIndex;
    insert(root, priorSymbol);

    // printf("prior op index: %d\n", priorOpIndex);
    // printf("prior op: ");
    // print(*priorSymbol);
    // printf("\n");
    // printf("expression: ");
    // print(expr);
    // printf("left subtree:");
    // print(strip(slice(expr, 0, priorOpIndex)));
    // printf("right subtree:");
    // print(strip(slice(expr, priorOpIndex + 1, expr->length)));
    // printf("\n");

    BETNode *leftSubtree = exprToAET(slice(expr, 0, priorOpIndex), nestLevel);
    root->left = leftSubtree;

    BETNode *rightSubtree = exprToAET(slice(expr, priorOpIndex + 1, expr->length), nestLevel);
    root->right = rightSubtree;
    return root;
}

BETNode *exprToAET(Expression *expr)
{
    return exprToAET(expr, 0);
}

Number eval(char *str, Dictionary *dict)
{
    Expression *e = strToExpr(str);
    // printf("strToExpr success: ");
    // print(e);
    // printf("\n");
    BETNode *root = exprToAET(e);
    // printf("exprToAET success\n");
    Number value = eval(root, dict);
    // printf("eval success\n");
    // printf("pre-order: ");
    // pre_order(root);
    // printf("\n");
    // printf("in-order: ");
    // in_order(root);
    // printf("\n");
    // printf("post-order: ");
    // post_order(root);
    // printf("\n");
    // prettyPrint(root);
    // print(root);

    return value;
}

void consoleModeStart(unsigned int dictionarySize)
{
    printf("> hello\n");
    Dictionary *dict = createDictionary(dictionarySize);
    char expr[EXPR_MAX_LEN + 1];
    char var[MAX_VARIABLE_NAME_LEN + 1];
    while (1)
    {
        fgets(expr, EXPR_MAX_LEN, stdin);

        expr[strlen(expr) - 1] = '\0';
        // printf("%s\n%s\n", expr, "exit");
        if (strcmp(expr, "exit") == 0)
        {
            break;
        }
        if (strcmp(expr, "vars") == 0)
        {
            print(dict);
            continue;
        }
        if (strcmp(expr, "saveB") == 0)
        {
            saveDictionary(dict, true);
            continue;
        }
        if (strcmp(expr, "save") == 0)
        {
            saveDictionary(dict);
            continue;
        }
        if (strcmp(expr, "load") == 0)
        {
            Dictionary *newDict = loadDictionary(dictionarySize);
            if (newDict != nullptr)
            {
                dict = newDict;
            }

            continue;
        }

        bool resultToVariable = false;
        if (strchr(expr, '='))
        {
            strncpy(var, strtok(expr, " ="), MAX_VARIABLE_NAME_LEN);
            strncpy(expr, strtok(NULL, "="), EXPR_MAX_LEN);
            // printf("var: |%s|\nexpr:|%s|\n", var, expr);
            resultToVariable = true;
        }
        // printf("expr: %s\n", expr);
        Number result = eval(expr, dict);
        setVariable("_", result, dict);
        printf("> ");
        print(result);
        printf("\n");
        if (resultToVariable)
        {
            setVariable(var, result, dict);
        }
    }
    printf("> bye\n");
}