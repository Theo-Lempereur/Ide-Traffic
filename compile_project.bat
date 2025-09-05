@echo off
echo Initializing Visual Studio Enterprise environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

echo.
echo Compiling RoadSim project...
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Set SFML directory to local installation
set SFML_DIR=%~dp0SFML-2.6.1

if not exist "%SFML_DIR%" (
    echo SFML directory not found at %SFML_DIR%
    echo Please ensure SFML is properly extracted.
    pause
    exit /b 1
)

echo Using SFML from: %SFML_DIR%

REM Compile all source files
echo Compiling source files...
cl /EHsc /std:c++20 /I".." /I"%SFML_DIR%\include" /c ..\app\core\Simulator.cpp ..\app\core\Scheduler.cpp ..\app\core\RNG.cpp ..\app\core\GameObject.cpp ..\app\core\Transform.cpp ..\app\core\Collider.cpp ..\app\core\Scene.cpp ..\app\editor\MapEditor.cpp ..\app\editor\EntityEditor.cpp ..\app\render\Window.cpp ..\app\render\Renderer.cpp ..\app\render\UIManager.cpp ..\app\io\JsonLoader.cpp ..\app\io\ConfigLoader.cpp ..\app\runtime\ThreadManager.cpp ..\app\runtime\Application.cpp ..\app\main.cpp

if %errorlevel% neq 0 (
    echo.
    echo ✗ Compilation failed!
    echo Check the error messages above.
    pause
    exit /b 1
)

echo.
echo Linking with SFML libraries...
link *.obj /OUT:RoadSim.exe /LIBPATH:"%SFML_DIR%\lib" sfml-graphics.lib sfml-window.lib sfml-system.lib user32.lib gdi32.lib winmm.lib opengl32.lib

if %errorlevel% == 0 (
    echo.
    echo ✓ Build successful!
    echo Executable: build\RoadSim.exe
    echo.
    echo Copying SFML DLLs...
    copy "%SFML_DIR%\bin\*.dll" .
) else (
    echo.
    echo ✗ Linking failed!
    echo Check the error messages above.
)

cd ..
pause