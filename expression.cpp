#include "expression.h"

Expression *handleNumberSequences(Expression *expr)
{
    Symbol *oversizedSymbolArray = new Symbol[expr->length];
    int resultLength = 0;

    bool currentIsNumber = false;
    bool decimalPart = false;

    int digitsBuffer[MAX_NUMBER_LENGTH];
    int currentNumberLength = 0;

    int decimalPartBuffer[MAX_NUMBER_LENGTH];
    int decimalPartLength = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (currentNumberLength > MAX_NUMBER_LENGTH or decimalPartLength > MAX_NUMBER_LENGTH)
        {
            printf("expression has too long numbers\n");
        }

        if (expr->symbols[symbolIndex].type == DECIMAL_COMMA)
        {
            // printf("comma index: %d\n", symbolIndex);
            decimalPart = true;
            currentIsNumber = true;
            continue;
        }

        if (expr->symbols[symbolIndex].type == NUMBER)
        {
            if (currentIsNumber)
            {
                if (decimalPart)
                {
                    decimalPartBuffer[decimalPartLength] = expr->symbols[symbolIndex].entity.number;
                    // printf("|%d|", decimalPartBuffer[decimalPartLength]);
                    decimalPartLength++;
                }
                else
                {
                    digitsBuffer[currentNumberLength] = expr->symbols[symbolIndex].entity.number;
                    currentNumberLength++;
                }
            }
            else
            {
                currentIsNumber = true;
                symbolIndex--;
            }
        }
        else
        {
            Symbol newSymbol;
            if (currentNumberLength + decimalPartLength > 0)
            {
                //number finished construction
                // print(digitsBuffer, currentNumberLength);
                double number = numberFromDigits(digitsBuffer, currentNumberLength, decimalPartBuffer, decimalPartLength);
                // printf("%d", number);

                //create symbol
                newSymbol.type = NUMBER;
                newSymbol.entity.number = number;

                //reset cycle
                memset(digitsBuffer, 0, MAX_NUMBER_LENGTH * sizeof(int));
                memset(decimalPartBuffer, 0, MAX_NUMBER_LENGTH * sizeof(int));
                decimalPartLength = 0;
                currentNumberLength = 0;
                currentIsNumber = false;
                decimalPart = false;

                //add to array
                oversizedSymbolArray[resultLength] = newSymbol;
                resultLength++;
            }
            newSymbol = expr->symbols[symbolIndex];
            oversizedSymbolArray[resultLength] = newSymbol;
            resultLength++;
            // print(newSymbol);
        }
    }

    if (currentNumberLength + decimalPartLength > 0)
    {
        Symbol newSymbol;
        //number finished construction
        // print(digitsBuffer, currentNumberLength);
        double number = numberFromDigits(digitsBuffer, currentNumberLength, decimalPartBuffer, decimalPartLength);
        // printf("%d", number);

        //create symbol
        newSymbol.type = NUMBER;
        newSymbol.entity.number = number;

        //add to array
        oversizedSymbolArray[resultLength] = newSymbol;
        resultLength++;
    }

    Expression *result = new Expression;
    result->length = resultLength;
    result->symbols = new Symbol[resultLength];
    // memcpy(result->symbols, oversizedSymbolArray, resultLength * sizeof(Symbol));

    // printf("over:\n");
    for (int i = 0; i < resultLength; i++)
    {
        // print(oversizedSymbolArray[i]);
        result->symbols[i] = oversizedSymbolArray[i];
    }
    // printf("\nexpr:\n");
    // print(result);

    // printf("\n");
    return result;
}

