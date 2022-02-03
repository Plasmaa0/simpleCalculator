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


if "%1"=="r" (
    echo building...
    g++ -Wall -O0 *.cpp -o build.exe
    echo build OK!
    echo running...
    build.exe
) else (
    if "%1"=="d" (
        set DEBUG=1
        echo building...
        g++ -Wall -g3 -O0 *.cpp -o build.exe
        echo build OK!
        echo running GDB...
        gdb build.exe
        set DEBUG
    ) else (
        echo building...
        g++ -Wall -O0 *.cpp -o build.exe
        echo build OK!
        echo done
    )
)

