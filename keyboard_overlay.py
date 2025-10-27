import sys
import time
import threading
from typing import Optional, Any

# Compatible SimpleControllerState for Sparkline
class SimpleControllerState:
    def __init__(self, steer=0.0, throttle=0.0, pitch=0.0, yaw=0.0, roll=0.0, jump=False, boost=False, handbrake=False, use_item=False):
        self.steer = steer
        self.throttle = throttle
        self.pitch = pitch
        self.yaw = yaw
        self.roll = roll
        self.jump = jump
        self.boost = boost
        self.handbrake = handbrake
        self.use_item = use_item

# PySide6 imports for the overlay
try:
    from PySide6.QtWidgets import QApplication, QWidget
    from PySide6.QtCore import Qt, QMetaObject, Q_ARG
    from PySide6.QtGui import QPainter, QPen, QBrush, QColor, QFont, QPalette
    PYSIDE6_AVAILABLE = True
except ImportError:
    PYSIDE6_AVAILABLE = False

class KeyboardData:
    """Container for keyboard input data"""
    def __init__(self):
        # Movement keys
        self.w = False  # Forward/Throttle
        self.s = False  # Backward/Brake
        self.a = False  # Left/Steer left
        self.d = False  # Right/Steer right
        
        # Aerial control
        self.space = False  # Jump
        self.shift = False  # Boost
        self.ctrl = False   # Handbrake/Powerslide
        
        # Camera control (if applicable)
        self.q = False     # Yaw left
        self.e = False     # Yaw right
        self.up = False    # Pitch up
        self.down = False  # Pitch down
        self.left = False  # Roll left
        self.right = False # Roll right
        
        # Additional keys
        self.r = False     # Reset ball
        self.t = False     # Team chat
        self.y = False     # All chat
        self.tab = False   # Scoreboard
        
        # Smoothed values for analog-like display
        self.smooth_throttle = 0.0
        self.smooth_steer = 0.0