Expression *handleLetterSequences(Expression *expr)
{
    Symbol *oversizedSymbolArray = new Symbol[expr->length];
    int resultLength = 0;

    bool currentIsLetter = false;
    char digitsBuffer[MAX_VARIABLE_NAME_LEN];
    int currentVariableLength = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (currentVariableLength > MAX_VARIABLE_NAME_LEN)
        {
            printf("too long variable name\n");
        }

        if (expr->symbols[symbolIndex].type == VARIABLE or (expr->symbols[symbolIndex].type == NUMBER and currentIsLetter))
        {
            if (currentIsLetter)
            {
                if (expr->symbols[symbolIndex].type == VARIABLE)
                    digitsBuffer[currentVariableLength] = expr->symbols[symbolIndex].entity.variable[0];
                if (expr->symbols[symbolIndex].type == NUMBER)
                    digitsBuffer[currentVariableLength] = expr->symbols[symbolIndex].entity.number + '0';
                currentVariableLength++;
            }
            else
            {
                currentIsLetter = true;
                symbolIndex--;
            }
        }
        else
        {
            Symbol newSymbol;
            if (currentVariableLength > 0)
            {
                //create symbol
                newSymbol.type = VARIABLE;
                strncpy(newSymbol.entity.variable, digitsBuffer, currentVariableLength);
                newSymbol.entity.variable[currentVariableLength] = '\0';
                // print(newSymbol);

                //reset cycle
                currentVariableLength = 0;
                currentIsLetter = false;

                //add to array
                oversizedSymbolArray[resultLength] = newSymbol;
                resultLength++;
            }
            newSymbol = expr->symbols[symbolIndex];
            oversizedSymbolArray[resultLength] = newSymbol;
            resultLength++;
        }
    }

    if (currentVariableLength > 0)
    {
        Symbol newSymbol;

        //create symbol
        newSymbol.type = VARIABLE;
        strncpy(newSymbol.entity.variable, digitsBuffer, currentVariableLength);
        newSymbol.entity.variable[currentVariableLength] = '\0';

        //add to array
        oversizedSymbolArray[resultLength] = newSymbol;
        resultLength++;
    }

    Expression *result = new Expression;
    result->length = resultLength;
    result->symbols = new Symbol[resultLength];
    for (int i = 0; i < resultLength; i++)
    {
        result->symbols[i] = oversizedSymbolArray[i];
    }
    return result;
}

Expression *handleUnaryMinus(Expression *expr)
{
    int unaryOperatorsCount = 0;
    bool isUnaryOperatorLUT[expr->length];
    for (int i = 0; i < expr->length; i++)
    {
        bool isUnary = false;
        if (expr->symbols[i].type == OPERATOR && expr->symbols[i].entity.operator_ == '-')
        {
            if (i > 0)
            {
                if (expr->symbols[i - 1].type != NUMBER and expr->symbols[i - 1].type != VARIABLE and expr->symbols[i - 1].type != CLOSING_BRACKET)
                {
                    isUnary = true;
                }
            }
            else
            {
                isUnary = true;
            }
        }
        if (isUnary)
        {
            isUnaryOperatorLUT[i] = true;
            unaryOperatorsCount++;
            // printf("unary op pos: %d\n", i);
        }
        else
        {
            isUnaryOperatorLUT[i] = false;
        }
    }

    //this        : -
    //becomes this: (-1)*
    int withUnaryLength = expr->length + unaryOperatorsCount * 3;
    Symbol *withUnary = new Symbol[withUnaryLength];
    int writeIndex = 0;
    for (int i = 0; i < expr->length; i++)
    {
        if (isUnaryOperatorLUT[i])
        {
            withUnary[writeIndex].type = OPENING_BRACKET;
            withUnary[writeIndex].entity.bracket = '(';

            withUnary[writeIndex + 1].type = NUMBER;
            withUnary[writeIndex + 1].entity.number = -1;

            withUnary[writeIndex + 2].type = CLOSING_BRACKET;
            withUnary[writeIndex + 2].entity.bracket = ')';

            withUnary[writeIndex + 3].type = OPERATOR;
            withUnary[writeIndex + 3].entity.operator_ = '*';

            writeIndex += 4;
        }
        else
        {
            withUnary[writeIndex] = expr->symbols[i];
            writeIndex++;
        }
    }
    // printf("wi = %d; wul = %d\n", writeIndex, withUnaryLength);

    Expression *result = new Expression;
    result->length = withUnaryLength;
    result->symbols = withUnary;
    // printf("expr:");
    // print(result);
    // printf("\n");
    return result;
}

