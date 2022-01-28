echo off

if exist test\got.txt (
    del test\got.txt
)
if exist variables.txt (
    del variables.txt
)
if exist variables (
    del variables
)
if exist build.exe (
    del build.exe
)
if exist test\compare.exe (
    del test\compare.exe
)

echo building...

@REM g++ -Wall -g -O0 compare.cpp ..\BET.cpp ..\calculator.cpp ..\dictionary.cpp ..\expression.cpp ..\function.cpp ..\input.cpp ..\number.cpp ..\output.cpp ..\symbol.cpp ..\util.cpp -o tests.exe
g++ -O0 *.cpp -o build.exe

cd test
g++ compare.cpp -o compare.exe
cd ../

echo build OK!

echo getting output...
build.exe > "test\got.txt"

echo comparing...
cd test
compare.exe
cd ../

@REM if exist test\got.txt (
@REM     del test\got.txt
@REM )
@REM if exist variables.txt (
@REM     del variables.txt
@REM )
@REM if exist variables (
@REM     del variables
@REM )
@REM if exist build.exe (
@REM     del build.exe
@REM )
@REM if exist test\compare.exe (
@REM     del test\compare.exe
@REM )