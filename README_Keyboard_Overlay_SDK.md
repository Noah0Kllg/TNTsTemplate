# Keyboard Overlay for TNT Template SDK

A C++ keyboard input overlay module for Rocket League that displays which keys are being pressed in real-time. This overlay integrates directly with the TNT template SDK and uses ImGui for rendering.

## Features

- **Real-time keyboard input display**: Shows which keys are currently being pressed
- **WASD movement visualization**: Displays forward/backward and left/right movement
- **Action key indicators**: Shows jump (Space), boost (Shift), handbrake (Ctrl), and reset (R)
- **Camera control display**: Shows aerial control keys (Q, E, arrow keys)
- **Analog-like movement bars**: Visual representation of throttle and steering intensity
- **Color-coded keys**: Different colors for different types of actions
- **Always-on-top overlay**: Stays visible while gaming

## Integration

The keyboard overlay is already integrated into the TNT template SDK:

### Files Added:
- `Modules/Mods/KeyboardOverlay.hpp` - Header file
- `Modules/Mods/KeyboardOverlay.cpp` - Implementation file

### Integration Points:
- Added to `Modules/Includes.hpp`
- Integrated into `Components/Components/GUI.cpp`
- Initialized in `Components/Components/Main.cpp`

## How It Works

### Input Detection
The overlay reads vehicle inputs from the game using the `FVehicleInputs` structure:

```cpp
FVehicleInputs currentInputs = SafeRead<FVehicleInputs>((uintptr_t)localPlayerController + Offsets::TAGame::PlayerController_TA::VehicleInput);
```

### Input Mapping
The overlay maps analog controller inputs to keyboard digital states:

- **W/S**: Mapped from `Throttle` (positive/negative values)
- **A/D**: Mapped from `Steer` (negative/positive values)
- **Space**: Mapped from `bJump` (boolean)
- **Shift**: Mapped from `bActivateBoost` (boolean)
- **Ctrl**: Mapped from `bHandbrake` (boolean)
- **Q/E**: Mapped from `Yaw` (negative/positive values)
- **Arrow Keys**: Mapped from `Pitch` and `Roll` values

### Rendering
The overlay uses ImGui to render:
- Individual key representations with pressed/unpressed states
- Color-coded keys for different actions
- Progress bars for analog input values
- Positioned in the upper right corner of the screen

## Keyboard Layout

The overlay displays the following keys:

### Movement Keys (WASD)
- **W** (Green): Forward/Throttle
- **S** (Red): Backward/Brake  
- **A** (Blue): Left/Steer left
- **D** (Orange): Right/Steer right

### Action Keys
- **SPACE** (Green): Jump
- **SHIFT** (Red): Boost
- **CTRL** (Blue): Handbrake/Powerslide
- **R** (Yellow): Reset ball

### Camera Control Keys
- **Q** (Purple): Yaw left
- **E** (Purple): Yaw right
- **Arrow Keys** (Purple): Pitch and roll control

### Analog Indicators
- **Throttle Bar**: Shows forward/backward movement intensity
- **Steering Bar**: Shows left/right steering intensity

## Customization

### Changing Colors
You can modify the colors in the `DrawKey` function calls:

```cpp
// Example: Change W key color
DrawKey("W", CurrentKeyboardState.w, wPos, ImVec2(keySize, keySize), 
        IM_COL32(255, 255, 255, 100), IM_COL32(0, 255, 0, 200));
```

### Changing Position
Modify the position in the `DrawKeyboardOverlay` method:

```cpp
ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 350, 20), ImGuiCond_FirstUseEver);
```

### Changing Size
Modify the size in the `DrawKeyboardOverlay` method:

```cpp
ImGui::SetNextWindowSize(ImVec2(330, 220), ImGuiCond_FirstUseEver);
```

## Usage

1. **Compile the SDK**: The keyboard overlay is automatically included in the build
2. **Inject the DLL**: The overlay will appear when you're in a game
3. **View the overlay**: Look for the "Keyboard Input" window in the upper right corner

## Technical Details

### Module Structure
The keyboard overlay follows the same module pattern as other TNT template modules:

- Inherits from `Module` base class
- Implements event hooks for game state changes
- Uses ImGui for rendering
- Integrates with the existing GUI system

### Event Hooks
- `OnGameEventStart`: Called when a game starts
- `OnGameEventDestroyed`: Called when a game ends
- `PlayerTickCalled`: Called every frame to update input state
- `OnRender`: Called every frame to render the overlay

### Memory Safety
The overlay uses the `SafeRead` template function to safely read game memory:

```cpp
FVehicleInputs currentInputs = SafeRead<FVehicleInputs>((uintptr_t)localPlayerController + Offsets::TAGame::PlayerController_TA::VehicleInput);
```

## Troubleshooting

### Overlay doesn't appear
1. Make sure you're in a game (the overlay only shows when `IsInGame` is true)
2. Check that the module is properly initialized
3. Verify that ImGui is working correctly

### Keys not updating
1. Ensure the `PlayerTickCalled` method is being called
2. Check that the vehicle inputs are being read correctly
3. Verify the threshold values in the input mapping

### Performance issues
1. The overlay is designed to be lightweight
2. If needed, you can reduce the update frequency
3. The ImGui rendering is optimized for minimal performance impact

## Differences from Python Version

This C++ implementation differs from the Python version in several ways:

1. **Direct SDK integration**: Built directly into the TNT template
2. **ImGui rendering**: Uses ImGui instead of PySide6
3. **Memory reading**: Directly reads game memory instead of using controller state injection
4. **Performance**: More efficient due to C++ implementation
5. **Integration**: Seamlessly integrated with existing modules

## License

This keyboard overlay follows the same license as the TNT template SDK.