class KeyboardOverlayWidget(QWidget):
    """PySide6 widget for displaying keyboard input"""
    
    def __init__(self):
        super().__init__()
        self.keyboard_data = KeyboardData()
        # Smoothing factor for analog-like movement display
        self.smoothing_factor = 0.15
        self.init_ui()
    
    def lerp(self, start, end, factor):
        """Linear interpolation between start and end values"""
        return start + (end - start) * factor
    
    def update_smooth_values(self):
        """Update smoothed values for analog-like movement display"""
        # Calculate throttle from W/S keys
        throttle = 0.0
        if self.keyboard_data.w:
            throttle = 1.0
        elif self.keyboard_data.s:
            throttle = -1.0
        
        # Calculate steering from A/D keys
        steer = 0.0
        if self.keyboard_data.a:
            steer = -1.0
        elif self.keyboard_data.d:
            steer = 1.0
        
        # Smooth the values
        self.keyboard_data.smooth_throttle = self.lerp(
            self.keyboard_data.smooth_throttle, 
            throttle, 
            self.smoothing_factor
        )
        self.keyboard_data.smooth_steer = self.lerp(
            self.keyboard_data.smooth_steer, 
            steer, 
            self.smoothing_factor
        )
    
    def init_ui(self):
        self.setWindowTitle("Sparkline Keyboard Overlay")
        self.setFixedSize(320, 200)
        
        # Position in upper RIGHT corner
        self.move(1000, 20)
        
        # Set window properties
        self.setWindowFlags(Qt.WindowStaysOnTopHint | Qt.Tool | Qt.FramelessWindowHint)
        
        # Set background color to dark gray/black
        palette = self.palette()
        palette.setColor(QPalette.Window, QColor(40, 40, 40))
        self.setPalette(palette)
        self.setAutoFillBackground(True)
    
    def update_keyboard_data(self, keyboard_dict):
        """Update the keyboard data and refresh display"""
        # Update all keyboard states
        for key, value in keyboard_dict.items():
            if hasattr(self.keyboard_data, key):
                setattr(self.keyboard_data, key, value)
        
        # Update smooth values
        self.update_smooth_values()
        
        # Trigger a repaint
        self.update()
    
    def paintEvent(self, event):
        """Custom paint event to draw the keyboard layout"""
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        # Colors
        white = QColor(255, 255, 255)
        gray = QColor(128, 128, 128)
        dark_gray = QColor(80, 80, 80)
        green = QColor(0, 255, 0)
        red = QColor(255, 0, 0)
        blue = QColor(0, 150, 255)
        yellow = QColor(255, 255, 0)
        orange = QColor(255, 165, 0)
        purple = QColor(128, 0, 128)
        
        # Draw title
        painter.setFont(QFont("Arial", 12, QFont.Bold))
        painter.setPen(QPen(white, 2))
        painter.drawText(10, 20, "KEYBOARD INPUT")
        
        # Draw WASD movement keys
        self.draw_movement_keys(painter, 20, 40, white, green, red, blue, orange)
        
        # Draw action keys
        self.draw_action_keys(painter, 20, 120, white, green, red, blue, yellow)
        
        # Draw camera keys
        self.draw_camera_keys(painter, 200, 40, white, purple)
        
        # Draw analog-like movement indicators
        self.draw_analog_indicators(painter, 200, 120, white, green, red)
    
    def draw_movement_keys(self, painter, x, y, normal_color, w_color, s_color, a_color, d_color):
        """Draw WASD movement keys in a cross pattern"""
        painter.setFont(QFont("Arial", 10, QFont.Bold))
        
        # W key (top)
        w_pressed = self.keyboard_data.w
        self.draw_key(painter, x + 25, y, "W", w_pressed, w_color if w_pressed else normal_color)
        
        # A key (left)
        a_pressed = self.keyboard_data.a
        self.draw_key(painter, x, y + 25, "A", a_pressed, a_color if a_pressed else normal_color)
        
        # S key (bottom)
        s_pressed = self.keyboard_data.s
        self.draw_key(painter, x + 25, y + 50, "S", s_pressed, s_color if s_pressed else normal_color)
        
        # D key (right)
        d_pressed = self.keyboard_data.d
        self.draw_key(painter, x + 50, y + 25, "D", d_pressed, d_color if d_pressed else normal_color)
    
    def draw_action_keys(self, painter, x, y, normal_color, jump_color, boost_color, handbrake_color, reset_color):
        """Draw action keys (Space, Shift, Ctrl, R)"""
        painter.setFont(QFont("Arial", 9, QFont.Bold))
        
        # Space (Jump)
        space_pressed = self.keyboard_data.space
        self.draw_key(painter, x, y, "SPACE", space_pressed, jump_color if space_pressed else normal_color, width=50)
        
        # Shift (Boost)
        shift_pressed = self.keyboard_data.shift
        self.draw_key(painter, x + 60, y, "SHIFT", shift_pressed, boost_color if shift_pressed else normal_color, width=40)
        
        # Ctrl (Handbrake)
        ctrl_pressed = self.keyboard_data.ctrl
        self.draw_key(painter, x, y + 30, "CTRL", ctrl_pressed, handbrake_color if ctrl_pressed else normal_color, width=40)
        
        # R (Reset)
        r_pressed = self.keyboard_data.r
        self.draw_key(painter, x + 60, y + 30, "R", r_pressed, reset_color if r_pressed else normal_color)
    
    def draw_camera_keys(self, painter, x, y, normal_color, pressed_color):
        """Draw camera control keys"""
        painter.setFont(QFont("Arial", 8, QFont.Bold))
        
        # Q (Yaw left)
        q_pressed = self.keyboard_data.q
        self.draw_key(painter, x, y, "Q", q_pressed, pressed_color if q_pressed else normal_color)
        
        # E (Yaw right)
        e_pressed = self.keyboard_data.e
        self.draw_key(painter, x + 25, y, "E", e_pressed, pressed_color if e_pressed else normal_color)
        
        # Arrow keys
        up_pressed = self.keyboard_data.up
        self.draw_key(painter, x + 25, y + 25, "↑", up_pressed, pressed_color if up_pressed else normal_color)
        
        down_pressed = self.keyboard_data.down
        self.draw_key(painter, x + 25, y + 50, "↓", down_pressed, pressed_color if down_pressed else normal_color)
        
        left_pressed = self.keyboard_data.left
        self.draw_key(painter, x, y + 37, "←", left_pressed, pressed_color if left_pressed else normal_color)
        
        right_pressed = self.keyboard_data.right
        self.draw_key(painter, x + 50, y + 37, "→", right_pressed, pressed_color if right_pressed else normal_color)
    
    def draw_analog_indicators(self, painter, x, y, normal_color, throttle_color, steer_color):
        """Draw analog-like movement indicators"""
        painter.setFont(QFont("Arial", 8))
        
        # Throttle indicator
        painter.setPen(QPen(normal_color, 1))
        painter.drawText(x, y, "Throttle:")
        
        # Throttle bar
        throttle_width = 60
        throttle_height = 8
        throttle_x = x
        throttle_y = y + 15
        
        # Background
        painter.setBrush(QBrush(dark_gray))
        painter.setPen(QPen(normal_color, 1))
        painter.drawRect(throttle_x, throttle_y, throttle_width, throttle_height)
        
        # Throttle level
        if self.keyboard_data.smooth_throttle != 0:
            throttle_fill_width = abs(self.keyboard_data.smooth_throttle) * throttle_width
            throttle_fill_x = throttle_x if self.keyboard_data.smooth_throttle > 0 else throttle_x + throttle_width - throttle_fill_width
            
            painter.setBrush(QBrush(throttle_color))
            painter.setPen(QPen(throttle_color, 1))
            painter.drawRect(throttle_fill_x, throttle_y, throttle_fill_width, throttle_height)
        
        # Steering indicator
        painter.setPen(QPen(normal_color, 1))
        painter.drawText(x, y + 40, "Steering:")
        
        # Steering bar
        steer_width = 60
        steer_height = 8
        steer_x = x
        steer_y = y + 55
        
        # Background
        painter.setBrush(QBrush(dark_gray))
        painter.setPen(QPen(normal_color, 1))
        painter.drawRect(steer_x, steer_y, steer_width, steer_height)
        
        # Steering level
        if self.keyboard_data.smooth_steer != 0:
            steer_fill_width = abs(self.keyboard_data.smooth_steer) * steer_width
            steer_fill_x = steer_x if self.keyboard_data.smooth_steer > 0 else steer_x + steer_width - steer_fill_width
            
            painter.setBrush(QBrush(steer_color))
            painter.setPen(QPen(steer_color, 1))
            painter.drawRect(steer_fill_x, steer_y, steer_fill_width, steer_height)
    
    def draw_key(self, painter, x, y, label, pressed, color, width=20, height=20):
        """Draw a single key"""
        if pressed:
            painter.setBrush(QBrush(color))
            painter.setPen(QPen(color, 2))
        else:
            painter.setBrush(QBrush())
            painter.setPen(QPen(color, 1))
        
        # Draw rectangle
        painter.drawRect(x, y, width, height)
        
        # Draw label
        painter.setPen(QPen(QColor(0, 0, 0) if pressed else color, 1))
        painter.drawText(x + 2, y + height - 4, label)

