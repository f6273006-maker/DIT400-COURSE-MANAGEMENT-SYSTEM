@echo off
REM Build script for DIT400 Course Management System (C++)

REM Compile using MinGW g++
g++ -std=c++11 -O2 -o cms_cpp.exe main.cpp

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo Build successful! Run the program with: cms_cpp.exe
