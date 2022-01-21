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

//BET

BETNode *createNode();
void insert(BETNode *root, Symbol *s);
Number eval(BETNode *root, Dictionary *dict);

#endif // __BET_H__