class KeyboardOverlayPlugin:
    def __init__(self, ConsoleLogger=None):
        """
        Initialize the Keyboard Overlay Plugin.
        ConsoleLogger: A function to log messages to the main application's console.
        """
        self.logger = ConsoleLogger or print
        self.Name = "Keyboard Overlay"
        self.enabled = True
        
        # GUI components
        self.overlay = None
        self.gui_initialized = False
        self.creation_attempted = False
        
        # Track current keyboard state
        self.current_keyboard = None
        
        self.logger(f"[{self.Name}] Plugin initialized.")
        
        if not PYSIDE6_AVAILABLE:
            self.logger(f"[{self.Name}] WARNING: PySide6 not available. Install with: pip install PySide6")
            self.enabled = False

    def create_overlay_safe(self):
        """Create overlay in a thread-safe manner"""
        try:
            app = QApplication.instance()
            if app is None:
                self.logger(f"[{self.Name}] No QApplication found")
                return False
            
            # Check if we're in the main thread
            main_thread = app.thread()
            current_thread = threading.current_thread()
            
            if hasattr(current_thread, 'ident') and hasattr(main_thread, 'currentThreadId'):
                if current_thread.ident != main_thread.currentThreadId():
                    self.logger(f"[{self.Name}] Not in main thread, deferring widget creation")
                    return False
            
            # Create the overlay widget
            self.overlay = KeyboardOverlayWidget()
            self.overlay.show()
            self.gui_initialized = True
            self.logger(f"[{self.Name}] Overlay created successfully in main thread")
            return True
            
        except Exception as e:
            self.logger(f"[{self.Name}] Error creating overlay: {e}")
            return False

    def initialize(self):
        """
        Called once when the plugin system initializes all loaded plugins.
        """
        self.logger(f"[{self.Name}] Initialize called.")
        
        if not self.enabled or not PYSIDE6_AVAILABLE:
            return
        
        if self.creation_attempted:
            self.logger(f"[{self.Name}] Overlay creation already attempted, skipping")
            return
            
        self.creation_attempted = True
        
        # Try to create overlay
        if not self.create_overlay_safe():
            self.logger(f"[{self.Name}] Will try to create overlay later in controller_filter")

    def game_tick_packet_set(self, packet, local_player_index, playername, field_info):
        """
        Called every game tick with the current game state.
        """
        if not self.enabled:
            return None
        
        # Try to create overlay if not yet created
        if not self.gui_initialized and not self.creation_attempted:
            self.creation_attempted = True
            self.create_overlay_safe()
        
        return None  # This plugin doesn't inject controller states

    def controller_filter(self, controller: SimpleControllerState) -> SimpleControllerState:
        """
        Called for each SimpleControllerState that is about to be written to memory.
        This is where we capture the actual controller inputs to display.
        """
        if not self.enabled:
            return controller
        
        # Try to create overlay if not yet created (this method is likely called from main thread)
        if not self.gui_initialized and PYSIDE6_AVAILABLE:
            if self.create_overlay_safe():
                pass  # Success
        
        if not self.gui_initialized:
            return controller
        
        try:
            # Extract keyboard values from the controller state
            # This assumes the controller state contains keyboard input data
            keyboard_dict = {
                'w': getattr(controller, 'throttle', 0.0) > 0.1,
                's': getattr(controller, 'throttle', 0.0) < -0.1,
                'a': getattr(controller, 'steer', 0.0) < -0.1,
                'd': getattr(controller, 'steer', 0.0) > 0.1,
                'space': getattr(controller, 'jump', False),
                'shift': getattr(controller, 'boost', False),
                'ctrl': getattr(controller, 'handbrake', False),
                'q': getattr(controller, 'yaw', 0.0) < -0.1,
                'e': getattr(controller, 'yaw', 0.0) > 0.1,
                'up': getattr(controller, 'pitch', 0.0) > 0.1,
                'down': getattr(controller, 'pitch', 0.0) < -0.1,
                'left': getattr(controller, 'roll', 0.0) < -0.1,
                'right': getattr(controller, 'roll', 0.0) > 0.1,
                'r': False,  # Reset - would need to be detected from game state
                'tab': False,  # Scoreboard - would need to be detected from game state
                't': False,  # Team chat - would need to be detected from game state
                'y': False   # All chat - would need to be detected from game state
            }
            
            # Update the overlay with the keyboard state
            if self.overlay and self.overlay.isVisible():
                self.overlay.update_keyboard_data(keyboard_dict)
            
            # Store for reference
            self.current_keyboard = keyboard_dict
            
        except Exception as e:
            self.logger(f"[{self.Name}] Error in controller_filter: {e}")
        
        return controller  # Return the unmodified controller

    def main(self):
        """
        Background thread for any ongoing tasks.
        Currently just monitors the overlay.
        """
        self.logger(f"[{self.Name}] Background thread started.")
        
        while self.enabled:
            try:
                # Check if overlay is still valid
                if self.overlay and self.gui_initialized:
                    if not self.overlay.isVisible():
                        self.logger(f"[{self.Name}] Overlay window was closed.")
                        self.gui_initialized = False
                
                time.sleep(1)  # Check every second
                
            except Exception as e:
                self.logger(f"[{self.Name}] Error in background thread: {e}")
                break
        
        self.logger(f"[{self.Name}] Background thread finishing.")

    def shutdown(self):
        """
        Called when plugins are being shut down.
        """
        self.logger(f"[{self.Name}] Shutdown called.")
        self.enabled = False
        
        if self.overlay:
            try:
                self.overlay.close()
                self.overlay.deleteLater()
                self.overlay = None
                self.logger(f"[{self.Name}] Overlay closed successfully.")
            except Exception as e:
                self.logger(f"[{self.Name}] Error closing overlay: {e}")
        
        self.gui_initialized = False
        self.creation_attempted = False