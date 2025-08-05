cmake_minimum_required(VERSION 3.20)
project(TNTsTemplate VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform-specific settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# CMake options
option(ENABLE_KEYBOARD_OVERLAY "Enable keyboard overlay feature" ON)
option(ENABLE_DEBUG_OUTPUT "Enable debug console output" ON)
option(ENABLE_IMGUI_DEMO "Enable ImGui demo window" OFF)
option(COPY_DEPENDENCIES "Copy dependency DLLs to output directory" ON)

# Print build configuration
message(STATUS "Build Configuration:")
message(STATUS "  ENABLE_KEYBOARD_OVERLAY: ${ENABLE_KEYBOARD_OVERLAY}")
message(STATUS "  ENABLE_DEBUG_OUTPUT: ${ENABLE_DEBUG_OUTPUT}")
message(STATUS "  ENABLE_IMGUI_DEMO: ${ENABLE_IMGUI_DEMO}")
message(STATUS "  COPY_DEPENDENCIES: ${COPY_DEPENDENCIES}")

# Find required packages
find_package(PkgConfig QUIET)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/ImGui)
include_directories(${CMAKE_SOURCE_DIR}/Detours)
include_directories(${CMAKE_SOURCE_DIR}/RLSDK)
include_directories(${CMAKE_SOURCE_DIR}/RLSDK/SDK_HEADERS)

# Source files
set(SOURCES
    dllmain.cpp
    pch.cpp
    Example.cpp
    Modules/Module.cpp
    Modules/Mods/Drawing.cpp
    Modules/Mods/KeyboardOverlay.cpp
    Components/Component.cpp
    Components/Components/Core.cpp
    Components/Components/Console.cpp
    Components/Components/Events.cpp
    Components/Components/GUI.cpp
    Components/Components/GameState.cpp
    Components/Components/Instances.cpp
    Components/Components/Main.cpp
    Components/Components/Manager.cpp
    Extensions/Extensions/Colors.cpp
    Extensions/Extensions/Formatting.cpp
    Extensions/Extensions/Math.cpp
    Extensions/Extensions/Memory.cpp
    Extensions/Extensions/UnrealMemory.cpp
    RLSDK/GameDefines.cpp
)

# Header files
set(HEADERS
    pch.hpp
    Offsets.hpp
    Functions.hpp
    Example.hpp
    Modules/Module.hpp
    Modules/Mods/Drawing.hpp
    Modules/Mods/KeyboardOverlay.hpp
    Modules/Mods/KeyboardOverlayConfig.hpp
    Modules/Includes.hpp
    Components/Component.hpp
    Components/Components/Core.hpp
    Components/Components/Console.hpp
    Components/Components/Events.hpp
    Components/Components/GUI.hpp
    Components/Components/GameState.hpp
    Components/Components/Instances.hpp
    Components/Components/Main.hpp
    Components/Components/Manager.hpp
    Components/Includes.hpp
    Extensions/Includes.hpp
    Extensions/Extensions/Colors.hpp
    Extensions/Extensions/Formatting.hpp
    Extensions/Extensions/Math.hpp
    Extensions/Extensions/Memory.hpp
    Extensions/Extensions/UnrealMemory.hpp
)

# ImGui source files
set(IMGUI_SOURCES
    ImGui/imgui.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    ImGui/imgui_impl_dx11.cpp
    ImGui/imgui_impl_win32.cpp
    ImGui/imgui_stdlib.cpp
)

# Conditionally include ImGui demo
if(ENABLE_IMGUI_DEMO)
    list(APPEND IMGUI_SOURCES ImGui/imgui_demo.cpp)
endif()

# Detours library
if(EXISTS "${CMAKE_SOURCE_DIR}/Detours/detours.lib")
    set(DETOURS_LIB "${CMAKE_SOURCE_DIR}/Detours/detours.lib")
else()
    # Try to find Detours in system
    find_library(DETOURS_LIB NAMES detours)
    if(NOT DETOURS_LIB)
        message(FATAL_ERROR "Detours library not found. Please ensure it's available.")
    endif()
endif()

# Create the DLL
add_library(${PROJECT_NAME} SHARED 
    ${SOURCES}
    ${HEADERS}
    ${IMGUI_SOURCES}
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "TNTsTemplate"
    PREFIX ""
    SUFFIX ".dll"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${DETOURS_LIB}
    d3d11
    dxgi
    d3dcompiler
    d3dx11
    winmm
    psapi
    kernel32
    user32
    gdi32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
)

# Compiler-specific settings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /wd4100  # Unreferenced formal parameter
        /wd4201  # Nonstandard extension used: nameless struct/union
        /wd4996  # Deprecated functions
        /wd4244  # Conversion from 'type1' to 'type2', possible loss of data
        /wd4267  # Conversion from 'size_t' to 'type', possible loss of data
        /wd4800  # Boolean conversion
        /wd4251  # Class needs to have dll-interface
        /wd4275  # Non dll-interface class used as base
    )
    
    # Set precompiled header
    target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)
endif()

# Copy dependencies to output directory
if(COPY_DEPENDENCIES)
    if(EXISTS "${CMAKE_SOURCE_DIR}/Detours/detours.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/Detours/detours.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying Detours DLL to output directory"
        )
    endif()
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Print configuration info
message(STATUS "Configuration:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Detours Library: ${DETOURS_LIB}")