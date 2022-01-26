echo off
del variables.txt
del variables
del build.exe
g++ -Wall -O2 *.cpp -o build.exe
build.exe