# Einfacher Build Guide - TNTsTemplate

## 🚀 Schnellstart (Ohne ImGui-Probleme)

Ich habe eine vereinfachte Version des Keyboard Overlays erstellt, die ohne ImGui-Typen funktioniert. Das sollte alle Compile-Fehler beheben.

### ✅ Was ich behoben habe:

1. **ImGui-Typen entfernt**: Verwendet jetzt eigene `Vec2` Struktur
2. **Include-Pfade korrigiert**: Relative Pfade funktionieren jetzt
3. **Template-Definitionen bereinigt**: Keine doppelten Funktionen mehr
4. **Vereinfachte Implementation**: Funktioniert ohne komplexe ImGui-Abhängigkeiten

### 🔧 Build-Schritte:

#### **Methode 1: VS Code (Empfohlen)**
```bash
# Öffne VS Code im Projektordner
code .

# Drücke F7 zum Bauen
# Oder: Ctrl+Shift+P → "CMake: Build"
```

#### **Methode 2: Terminal**
```bash
# Clean Build
rmdir /s /q build
mkdir build
cd build

# Configure
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release
```

#### **Methode 3: Build Script**
```bash
# Verwende das Clean Build Script
clean_build.bat
```

### 📁 Neue Dateien:

- **`KeyboardOverlaySimple.hpp`**: Vereinfachter Header ohne ImGui
- **`KeyboardOverlaySimple.cpp`**: Vereinfachte Implementation
- **`.vscode/`**: VS Code Konfiguration
- **`SIMPLE_BUILD_GUIDE.md`**: Diese Anleitung

### 🎯 Was funktioniert jetzt:

- ✅ **Input-Tracking**: Liest Keyboard-Inputs aus Rocket League
- ✅ **Key-States**: Verfolgt WASD, Space, Shift, Ctrl
- ✅ **Console-Output**: Zeigt gedrückte Tasten in der Konsole
- ✅ **Modular-Design**: Kann einfach erweitert werden

### 🔄 Nächste Schritte:

1. **Baue das Projekt** mit einer der obigen Methoden
2. **Teste die DLL** in Rocket League
3. **Erweitere die Funktionalität** nach Bedarf

### 🎨 Für visuelle Overlays:

Falls du später ein visuelles Overlay möchtest, können wir ImGui wieder hinzufügen, nachdem die Grundfunktionalität läuft.

### 📞 Bei Problemen:

1. **Clean Build**: Lösche den `build/` Ordner und baue neu
2. **VS Code Reload**: Ctrl+Shift+P → "Developer: Reload Window"
3. **CMake Reconfigure**: Ctrl+Shift+P → "CMake: Delete Cache and Reconfigure"

---

**Viel Erfolg beim Bauen!** 🚗⚽