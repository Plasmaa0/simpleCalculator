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

double eval(BETNode *root, Dictionary *dict)
{
    if (root->s->type == NUMBER)
    {
        return root->s->entity.number;
    }
    if (root->s->type == VARIABLE)
    {
        return getVariable(root->s->entity.variable, dict);
    }
    return solve(eval(root->left, dict), eval(root->right, dict), root->s->entity.operator_);
}
