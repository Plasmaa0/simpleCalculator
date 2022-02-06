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
            //if single variable not exist, success->false
            // printf("got variable %s\n", currentArg.entity.variable);
            success = (success && getVariable(currentArg.entity.variable, dict, argValues[i]));
        }
        else if (currentArg.type == ESymbolType::NUMBER)
        {
            // printf("got number ");
            // print(currentArg);
            // printf("\n");
            argValues[i] = currentArg.entity.number;
        }
        else if (currentArg.type == ESymbolType::FUNCTION_CALL)
        {
// printf("started evaluating %s\n", currentArg.entity.functionCall->functionName);
#ifdef DEBUF
            printf("                         EVALUATE FUNCTION CALL --->>> EVALUATE FUNCTION CALL\n");
#endif
            success = (success && evaluateFunctionCall(&currentArg, dict, fdict, argValues[i]));
        }
        // printf("%s arg %d = ", functionCallSymbol->entity.functionCall->functionName, i);
        // print(argValues[i]);
        // printf("\n");
    }
    Function *func = nullptr;
    success = (success && getFunction(functionCallSymbol->entity.functionCall->functionName, fdict, &func));
    if (success)
    {
#ifdef DEBUF
        printf("                EVALUATE FUNCTION CALL --->>> EVALUATE FUNCTION\n");
#endif
        success = success && evaluateFunction(argValues, argsN, func, fdict, result);
    }
#ifdef DEBUF
    printf("evaluated %s with args: ", functionCallSymbol->entity.functionCall->functionName);
    for (int i = 0; i < argsN; i++)
    {
        print(functionCallSymbol->entity.functionCall->args[i]);
        printf(" ");
    }
    printf("\n");
#endif
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
#ifdef DEBUF
        printf("                         EVAL --->>> EVALUATE FUNCTION CALL\n");
#endif
        // printf("a\n");
        bool success = evaluateFunctionCall(root->s, dict, fdict, result);
        // printf("b\n");
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