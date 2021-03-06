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
        root->s = new Symbol;
        *root->s = *s;
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

bool evaluateFunctionCall(Symbol *functionCallSymbol, VariableDictionary *dict, FunctionDictionary *fdict, Number &result)
{
    bool success = true;
    int argsN = functionCallSymbol->entity.functionCall->argsN;
    Number argValues[argsN];
    for (int i = 0; i < argsN; i++)
    {
        Symbol currentArg = functionCallSymbol->entity.functionCall->args[i];
        if (currentArg.type == ESymbolType::VARIABLE)
        {
            success = (success && getVariable(currentArg.entity.variable, dict, argValues[i]));
        }
        else if (currentArg.type == ESymbolType::NUMBER)
        {
            argValues[i] = currentArg.entity.number;
        }
        else if (currentArg.type == ESymbolType::FUNCTION_CALL)
        {
            success = (success && evaluateFunctionCall(&currentArg, dict, fdict, argValues[i]));
        }
    }
    Function *func = nullptr;
    success = (success && getFunction(functionCallSymbol->entity.functionCall->functionName, fdict, &func));
    if (success)
    {
        success = success && evaluateFunction(argValues, argsN, func, dict, fdict, result);
    }
    return success;
}

bool eval(BETNode *root, VariableDictionary *dict, FunctionDictionary *fdict, Number &result)
{
    if (root == nullptr)
    {
        printf("syntax error\n");
        return false;
    }
    if (root->s->type == ESymbolType::FUNCTION_CALL)
    {
        bool success = evaluateFunctionCall(root->s, dict, fdict, result);
        return success;
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
    bool aSuccess = eval(root->left, dict, fdict, a);
    Number b;
    bool bSuccess = eval(root->right, dict, fdict, b);
    bool solveSuccess = solve(a, b, root->s->entity.operator_, result);
    bool totalSuccess = (aSuccess && bSuccess && solveSuccess);
    return totalSuccess;
}

void destroyBET(BETNode *root)
{
    if (root != nullptr)
    {
        destroyBET(root->left);
        destroyBET(root->right);
        delete root->s;
        delete root;
    }
}