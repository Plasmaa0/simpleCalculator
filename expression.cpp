#include "expression.h"

Expression *handleNumberSequences(Expression *expr)
{
    Symbol *oversizedSymbolArray = new Symbol[expr->length];
    int resultLength = 0;

    bool currentIsNumber = false;
    bool decimalPart = false;

    int digitsBuffer[constants::MAX_NUMBER_LENGTH];
    int currentNumberLength = 0;

    int decimalPartBuffer[constants::MAX_NUMBER_LENGTH];
    int decimalPartLength = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (expr->symbols[symbolIndex].type == ESymbolType::DECIMAL_COMMA)
        {
            // printf("comma index: %d\n", symbolIndex);
            decimalPart = true;
            currentIsNumber = true;
            continue;
        }

        if (expr->symbols[symbolIndex].type == ESymbolType::NUMBER)
        {
            if (currentIsNumber)
            {
                if (decimalPart)
                {
                    decimalPartBuffer[decimalPartLength] = expr->symbols[symbolIndex].entity.number.value.integer;
                    // printf("|%d|", decimalPartBuffer[decimalPartLength]);
                    decimalPartLength++;
                }
                else
                {
                    digitsBuffer[currentNumberLength] = expr->symbols[symbolIndex].entity.number.value.integer;
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
                Number number = numberFromDigits(digitsBuffer, currentNumberLength, decimalPartBuffer, decimalPartLength);
                // printf("%d", number);

                //create symbol
                newSymbol.type = ESymbolType::NUMBER;
                newSymbol.entity.number = number;

                //reset cycle
                memset(digitsBuffer, 0, constants::MAX_NUMBER_LENGTH * sizeof(int));
                memset(decimalPartBuffer, 0, constants::MAX_NUMBER_LENGTH * sizeof(int));
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
        Number number = numberFromDigits(digitsBuffer, currentNumberLength, decimalPartBuffer, decimalPartLength);
        // printf("%d", number);

        //create symbol
        newSymbol.type = ESymbolType::NUMBER;
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
    char digitsBuffer[constants::MAX_VARIABLE_NAME_LEN];
    int currentVariableLength = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (currentVariableLength > constants::MAX_VARIABLE_NAME_LEN)
        {
            printf("too long variable name\n");
        }

        if (expr->symbols[symbolIndex].type == ESymbolType::VARIABLE or (expr->symbols[symbolIndex].type == ESymbolType::NUMBER and currentIsLetter))
        {
            if (currentIsLetter)
            {
                if (expr->symbols[symbolIndex].type == ESymbolType::VARIABLE)
                    digitsBuffer[currentVariableLength] = expr->symbols[symbolIndex].entity.variable[0];
                if (expr->symbols[symbolIndex].type == ESymbolType::NUMBER)
                    digitsBuffer[currentVariableLength] = expr->symbols[symbolIndex].entity.number.value.integer + '0';
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
            //must match start
            if (currentVariableLength > 0)
            {
                //create symbol
                newSymbol.type = ESymbolType::VARIABLE;
                // printf("2\n");
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
            //must match end
            newSymbol = expr->symbols[symbolIndex];
            oversizedSymbolArray[resultLength] = newSymbol;
            resultLength++;
        }
    }

    //must match start
    if (currentVariableLength > 0)
    {
        Symbol newSymbol;

        //create symbol
        newSymbol.type = ESymbolType::VARIABLE;
        // printf("1\n");
        strncpy(newSymbol.entity.variable, digitsBuffer, currentVariableLength);
        newSymbol.entity.variable[currentVariableLength] = '\0';

        //add to array
        oversizedSymbolArray[resultLength] = newSymbol;
        resultLength++;
    }
    //must match end

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
        if (expr->symbols[i].type == ESymbolType::OPERATOR && (expr->symbols[i].entity.operator_ == '-' || expr->symbols[i].entity.operator_ == '+'))
        {
            if (i > 0)
            {
                if (expr->symbols[i - 1].type != ESymbolType::NUMBER and expr->symbols[i - 1].type != ESymbolType::VARIABLE and expr->symbols[i - 1].type != ESymbolType::CLOSING_BRACKET)
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
            withUnary[writeIndex].type = ESymbolType::OPENING_BRACKET;
            withUnary[writeIndex].entity.bracket = '(';

            withUnary[writeIndex + 1].type = ESymbolType::NUMBER;
            withUnary[writeIndex + 1].entity.number.type = EnumberType::INTEGER;
            if (expr->symbols[i].entity.operator_ == '-')
            {
                //unary minus
                withUnary[writeIndex + 1].entity.number.value.integer = -1;
            }
            else
            {
                //unary plus
                withUnary[writeIndex + 1].entity.number.value.integer = 1;
            }

            withUnary[writeIndex + 2].type = ESymbolType::CLOSING_BRACKET;
            withUnary[writeIndex + 2].entity.bracket = ')';

            withUnary[writeIndex + 3].type = ESymbolType::OPERATOR;
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

Expression *deleteNAS(Expression *expr)
{
    int NASCount = 0;
    for (int i = 0; i < expr->length; i++)
    {
        if (expr->symbols[i].type == ESymbolType::NOT_A_SYMBOL)
        {
            NASCount++;
        }
    }

    Expression *result = new Expression;
    result->length = expr->length - NASCount;
    result->symbols = new Symbol[result->length];
    int writeIndex = 0;
    for (int i = 0; i < expr->length; i++)
    {
        if (expr->symbols[i].type != ESymbolType::NOT_A_SYMBOL)
        {
            result->symbols[writeIndex] = expr->symbols[i];
            writeIndex++;
        }
    }
    // if (writeIndex != result->length)
    // {
    //     printf("wrffdamfoga\n");
    // }
    return result;
}

Expression *handleFunctions(Expression *expr)
{
    bool hasFunctionCalls = false;
    //setting the priority of evaluating every function
    int maxPriority = 0;
    {
        int priority = 0;
        for (int symbolIndex = 0; symbolIndex < expr->length - 2; symbolIndex++)
        {
            if (expr->symbols[symbolIndex].type == ESymbolType::OPENING_BRACKET)
            {
                priority += 1;
            }
            if (expr->symbols[symbolIndex].type == ESymbolType::CLOSING_BRACKET)
            {
                priority -= 1;
            }
            if (expr->symbols[symbolIndex].type == ESymbolType::VARIABLE and expr->symbols[symbolIndex + 1].type == ESymbolType::OPENING_BRACKET)
            {
                if (priority > maxPriority)
                {
                    maxPriority = priority;
                }
                hasFunctionCalls = true;
                expr->symbols[symbolIndex].priority = priority;
                // printf("function recognized: {%s}. priority=%d\n", expr->symbols[symbolIndex].entity.variable, priority);
            }
            else
            {
                expr->symbols[symbolIndex].priority = -1;
            }
        }
    }

    if (hasFunctionCalls)
    {
        //pick the function with max priority and parse it's parameters
        // printf("pr:\n");
        for (int priority = maxPriority; priority >= 0; priority--)
        {
            int funcIndex;
            Symbol func;
            func.type = ESymbolType::FUNCTION_CALL;
            // func.entity.functionCall = new struct functionCall;
            // printf("1\n");
            // printf("%d", func.entity.functionCall);
            // func.entity.functionCall->argsN = 0;
            // printf("!%d!\n", func.entity.functionCall->argsN);
            bool lookingForArgs = false;
            for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
            {
                ESymbolType currentSymbolType = expr->symbols[symbolIndex].type;
                if ((not lookingForArgs) and expr->symbols[symbolIndex].priority == priority)
                {
                    //reset funnctionCall
                    func.entity.functionCall = new struct functionCall;
                    func.entity.functionCall->argsN = 0;

                    funcIndex = symbolIndex;
                    lookingForArgs = true;
                    strcpy(func.entity.functionCall->functionName, expr->symbols[symbolIndex].entity.variable);
                    // printf("started construction of %s on index %d\n", func.entity.functionCall->functionName, symbolIndex);
                    continue;
                }

                if (lookingForArgs)
                {
                    if (currentSymbolType == ESymbolType::NUMBER or currentSymbolType == ESymbolType::VARIABLE or currentSymbolType == ESymbolType::FUNCTION_CALL)
                    {
                        // printf("arg: ");
                        // print(expr->symbols[symbolIndex]);
                        // printf("\n");
                        func.entity.functionCall->args[func.entity.functionCall->argsN] = expr->symbols[symbolIndex];
                        func.entity.functionCall->argsN++;
                    }
                    else if (currentSymbolType == ESymbolType::CLOSING_BRACKET)
                    {
                        // printf(".\n");
                        expr->symbols[funcIndex] = func;
                        expr->symbols[funcIndex + 1].type = ESymbolType::NOT_A_SYMBOL; //opening bracket
                        expr->symbols[symbolIndex].type = ESymbolType::NOT_A_SYMBOL;   //closing bracket

                        // printf("constructed function: ");
                        // print(func);
                        // printf("\n");
                        // print(expr);
                        lookingForArgs = false;
                    }
                    /**
                     * 
                     * TODO: сделать чтобы functionCall не менялся на Not_A_symbol
                     * 
                     */
                    // if (currentSymbolType != ESymbolType::FUNCTION_CALL)
                    // print(expr->symbols[symbolIndex]);
                    // printf("-> NAS\n");
                    expr->symbols[symbolIndex].type = ESymbolType::NOT_A_SYMBOL;
                    continue;
                }
            }
        }
        expr = deleteNAS(expr);
        // printf("parsed function calls: ");
        // print(expr);
    }
    return expr;
}

Expression *createExpr()
{
    Expression *res = new Expression;
    if (res == nullptr)
    {
        printf("WTF \nWTF \nWTF \nWTF \nWTF \nWTF \nWTF \nWTF \nWTF \n");
    }
    res->length = 0;
    res->symbols = nullptr;
    return res;
}

Expression *strToExpr(char *str)
{
    // malloc();
    Expression *expr = createExpr();
    // Expression *expr = new Expression;

    expr->length = strlen(str);
    // printf("len: %d\n", expr->length);
    if (expr->length > constants::EXPR_MAX_LEN)
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
    // printf("spcs: %d\n", spacesCounter);

    expr->symbols = new Symbol[expr->length - spacesCounter];
    // printf("creating..\n");
    int writeIndex = 0;
    for (int symbolIndex = 0; symbolIndex < expr->length; symbolIndex++)
    {
        if (str[symbolIndex] != ' ')
        {
            expr->symbols[writeIndex] = charToSymbol(str[symbolIndex]);
            // if (expr->symbols[writeIndex].type == ESymbolType::NOT_A_SYMBOL)
            // {
            //     printf("%c is unknown symbol. it may cause problems.\n", str[symbolIndex]);
            // }
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

    expr = handleFunctions(expr);

    return expr;
}

Expression *slice(Expression *expr, int a, int b)
{
    if (b - a == expr->length)
    {
        return expr;
    }
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
        if (expr->symbols[i].type == ESymbolType::OPENING_BRACKET)
        {
            nestLevel += 10;
            expr->symbols[i].priority = -nestLevel;
        }
        else if (expr->symbols[i].type == ESymbolType::CLOSING_BRACKET)
        {
            nestLevel -= 10;
            expr->symbols[i].priority = -nestLevel;
        }
        else if (expr->symbols[i].type == ESymbolType::OPERATOR)
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
    int minPriority = constants::PRIORITY_MAX;
    int priorOpIndex = constants::PRIORITIZED_OPERATOR_NOT_FOUND;
    unsigned int nestLevel = 0;
    for (int i = 0; i < expr->length; i++)
    {

        if (expr->symbols[i].type == ESymbolType::OPENING_BRACKET)
        {
            nestLevel++;
        }
        else if (expr->symbols[i].type == ESymbolType::CLOSING_BRACKET)
        {
            nestLevel--;
        }
        if (expr->symbols[i].type == ESymbolType::OPERATOR && expr->symbols[i].priority <= minPriority && nestLevel == 0)
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
    if (expr->symbols[0].type == ESymbolType::OPENING_BRACKET || expr->symbols[0].type == ESymbolType::CLOSING_BRACKET)
    {
        stripLeft = true;
    }

    bool stripRight = false;
    if (expr->symbols[expr->length - 1].type == ESymbolType::OPENING_BRACKET || expr->symbols[expr->length - 1].type == ESymbolType::CLOSING_BRACKET)
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
