#include "BET.h"

BETNode *createNode()
{
    BETNode *node = new BETNode;
    node->left = nullptr;
    node->right = nullptr;
    node->s = nullptr;
    return node;
}

void insert(BETNode *root, Symbol *s)
{
    if (root->s == nullptr)
    {
        root->s = s;
        // printf("s\n");
        return;
    }
    if (s->priority >= root->s->priority)
    {
        if (root->right == nullptr)
            root->right = createNode();

        insert(root->right, s);
        return;
    }
    if (s->priority < root->s->priority)
    {
        if (root->left == nullptr)
            root->left = createNode();
        insert(root->left, s);
        return;
    }
}

bool eval(BETNode *root, VariableDictionary *dict, Number &result)
{
    if (root == nullptr)
    {
        printf("syntax error\n");
        return false;
    }
    if (root->s->type == ESymbolType::NUMBER)
    {
        result = root->s->entity.number;
        return true;
    }
    if (root->s->type == ESymbolType::VARIABLE)
    {
        return getVariable(root->s->entity.variable, dict, result);
    }
    if (root->left == nullptr or root->right == nullptr)
    {
        printf("syntax error\n");
        return false;
    }

    Number a;
    bool aSuccess = eval(root->left, dict, a);
    Number b;
    bool bSuccess = eval(root->right, dict, b);
    bool solveSuccess = solve(a, b, root->s->entity.operator_, result);
    bool totalSuccess = (aSuccess && bSuccess && solveSuccess);
    return totalSuccess;
}
