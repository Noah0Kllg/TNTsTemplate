@echo off
echo ========================================
echo    Rocket League Keyboard Overlay
echo    Build Script for TNTsTemplate
echo ========================================
echo.

echo Checking for Visual Studio...
where msbuild >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: MSBuild not found in PATH
    echo Please ensure Visual Studio is installed and MSBuild is available
    echo You may need to run this from a Visual Studio Developer Command Prompt
    pause
    exit /b 1
)

echo Building project in Release x64 configuration...
msbuild tntsTemplate.sln /p:Configuration=Release /p:Platform=x64 /m

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo The keyboard overlay has been built successfully.
    echo Look for the compiled DLL in the Release folder.
    echo.
    echo To use the overlay:
    echo 1. Inject the DLL into Rocket League
    echo 2. The overlay will appear automatically
    echo 3. Use F1 to toggle the overlay on/off
    echo 4. Access settings through the mod's GUI
    echo.
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
    echo - Missing dependencies
    echo.
)

pause