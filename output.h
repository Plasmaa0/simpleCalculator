#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <iostream>
#include <time.h>

#include "expression.h"
#include "symbol.h"
#include "dictionary.h"
#include "number.h"
#include "BET.h"

//OUTPUT

void tabulate(int n);
void pre_order(const BETNode *root);
void in_order(const BETNode *root);
void post_order(const BETNode *root);
void prettyPrint(BETNode *node);
void print(const BETNode *root);
void print(Symbol symb, bool brackets = false);
void print(Dictionary *dict);
void print(Number n, bool printType = false);

void saveDictionary(Dictionary *dict, bool asBinary = false);

#endif // __OUTPUT_H__