# VS Code Build Guide - TNTsTemplate Keyboard Overlay

## 🚀 Schneller Start mit VS Code

### Voraussetzungen
- **VS Code** installiert
- **CMake Tools** Extension installiert
- **C/C++** Extension installiert
- **Visual Studio 2022** (mit C++ Workload)

### Installation der Extensions
1. Öffne VS Code
2. Gehe zu Extensions (Ctrl+Shift+X)
3. Installiere:
   - **CMake Tools** (ms-vscode.cmake-tools)
   - **C/C++** (ms-vscode.cpptools)

## 🔧 Build-Methoden

### Methode 1: CMake Tools Extension (Empfohlen)
1. **Öffne das Projekt** in VS Code
2. **Wähle Kit**: Unten in der Statusleiste auf "No Kit Selected" klicken
3. **Wähle**: "Visual Studio Build Tools 2022 Release - amd64"
4. **Konfiguriere**: Ctrl+Shift+P → "CMake: Configure"
5. **Baue**: Ctrl+Shift+P → "CMake: Build" oder F7

### Methode 2: Tasks (Einfach)
1. **Öffne Command Palette**: Ctrl+Shift+P
2. **Wähle**: "Tasks: Run Task"
3. **Wähle**: "CMake Build Release"

### Methode 3: Terminal
```bash
# Im VS Code Terminal
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Methode 4: Shortcuts
- **F7**: Build
- **Ctrl+Shift+P → "CMake: Build"**: Build
- **Ctrl+Shift+P → "CMake: Clean"**: Clean

## ⚙️ VS Code Konfiguration

### Automatische Konfiguration
Die `.vscode/` Ordner enthält bereits:
- `c_cpp_properties.json`: Include-Pfade und Compiler-Einstellungen
- `settings.json`: CMake und C++ Einstellungen
- `tasks.json`: Build-Tasks

### Manuelle Anpassung
Falls du andere Pfade hast, bearbeite `c_cpp_properties.json`:
```json
{
    "compilerPath": "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.38.33130/bin/Hostx64/x64/cl.exe"
}
```

## 🔍 Troubleshooting

### Problem: "Components/Includes.hpp: No such file or directory"
**Lösung**: 
1. Stelle sicher, dass alle Dateien vorhanden sind
2. Reload VS Code (Ctrl+Shift+P → "Developer: Reload Window")
3. Reconfigure CMake (Ctrl+Shift+P → "CMake: Delete Cache and Reconfigure")

### Problem: "Memberfunktion bereits definiert"
**Lösung**: 
1. Clean Build (Ctrl+Shift+P → "CMake: Clean")
2. Rebuild (F7)

### Problem: ImGui-Typen nicht gefunden
**Lösung**:
1. Stelle sicher, dass ImGui-Ordner vorhanden ist
2. Checke Include-Pfade in `c_cpp_properties.json`

### Problem: CMake nicht gefunden
**Lösung**:
1. Installiere CMake: https://cmake.org/download/
2. Füge CMake zu PATH hinzu
3. Restart VS Code

## 🎯 Build-Tasks Übersicht

| Task | Beschreibung | Shortcut |
|------|-------------|----------|
| CMake Configure | Konfiguriert das Projekt | - |
| CMake Build Release | Baut Release-Version | F7 |
| CMake Build Debug | Baut Debug-Version | - |
| Clean Build | Löscht Build-Ordner | - |
| Full Clean and Build | Kompletter Neubau | - |

## 📁 Projektstruktur

```
TNTsTemplate/
├── .vscode/                    # VS Code Konfiguration
│   ├── c_cpp_properties.json   # Include-Pfade
│   ├── settings.json           # CMake Einstellungen
│   └── tasks.json              # Build-Tasks
├── Modules/Mods/
│   ├── KeyboardOverlay.cpp     # Keyboard Overlay
│   ├── KeyboardOverlay.hpp     # Header
│   └── KeyboardOverlayConfig.hpp # Konfiguration
├── Components/                 # Core Components
├── ImGui/                      # ImGui Library
├── Detours/                    # Microsoft Detours
└── CMakeLists.txt              # CMake Konfiguration
```

## 🚀 Schnellstart

1. **Öffne Projekt** in VS Code
2. **Warte** auf CMake Auto-Configure
3. **Drücke F7** zum Bauen
4. **Fertig!** DLL ist in `build/bin/Release/`

## 🔧 Erweiterte Einstellungen

### CMake Presets verwenden
```bash
# Im Terminal
cmake --preset default
cmake --build --preset default
```

### Debug-Build
1. Ändere Build-Type auf "Debug"
2. Oder verwende Debug-Preset

### Custom CMake Options
```bash
cmake .. -DENABLE_KEYBOARD_OVERLAY=ON -DENABLE_DEBUG_OUTPUT=OFF
```

## 📞 Hilfe

### VS Code Extensions
- **CMake Tools**: Hauptextension für CMake
- **C/C++**: IntelliSense und Debugging
- **CMake**: Syntax-Highlighting

### Nützliche Commands
- `Ctrl+Shift+P → "CMake: Configure"`: Konfiguriert Projekt
- `Ctrl+Shift+P → "CMake: Build"`: Baut Projekt
- `Ctrl+Shift+P → "CMake: Clean"`: Löscht Build
- `Ctrl+Shift+P → "CMake: Clean Rebuild"`: Kompletter Neubau

### Debugging
- Setze Breakpoints in VS Code
- Verwende Debug-Build für besseres Debugging
- CMake Tools unterstützt Debugging

---

**Viel Spaß beim Bauen!** 🚗⚽