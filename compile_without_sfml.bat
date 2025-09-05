@echo off
echo Compiling RoadSim project without SFML...

:: Initialize Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

:: Create build directory
if not exist build mkdir build
cd build

echo.
echo Compiling core components...

:: Compile only non-SFML dependent files
cl /EHsc /std:c++20 /I".." /c ..\app\core\Simulator.cpp ..\app\core\Scheduler.cpp ..\app\core\RNG.cpp ..\app\editor\MapEditor.cpp ..\app\editor\EntityEditor.cpp ..\app\io\JsonLoader.cpp ..\app\io\ConfigLoader.cpp ..\app\runtime\ThreadManager.cpp

if %errorlevel% neq 0 (
    echo.
    echo ✗ Compilation failed!
    echo Check the error messages above.
    pause
    exit /b 1
)

echo.
echo ✓ Core components compiled successfully!
echo Note: SFML-dependent components (Window, Renderer, Application, main) were skipped.
echo Install SFML to compile the complete project.

cd ..
pause