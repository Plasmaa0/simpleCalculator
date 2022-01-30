[![CodeQL](https://github.com/Plasmaa0/simpleCalculator/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/Plasmaa0/simpleCalculator/actions/workflows/codeql-analysis.yml)
# Simple Interpreting Calculator Project

## Requirements
 - g++ compiler

## To build and run project:
```
run.bat
```

## Demo
<img src="docs/example1.gif" alt="example1" width="300" height="500"/>

## Algorithm
1. Get the expression as a string.
   - If string contains assignment operation then split it to variable and expression to evaluate. ('x=1+2' -> variable to assign a value to: 'x' and value to be assigned '1+2' (will be evaluated in next steps))
2. Convert string to array of special symbols (operators, operands, brackets, etc).
   1. Handle sequences of numbers to get numbers with multiple digits and number with floating point.
   2. Handle sequences of letters to get variables with multiple digits including numbers (like 'var1').
   3. Handle unary operators by converting them to binary operators (replace '-x' with '(-1)*x').
3. Convert array of special symbols to BET (binary expression tree) ![BET](docs/example2.png)
   - If current expression is length of 1, then add it to root.
   - Else find the last operator with the minimum priority and add it to root. Then add to root the result of recursive call of the same function on the expression on the left and right side of this operator.
4. Evaluate binary BET from bottom to top.
   - If the node is number or variable, then return it's value.
   - Else (the node is operator) return the result of recursive call of the same function on the right and left child.

## Main features
- Types: floating point, integer, nan
- Basic binary operations: - + * /
- Unary + and -
- Multiple digits numbers. ex: 123
- Multiple digits variables (including numbers on non-first position). ex: length1
- Floating point number. 3.14 or .15
- Positive integer powers: 2^3 -> 8
- Space ignoring in expressions for better user experience.
- Invalid result handling
- Syntax errors handling

## Other features

### See the result of previous evaluation with reserved underscore '_' variable
```
3*4 + 2^5
> 44.000000
_
> 44.000000
```

### Display current variables with their values with 'vars' command
```
x = 1
> 1.000000
y = 2
> 2.000000
z = x*y^2
> 4.000000
vars
> VARIABLES:
>> _ = 4.00
>> x = 1.00
>> y = 2.00
>> z = 4.00
> END
```

### Save current variables in text or binary file. Load previously saved variables if binary file with them exists. Use keywords 'save', 'saveB', 'load'.
### Save and load example:
```
x = 1
> 1.000000
y = 2
> 2.000000
saveB
> successfully saved 3 variables
x = 0
> 0.000000
y = 0
> 0.000000
load
> successfully loaded 3 variables
x
> 1.000000
y
> 2.000000
```


### Close the application with 'exit'
```
exit
> bye
```
