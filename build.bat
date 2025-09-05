@echo off
echo Testing compilation environment...
echo.

REM Check for Visual Studio Build Tools
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2019 Build Tools
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    goto :compile
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2022 Community
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto :compile
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2022 Professional
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto :compile
)

REM Check for MinGW
where gcc >nul 2>&1
if %errorlevel% == 0 (
    echo Found MinGW/GCC
    goto :compile_gcc
)

echo No suitable compiler found!
echo Please install one of the following:
echo - Visual Studio 2019/2022 with C++ tools
echo - MinGW-w64
echo - CMake
pause
exit /b 1

:compile
echo Using MSVC compiler...
REM Create a simple test compilation
cl /EHsc /std:c++20 /I. main.cpp /Fe:test.exe
if %errorlevel% == 0 (
    echo Compilation successful!
    echo Running test...
    test.exe
) else (
    echo Compilation failed!
)
goto :end

:compile_gcc
echo Using GCC compiler...
gcc -std=c++20 -I. main.cpp -o test.exe
if %errorlevel% == 0 (
    echo Compilation successful!
    echo Running test...
    test.exe
) else (
    echo Compilation failed!
)
goto :end

:end
pause