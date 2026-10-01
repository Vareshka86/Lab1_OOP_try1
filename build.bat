@echo off
chcp 65001 > nul
rem Сборка лабораторной работы №1 компилятором g++ (MinGW-w64).
rem Результат: build\lab1.exe
rem Стандарт языка - C++14 (требование преподавателя): флаг -std=c++14.
rem Флаг -static встраивает библиотеки MinGW (libstdc++ и др.) прямо в exe,
rem поэтому программа запускается и на компьютере, где MinGW не установлен.

if not exist build mkdir build

g++ -std=c++14 -Wall -Wextra -pedantic -static -Iinclude ^
    src\main.cpp src\input.cpp src\task1.cpp ^
    -o build\lab1.exe

if errorlevel 1 (
    echo Сборка завершилась с ошибкой.
    exit /b 1
)
echo Сборка успешна: build\lab1.exe
