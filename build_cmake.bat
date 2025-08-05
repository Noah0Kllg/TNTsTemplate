@echo off
echo ========================================
echo    TNTsTemplate CMake Build Script
echo    Rocket League Keyboard Overlay
echo ========================================
echo.

:: Check if CMake is available
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake and ensure it's in your PATH
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

:: Check if Visual Studio is available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo WARNING: Visual Studio compiler not found in PATH
    echo You may need to run this from a Visual Studio Developer Command Prompt
    echo Or use the Visual Studio generator explicitly
    echo.
)

:: Create build directory
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

:: Change to build directory
cd build

:: Configure with CMake
echo Configuring project with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo    CONFIGURATION FAILED!
    echo ========================================
    echo.
    echo Common solutions:
    echo 1. Run from Visual Studio Developer Command Prompt
    echo 2. Install Visual Studio 2022 with C++ workload
    echo 3. Install CMake 3.20 or later
    echo 4. Check that all source files exist
    echo.
    pause
    exit /b 1
)

:: Build the project
echo.
echo Building project in Release configuration...
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo The keyboard overlay has been built successfully.
    echo Output location: build\bin\Release\TNTsTemplate.dll
    echo.
    echo To use the overlay:
    echo 1. Inject TNTsTemplate.dll into Rocket League
    echo 2. The overlay will appear automatically
    echo 3. Use F1 to toggle the overlay on/off
    echo 4. Access settings through the mod's GUI
    echo.
    
    :: Show the output file
    if exist "bin\Release\TNTsTemplate.dll" (
        echo Build output found at:
        echo %CD%\bin\Release\TNTsTemplate.dll
        echo.
        echo File size: 
        for %%A in ("bin\Release\TNTsTemplate.dll") do echo %%~zA bytes
    )
    
) else (
    echo.
    echo ========================================
    echo    BUILD FAILED!
    echo ========================================
    echo.
    echo Please check the error messages above.
    echo Common issues:
    echo - Missing Visual Studio installation
    echo - Incorrect project configuration
    echo - Missing dependencies (Detours, etc.)
    echo - Compiler errors in source code
    echo.
)

:: Return to original directory
cd ..

echo.
echo Build process completed.
pause