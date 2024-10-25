@echo off
setlocal

rem Check if g++ is installed
where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: g++ compiler not found or not set up correctly.
    echo Please install g++ and make sure it's in your PATH.
    exit /b 1
)

set CXX=g++
set CXXFLAGS=-std=c++17 -Iinclude
set OUTDIR=build

rem Create build directory if it doesn't exist
if not exist %OUTDIR% (
    mkdir %OUTDIR%
)

rem Compile the source files and output to build directory
%CXX% %CXXFLAGS% -o %OUTDIR%\run src\*.cpp

if %errorlevel% neq 0 (
    echo Build failed!
) else (
    echo Build successful! Executable is located in the 'build' directory.
)

endlocal
