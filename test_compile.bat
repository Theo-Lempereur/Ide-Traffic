@echo off
echo Initializing Visual Studio environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

echo.
echo Testing compiler...
cl /?

echo.
echo Attempting simple compilation test...
echo #include ^<iostream^> > test.cpp
echo int main() { std::cout ^<^< "Hello World!" ^<^< std::endl; return 0; } >> test.cpp

cl /EHsc test.cpp /Fe:test.exe
if %errorlevel% == 0 (
    echo Compilation successful!
    test.exe
    del test.cpp test.exe test.obj
) else (
    echo Compilation failed!
)

pause