Expression *strToExpr(char *str)
{
    Expression *expr = new Expression;
    expr->length = strlen(str);
    if (expr->length > EXPR_MAX_LEN)
    {
        printf("expression is longer than it can be\n");
    }

    int spacesCounter = 0;
    for (int i = 0; i < expr->length; i++)
    {
        if (str[i] == ' ')
        {
            spacesCounter++;
        }
    }

    expr->symbols = new Symbol[expr->length - spacesCounter];
    int writeIndex = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (str[symbolIndex] != ' ')
        {
            expr->symbols[writeIndex] = charToSymbol(str[symbolIndex]);
            if (expr->symbols[writeIndex].type == NOT_A_SYMBOL)
            {
                printf("%c is unknown symbol. it may cause problems.\n", str[symbolIndex]);
            }
            writeIndex++;
        }
    }

    // printf("spaces: %d\n", spacesCounter);
    expr->length -= spacesCounter;
    // printf("raw\n");
    // print(expr);
    // printf("\n");
    expr = handleLetterSequences(expr);
    // printf("letters\n");
    // print(expr);
    // printf("\n");
    expr = handleNumberSequences(expr);
    // printf("numbers\n");
    // print(expr);
    // printf("\n");
    expr = handleUnaryMinus(expr);
    return expr;
}

Expression *slice(Expression *expr, int a, int b)
{
    Expression *result = new Expression;
    result->length = b - a;
    result->symbols = new Symbol[result->length];
    int k = 0;
    for (int i = a; i < b; i++)
    {
        result->symbols[k] = expr->symbols[i];
        k++;
    }
    return result;
}

void setPriorities(Expression *expr)
{
    unsigned int nestLevel = 0;
    for (int i = 0; i < expr->length; i++)
    {
        if (expr->symbols[i].type == OPENING_BRACKET)
        {
            nestLevel += 10;
            expr->symbols[i].priority = -nestLevel;
        }
        else if (expr->symbols[i].type == CLOSING_BRACKET)
        {
            nestLevel -= 10;
            expr->symbols[i].priority = -nestLevel;
        }
        else if (expr->symbols[i].type == OPERATOR)
        {
            expr->symbols[i].priority = nestLevel + priority(expr->symbols[i].entity.operator_);
        }
        else
        {
            expr->symbols[i].priority = nestLevel;
        }
    }
}

unsigned int prioritizedOperatorIndex(Expression *expr)
{
    setPriorities(expr);
    int minPriority = 10000000;
    int priorOpIndex = PRIORITIZED_OPERATOR_NOT_FOUND;
    unsigned int nestLevel = 0;
    for (int i = 0; i < expr->length; i++)
    {

        if (expr->symbols[i].type == OPENING_BRACKET)
        {
            nestLevel++;
        }
        else if (expr->symbols[i].type == CLOSING_BRACKET)
        {
            nestLevel--;
        }
        if (expr->symbols[i].type == OPERATOR && expr->symbols[i].priority <= minPriority && nestLevel == 0)
        {
            minPriority = expr->symbols[i].priority;
            priorOpIndex = i;
        }
    }
    return priorOpIndex;
}

Expression *strip(Expression *expr)
{
    bool stripLeft = false;
    if (expr->symbols[0].type == OPENING_BRACKET || expr->symbols[0].type == CLOSING_BRACKET)
    {
        stripLeft = true;
    }

    bool stripRight = false;
    if (expr->symbols[expr->length - 1].type == OPENING_BRACKET || expr->symbols[expr->length - 1].type == CLOSING_BRACKET)
    {
        stripRight = true;
    }

    if (stripLeft && stripRight)
    {
        expr = slice(expr, 1, expr->length - 1);
    }
    else if (stripLeft)
    {
        expr = slice(expr, 1, expr->length);
    }
    else if (stripRight)
    {
        expr = slice(expr, 0, expr->length - 1);
    }

    return expr;
}
