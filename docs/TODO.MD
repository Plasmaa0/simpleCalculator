# Objects that are used in project

## Structures and their lifecycle
- **BET - Binary Expression Tree**
  - Evaluating an expression
    - exprToBET(...) generates BET
    - BET evaluates
  
  - Evaluating a function
    - exprToBET(strToExpr(funcCall.body)) generates BET
    - BET evaluates


- **VariableDictionary**
  - All the calculator lifecycle
    - Creates with createVariableDictionary(size) on init
    - Used for storing global variables in any moment

  - Function evaluation
    - Creates when function begins to evaluate
    - Passes as arg to eval with BET of function to eval(...)


- **Expression**
  - Globally
    - strToExpr generates expression
    - expression gets converted to BET
  - Inside strToExpr
    - 
- **Function**
- **FunctionDictionary**
- **NumberContainer** - union
- **Number**
- **SymbolContainer** - union
- **Symbol**
- **functionCall**

## Enumerators
- **EExpressionType**
- **ENumberType**
- **ESymbolType**

## all the functions
- BETNode *createNode();
- void insert(BETNode *root, Symbol *s);
- bool eval(BETNode *root, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);
- bool evaluateFunctionCall(Symbol *functionCallSymbol, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);
- BETNode *exprToAET(Expression *expr, int nestLevel);        
- BETNode *exprToAET(Expression *expr);
- bool eval(char *str, VariableDictionary *dict, FunctionDictionary *fdict, Number &result);
- EExpressionType recognizeExpressionType(char *expr);        
- void deleteSpaces(char *expr);
- int equalsSignIndex(char *expr);
- bool hasCompoundAssignment(char *expr);
- bool isCorrectVariableName(char *var);
- char getCompoundOperator(char *expr);
- void CalculatorInit(unsigned int dictionarySize, char *filename = nullptr);
- void smartLineNumberPrint(char *expr, int lineNumber);      
- VariableDictionary *createVariableDictionary(unsigned int size);
- void setVariable(char *variableName, Number number, VariableDictionary *dict);
- bool getVariable(char *variableName, VariableDictionary *dict, Number &num);
- void print(Expression *expr);
- Expression *strToExpr(char *str);
- Expression *slice(Expression *expr, int a, int b);
- Expression *strip(Expression *expr);
- unsigned int prioritizedOperatorIndex(Expression *expr);    
- void setPriorities(Expression *expr);
- Expression *handleNumberSequences(Expression *expr);        
- Expression *handleLetterSequences(Expression *expr);        
- Expression *handleUnaryMinus(Expression *expr);
- Expression *deleteNAS(Expression *expr);
- Expression *handleFunctions(Expression *expr);
- Function *createFunction(char *paramsAsString, char *body); 
- bool evaluateFunction(Number *args, unsigned int argsN, Function *func, FunctionDictionary *fdict, Number &result);     
- void print(FunctionDictionary *dict);
- FunctionDictionary *createFunctionDictionary(unsigned int size);
- void addFunction(char *funcName, Function func, FunctionDictionary *dict);
- bool getFunction(char *funcName, FunctionDictionary *dict, Function &func);
- VariableDictionary *loadDictionary(unsigned int maxSize);   
- void print(Number n, bool printType = false);
- void tabulate(int n);
- void pre_order(const BETNode *root);
- void in_order(const BETNode *root);
- void post_order(const BETNode *root);
- void prettyPrint(BETNode *node);
- void print(const BETNode *root);
- void print(VariableDictionary *dict);
- void saveDictionary(VariableDictionary *dict, bool asBinary = false);
- Symbol charToSymbol(char ch);
- ESymbolType recognizeSymbol(char symbol);
- unsigned int priority(char symb);
- void print(Symbol symb, bool brackets = false);
- bool solve(Number a, Number b, char op, Number &result);    
- double naturalPow(double a, int b);
- Number numberFromDigits(int *digitsArray, int digitsN, int *decimalPart, int decimalPartLength);