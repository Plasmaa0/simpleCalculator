#ifndef __BET_H__
#define __BET_H__

#include "symbol.h"
#include "dictionary.h"
#include "util.h"
#include "number.h"

//Binary Expression Tree Node
typedef struct BETNode
{
    Symbol *s;
    BETNode *left;
    BETNode *right;
} BETNode;

#include "function.h"

//BET

BETNode *createNode();
void insert(BETNode *root, Symbol *s);
bool eval(BETNode *root, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);
bool evaluateFunctionCall(Symbol *functionCallSymbol, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);

#endif // __BET_H__