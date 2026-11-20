@echo off
chcp 65001 > nul
rem ============================================================
rem  Build script for LR_2.
rem  Requires g++ with C++17 support (MinGW).
rem  Output: lr2.exe
rem
rem  NOTE: this file is ASCII-only on purpose. Russian text in a
rem  .bat file breaks parsing after "chcp 65001".
rem ============================================================

g++ -std=c++17 -O2 -Wall -Wextra -o lr2.exe core.cpp algo.cpp main.cpp

if errorlevel 1 (
    echo.
    echo BUILD FAILED
    exit /b 1
)

echo.
echo Build OK: lr2.exe
echo.
echo Examples:
echo   lr2.exe 1 data\net.txt
echo   lr2.exe 2 data\map_big.txt -a 1 1 -b 20 20
echo   lr2.exe 3 data\bip.txt
echo   lr2.exe            (menu)
