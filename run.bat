echo off
del variables.txt
del variables
del build.exe
g++ -Wall -O0 main.cpp expression.cpp symbol.cpp dictionary.cpp BET.cpp util.cpp output.cpp input.cpp calculator.cpp -o build.exe
build.exe