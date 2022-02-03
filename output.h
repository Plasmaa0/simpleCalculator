#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <iostream>
#include <time.h>

#include "expression.h"
#include "symbol.h"
#include "dictionary.h"
#include "number.h"
#include "BET.h"
#include "function.h"

//OUTPUT

void tabulate(int n);
void pre_order(const BETNode *root);
void in_order(const BETNode *root);
void post_order(const BETNode *root);
void prettyPrint(BETNode *node);
void print(const BETNode *root);
void print(VariableDictionary *dict);

void saveDictionary(VariableDictionary *dict, bool asBinary = false);

#endif // __OUTPUT_H__