#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "io.h"
#include "expression.h"
#include "dictionary.h"
#include "BET.h"
#include "number.h"

BETNode *exprToAET(Expression *expr, int nestLevel);
BETNode *exprToAET(Expression *expr);

//THE MAIN PURPOSE OF ALL THIS SHIT
Number eval(char *str, Dictionary *dict);

void consoleModeStart(unsigned int dictionarySize);
#endif // __CALCULATOR_H__