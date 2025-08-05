# Quick Start Guide - Rocket League Keyboard Overlay

## 🚀 Get Started in 5 Minutes

### Prerequisites
- Visual Studio 2019 or later
- Windows 10/11
- Rocket League installed
- A DLL injector (like Process Hacker, Extreme Injector, etc.)

### Step 1: Build the Project
1. **Open the solution**: Double-click `tntsTemplate.sln`
2. **Set configuration**: Ensure it's set to `Release` and `x64`
3. **Build**: Press `Ctrl+Shift+B` or use the build script:
   ```
   build.bat
   ```

### Step 2: Inject the DLL
1. **Start Rocket League**
2. **Open your DLL injector**
3. **Select Rocket League process**
4. **Inject the compiled DLL** (usually in `Release/` folder)

### Step 3: Use the Overlay
- The overlay appears automatically in-game
- **F1**: Toggle overlay on/off
- **F2**: Open settings panel
- **F3**: Reset overlay position

## 🎮 Default Controls

| Key | Action | Visual Feedback |
|-----|--------|-----------------|
| W | Forward | Green glow when pressed |
| S | Backward | Green glow when pressed |
| A | Turn Left | Green glow when pressed |
| D | Turn Right | Green glow when pressed |
| Space | Jump | Green glow when pressed |
| Shift | Boost | Green glow when pressed |
| Ctrl | Handbrake | Green glow when pressed |

## ⚙️ Quick Settings

### In-Game Settings Panel
1. **Open the mod GUI** (usually with a hotkey)
2. **Go to "Main" tab**
3. **Adjust settings**:
   - ✅ Enable Overlay
   - ✅ Show Key Names
   - 🎚️ Opacity: 0.8 (recommended)
   - 🎚️ Scale: 1.0 (recommended)

### Position the Overlay
- **Drag sliders** to move overlay
- **Click "Reset Position"** to return to default
- **Click "Reset All Settings"** for factory defaults

## 🎨 Customization

### Change Colors
Edit `Modules/Mods/KeyboardOverlayConfig.hpp`:
```cpp
// Change pressed key color to red
constexpr unsigned int KEY_PRESSED_TOP = 0xFFFF0000;
constexpr unsigned int KEY_PRESSED_BOTTOM = 0xFFCC0000;
```

### Add New Keys
Edit `Modules/Mods/KeyboardOverlay.cpp`:
```cpp
// In UpdateInputs function
KeyStates["NewKey"] = currentInputs.NewInput;

// In RenderKeyboardLayout function
RenderKey("NewKey", position, size, KeyStates["NewKey"]);
```

## 🔧 Troubleshooting

### Overlay Not Showing
- ✅ Check if mod is injected
- ✅ Verify "Enable Overlay" is checked
- ✅ Ensure you're in a Rocket League game
- ✅ Try pressing F1 to toggle

### Performance Issues
- 🔧 Reduce scale to 0.8
- 🔧 Lower opacity to 0.6
- 🔧 Disable key names
- 🔧 Restart Rocket League

### Build Errors
- 🔧 Ensure Visual Studio is installed
- 🔧 Check Windows SDK is installed
- 🔧 Run as Administrator
- 🔧 Clean and rebuild solution

## 📞 Support

### Common Issues
1. **"MSBuild not found"**: Install Visual Studio
2. **"DLL injection failed"**: Run injector as Administrator
3. **"Overlay not responding"**: Restart Rocket League
4. **"Visual glitches"**: Update graphics drivers

### Getting Help
- Check the main `README.md` for detailed documentation
- Review `KEYBOARD_OVERLAY_README.md` for advanced features
- Look at the configuration file for customization options

## 🎯 Pro Tips

### Performance Optimization
- Use scale 0.8 for better performance
- Disable glow effects if FPS drops
- Position overlay in corners for less interference

### Recording/Streaming
- Set opacity to 0.9 for better visibility
- Use scale 1.2 for recording
- Position overlay away from important UI elements

### Competitive Play
- Use minimal opacity (0.6) to avoid distraction
- Position overlay in bottom-left corner
- Disable key names for cleaner look

---

**Ready to play?** Build the project and inject the DLL to start using your keyboard overlay! 🚗⚽