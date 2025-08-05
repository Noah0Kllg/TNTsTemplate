# Keyboard Overlay Plugin

A simple keyboard input overlay for Rocket League that displays which keys are being pressed in real-time. This overlay is designed to work alongside your existing controller overlay.

## Features

- **Real-time keyboard input display**: Shows which keys are currently being pressed
- **WASD movement visualization**: Displays forward/backward and left/right movement
- **Action key indicators**: Shows jump (Space), boost (Shift), handbrake (Ctrl), and reset (R)
- **Camera control display**: Shows aerial control keys (Q, E, arrow keys)
- **Analog-like movement bars**: Visual representation of throttle and steering intensity
- **Color-coded keys**: Different colors for different types of actions
- **Always-on-top overlay**: Stays visible while gaming

## Installation

1. **Install PySide6** (required for the GUI):
   ```bash
   pip install PySide6
   ```

2. **Copy the files**:
   - `keyboard_overlay.py` - Main plugin file
   - `test_keyboard_overlay.py` - Test script (optional)

## Usage

### Integration with Sparkline

The keyboard overlay plugin follows the same structure as your controller overlay and can be integrated into your Sparkline bot system:

1. Import the plugin in your main bot file
2. Initialize it alongside your controller overlay
3. The plugin will automatically capture keyboard input from the controller state

### Testing the Overlay

To test the overlay independently:

```bash
python test_keyboard_overlay.py
```

This will run a demonstration showing different keyboard input scenarios.

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
You can modify the colors in the `paintEvent` method of `KeyboardOverlayWidget`:

```python
# Colors
white = QColor(255, 255, 255)
green = QColor(0, 255, 0)      # Forward/Jump
red = QColor(255, 0, 0)        # Backward/Boost
blue = QColor(0, 150, 255)     # Left/Handbrake
orange = QColor(255, 165, 0)   # Right
yellow = QColor(255, 255, 0)   # Reset
purple = QColor(128, 0, 128)   # Camera controls
```

### Changing Position
Modify the position in the `init_ui` method:

```python
# Position in upper RIGHT corner
self.move(1000, 20)  # Change these coordinates
```

### Changing Size
Modify the size in the `init_ui` method:

```python
self.setFixedSize(320, 200)  # Change width and height
```

## Plugin Integration

The `KeyboardOverlayPlugin` class provides the same interface as your controller overlay:

- `initialize()`: Called when the plugin system initializes
- `game_tick_packet_set()`: Called every game tick
- `controller_filter()`: Called for each controller state
- `main()`: Background thread for ongoing tasks
- `shutdown()`: Called when plugins are being shut down

## Troubleshooting

### Overlay doesn't appear
1. Make sure PySide6 is installed: `pip install PySide6`
2. Check that you're running the script in the main thread
3. Verify that a QApplication instance exists

### Keys not updating
1. Ensure the `controller_filter` method is being called
2. Check that the controller state contains the expected values
3. Verify the threshold values in the keyboard mapping

### Performance issues
1. Reduce the smoothing factor for more responsive updates
2. Lower the update frequency in the main loop
3. Simplify the drawing operations if needed

## Differences from Controller Overlay

While the keyboard overlay follows the same structure as your controller overlay, there are some key differences:

1. **Input source**: Maps controller analog values to keyboard digital states
2. **Visual layout**: Shows individual keys rather than analog sticks
3. **Positioning**: Positioned in upper right corner (vs. upper left for controller)
4. **Size**: Larger overlay to accommodate more keys
5. **Color scheme**: Different colors for different key types

## License

This plugin follows the same license as your existing controller overlay code.