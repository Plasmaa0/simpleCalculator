echo off

if exist variables.txt (
    del variables.txt
)
if exist variables (
    del variables
)
if exist build.exe (
    del build.exe
)

echo building...

g++ -Wall -g -O0 *.cpp -o build.exe

echo build OK!

if "%1"=="r" (
    echo running...
    build.exe
) else (
    if "%1"=="d" (
        echo running GDB...
        gdb build.exe
    ) else (
        echo done
    )
)

