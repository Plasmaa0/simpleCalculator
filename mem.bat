echo off
g++ -Wall -g3 -ggdb *.cpp -o build.exe
drmemory -ignore_kernel build.exe