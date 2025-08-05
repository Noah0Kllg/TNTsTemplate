# CMake Build Guide - TNTsTemplate Keyboard Overlay

## 🚀 Quick Start (Your Method)

Yes, your approach is correct! Here's the exact commands you can use:

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

The compiled DLL will be available at: `build/bin/Release/TNTsTemplate.dll`

## 📋 Prerequisites

### Required Software
- **CMake 3.20 or later**
- **Visual Studio 2022** (with C++ workload)
- **Windows 10/11**

### Optional but Recommended
- **Git** (for version control)
- **Visual Studio Code** (for editing)

## 🔧 Installation Steps

### 1. Install CMake
1. Download from: https://cmake.org/download/
2. Choose "Windows x64 Installer"
3. During installation, select "Add CMake to the system PATH"
4. Verify installation: `cmake --version`

### 2. Install Visual Studio 2022
1. Download from: https://visualstudio.microsoft.com/downloads/
2. Install with "Desktop development with C++" workload
3. Ensure "MSVC v143" and "Windows 10/11 SDK" are selected

## 🏗️ Build Methods

### Method 1: Command Line (Recommended)
```bash
# Create build directory
mkdir build
cd build

# Configure project
cmake .. -G "Visual Studio 17 2022" -A x64

# Build in Release mode
cmake --build . --config Release
```

### Method 1b: Using CMake Presets (Easiest)
```bash
# Configure and build in one command
cmake --preset default
cmake --build --preset default

# Or for debug build
cmake --preset debug
cmake --build --preset debug

# Or for optimized release
cmake --preset release
cmake --build --preset release
```

### Method 2: Using the Build Script
```bash
# Run the provided build script
build_cmake.bat
```

### Method 3: Visual Studio IDE
```bash
# Generate Visual Studio solution
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64

# Open the generated .sln file in Visual Studio
# Build using Ctrl+Shift+B or Build menu
```

## 📁 Project Structure

```
TNTsTemplate/
├── CMakeLists.txt              # Main CMake configuration
├── build_cmake.bat             # Build script
├── dllmain.cpp                 # DLL entry point
├── Example.cpp                 # Example module
├── Modules/
│   ├── Mods/
│   │   ├── KeyboardOverlay.cpp # Keyboard overlay implementation
│   │   ├── KeyboardOverlay.hpp # Keyboard overlay header
│   │   └── KeyboardOverlayConfig.hpp # Configuration
│   └── ...
├── Components/
│   └── Components/
│       ├── Core.cpp            # Core functionality
│       ├── GUI.cpp             # ImGui interface
│       └── ...
├── ImGui/                      # ImGui library files
├── Detours/                    # Microsoft Detours library
└── Extensions/                 # Utility extensions
```

## ⚙️ CMake Configuration Options

### Generator Options
```bash
# Visual Studio 2022 (recommended)
cmake .. -G "Visual Studio 17 2022" -A x64

# Visual Studio 2019
cmake .. -G "Visual Studio 16 2019" -A x64

# Ninja (faster builds)
cmake .. -G "Ninja"
```

### Build Configuration
```bash
# Release build (optimized, smaller)
cmake --build . --config Release

# Debug build (with debug info)
cmake --build . --config Debug

# RelWithDebInfo (optimized with debug info)
cmake --build . --config RelWithDebInfo
```

### Custom Options
```bash
# Set custom compiler flags
cmake .. -DCMAKE_CXX_FLAGS="/std:c++20"

# Set custom output directory
cmake .. -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="custom_output"

# Enable verbose output
cmake --build . --config Release --verbose
```

## 🔍 Troubleshooting

### Common Issues

#### 1. "CMake not found"
```bash
# Solution: Install CMake and add to PATH
# Or use full path to cmake.exe
"C:\Program Files\CMake\bin\cmake.exe" ..
```

#### 2. "Visual Studio not found"
```bash
# Solution: Run from Visual Studio Developer Command Prompt
# Or specify generator explicitly
cmake .. -G "Visual Studio 17 2022" -A x64
```

#### 3. "Detours library not found"
```bash
# Solution: Ensure Detours folder exists in project root
# The project expects: ./Detours/detours.lib
```

#### 4. "Compiler errors"
```bash
# Solution: Check Visual Studio installation
# Ensure C++20 support is available
# Update Windows SDK if needed
```

#### 5. "Linker errors"
```bash
# Solution: Ensure all required libraries are available
# Check that ImGui files are present
# Verify Detours library is correct architecture (x64)
```

### Build Verification

After successful build, verify the output:
```bash
# Check if DLL was created
dir build\bin\Release\TNTsTemplate.dll

# Check file size (should be several MB)
# Check file properties (should be x64 architecture)
```

## 🎯 Advanced Configuration

### Custom CMake Options
Add these to your cmake command for customization:

```bash
# Enable debug output
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Set custom C++ standard
cmake .. -DCMAKE_CXX_STANDARD=20

# Enable specific features
cmake .. -DENABLE_KEYBOARD_OVERLAY=ON

# Set custom include paths
cmake .. -DCMAKE_INCLUDE_PATH="custom/path"
```

### IDE Integration

#### Visual Studio Code
1. Install "CMake Tools" extension
2. Open project folder
3. Select kit: "Visual Studio Build Tools 2022 Release - amd64"
4. Configure and build using Ctrl+Shift+P → "CMake: Build"

#### CLion
1. Open project folder
2. CMake will auto-configure
3. Build using Ctrl+F9

## 📦 Distribution

### Creating Release Package
```bash
# Build in Release mode
cmake --build . --config Release

# Create distribution folder
mkdir dist
copy build\bin\Release\TNTsTemplate.dll dist\
copy README.md dist\
copy QUICK_START.md dist\

# Optional: Include dependencies
copy Detours\detours.dll dist\
```

### Install Target
```bash
# Install to system (requires admin)
cmake --build . --config Release --target install

# Install to custom location
cmake --install . --prefix "C:\CustomInstallPath"
```

## 🔄 Continuous Integration

### GitHub Actions Example
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v3
    - name: Configure CMake
      run: cmake -B build -G "Visual Studio 17 2022" -A x64
    - name: Build
      run: cmake --build build --config Release
    - name: Upload Artifacts
      uses: actions/upload-artifact@v3
      with:
        name: TNTsTemplate
        path: build/bin/Release/
```

## 📚 Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [Visual Studio Documentation](https://docs.microsoft.com/en-us/visualstudio/)
- [ImGui Documentation](https://github.com/ocornut/imgui)
- [Microsoft Detours](https://github.com/microsoft/Detours)

## 🆘 Getting Help

If you encounter issues:

1. **Check the error messages** - they often contain the solution
2. **Verify prerequisites** - ensure all software is installed correctly
3. **Try the build script** - `build_cmake.bat` handles common issues
4. **Check file paths** - ensure all source files exist
5. **Update software** - use latest versions of CMake and Visual Studio

---

**Happy building!** 🚗⚽