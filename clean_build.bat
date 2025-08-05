@echo off
echo ========================================
echo    Cleaning and Rebuilding TNTsTemplate
echo ========================================
echo.

:: Remove existing build directory
if exist "build" (
    echo Removing existing build directory...
    rmdir /s /q build
)

:: Create new build directory
echo Creating new build directory...
mkdir build
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
    echo Please check the error messages above.
    echo Common issues:
    echo - Missing Visual Studio installation
    echo - CMake not in PATH
    echo - Missing source files
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
    echo.
)

:: Return to original directory
cd ..

echo.
echo Clean build process completed.
pause