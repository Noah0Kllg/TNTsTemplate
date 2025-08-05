# Rocket League Keyboard Overlay

A real-time keyboard input visualization overlay for Rocket League, built using the TNTsTemplate.

## Features

### 🎮 Real-time Input Display
- **WASD Movement Keys**: Visual feedback for forward/backward/left/right movement
- **Action Keys**: Space (Jump), Shift (Boost), Ctrl (Handbrake)
- **Input Values**: Real-time throttle and steering percentage display
- **Status Indicators**: Shows when any action key is active

### 🎨 Visual Design
- **Modern UI**: Gradient backgrounds and smooth animations
- **Glow Effects**: Pressed keys have a green glow effect
- **Progress Bars**: Visual representation of throttle and steering inputs
- **Customizable**: Adjustable opacity, scale, and position

### ⚙️ Configuration Options
- **Enable/Disable**: Toggle the overlay on and off
- **Opacity Control**: Adjust transparency (0.1 - 1.0)
- **Scale Control**: Resize the overlay (0.5x - 2.0x)
- **Position Control**: Move the overlay anywhere on screen
- **Key Names**: Show/hide key labels

## Installation

1. **Build the Project**: 
   - Open the solution in Visual Studio
   - Build in Release x64 configuration
   - The keyboard overlay is automatically included

2. **Inject the DLL**:
   - Use your preferred DLL injector
   - Inject the compiled DLL into Rocket League

## Usage

### In-Game Controls
- The overlay automatically appears when you start Rocket League
- It displays your current keyboard inputs in real-time
- No additional configuration needed to start using

### Configuration Panel
1. **Open the GUI**: The mod includes a configuration window
2. **Access Settings**: Go to the "Main" tab
3. **Adjust Settings**: Use the sliders and checkboxes to customize the overlay

### Settings Explained

| Setting | Description | Range |
|---------|-------------|-------|
| Enable Overlay | Turn the overlay on/off | Checkbox |
| Show Key Names | Display key labels on buttons | Checkbox |
| Opacity | Control transparency | 0.1 - 1.0 |
| Scale | Resize the overlay | 0.5x - 2.0x |
| X Position | Horizontal position | 0 - 1920 |
| Y Position | Vertical position | 0 - 1080 |

### Reset Options
- **Reset Position**: Returns overlay to default position (50, 50)
- **Reset All Settings**: Restores all settings to default values

## Technical Details

### Input Detection
The overlay reads vehicle inputs directly from Rocket League's memory:
- `Throttle`: Forward/backward movement (-100% to +100%)
- `Steer`: Left/right steering (-100% to +100%)
- `Jump`: Space bar input
- `ActivateBoost`: Shift key input
- `Handbrake`: Ctrl key input

### Rendering
- Uses ImGui for rendering
- Draws directly to the game's background
- No impact on game performance
- Compatible with all Rocket League graphics settings

### Memory Safety
- Safe memory reading with exception handling
- No game crashes or instability
- Automatic cleanup on mod unload

## Customization

### Adding New Keys
To add support for additional keys, modify `KeyboardOverlay.cpp`:

```cpp
// In UpdateInputs function
KeyStates["NewKey"] = currentInputs.NewInput;

// In RenderKeyboardLayout function
RenderKey("NewKey", position, size, KeyStates["NewKey"]);
```

### Changing Colors
Modify the color values in `RenderKey` function:
```cpp
// Pressed state colors
keyColorTop = IM_COL32(0, 255, 100, (int)(220 * globalAlpha));
keyColorBottom = IM_COL32(0, 200, 80, (int)(220 * globalAlpha));

// Unpressed state colors
keyColorTop = IM_COL32(60, 60, 70, (int)(180 * globalAlpha));
keyColorBottom = IM_COL32(40, 40, 50, (int)(180 * globalAlpha));
```

## Troubleshooting

### Overlay Not Appearing
1. Check if the mod is properly injected
2. Verify "Enable Overlay" is checked in settings
3. Ensure you're in a Rocket League game session

### Performance Issues
1. Reduce the scale setting
2. Lower the opacity
3. Disable key names if not needed

### Visual Glitches
1. Try resetting all settings
2. Restart Rocket League
3. Re-inject the DLL

## Compatibility

- **Rocket League Versions**: Compatible with current versions
- **Graphics APIs**: Works with DirectX 11
- **Resolutions**: Supports all common resolutions
- **Performance**: Minimal impact on FPS

## Credits

- Built using TNTsTemplate
- ImGui for rendering
- KieroHook for DirectX hooking

## License

This keyboard overlay is part of the TNTsTemplate project. Please refer to the main LICENSE file for terms and conditions.

---

**Note**: This mod is for educational and personal use only. Use at your own risk and in accordance with Rocket League's terms of service.