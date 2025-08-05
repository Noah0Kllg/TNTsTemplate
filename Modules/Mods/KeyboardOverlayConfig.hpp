#pragma once
#include <string>
#include <map>

// Keyboard Overlay Configuration
// Modify these settings to customize the overlay appearance and behavior

namespace KeyboardOverlayConfig {
    
    // Default settings
    constexpr bool DEFAULT_ENABLED = true;
    constexpr bool DEFAULT_SHOW_KEY_NAMES = true;
    constexpr float DEFAULT_OPACITY = 0.8f;
    constexpr float DEFAULT_SCALE = 1.0f;
    constexpr float DEFAULT_POSITION_X = 50.0f;
    constexpr float DEFAULT_POSITION_Y = 50.0f;
    
    // Panel dimensions
    constexpr float PANEL_WIDTH = 450.0f;
    constexpr float PANEL_HEIGHT = 280.0f;
    
    // Key dimensions
    constexpr float KEY_SIZE = 45.0f;
    constexpr float KEY_SPACING = 12.0f;
    
    // Colors (RGBA format)
    namespace Colors {
        // Background colors
        constexpr unsigned int BG_COLOR_1 = 0xFF141419; // Dark blue-gray
        constexpr unsigned int BG_COLOR_2 = 0xFF1E1E23; // Lighter blue-gray
        constexpr unsigned int BORDER_COLOR = 0xFF6496FF; // Blue border
        
        // Key colors - Pressed state
        constexpr unsigned int KEY_PRESSED_TOP = 0xFF00FF64; // Green top
        constexpr unsigned int KEY_PRESSED_BOTTOM = 0xFF00C850; // Darker green bottom
        constexpr unsigned int KEY_PRESSED_BORDER = 0xFF00FF96; // Green border
        constexpr unsigned int KEY_PRESSED_GLOW = 0x6600FF64; // Green glow
        
        // Key colors - Unpressed state
        constexpr unsigned int KEY_UNPRESSED_TOP = 0xFF3C3C46; // Dark gray top
        constexpr unsigned int KEY_UNPRESSED_BOTTOM = 0xFF282832; // Darker gray bottom
        constexpr unsigned int KEY_UNPRESSED_BORDER = 0xFF787882; // Gray border
        
        // Text colors
        constexpr unsigned int TEXT_PRESSED = 0xFF000000; // Black text for pressed keys
        constexpr unsigned int TEXT_UNPRESSED = 0xFFFFFFFF; // White text for unpressed keys
        constexpr unsigned int TEXT_SHADOW = 0xFF000000; // Black shadow
        
        // Progress bar colors
        constexpr unsigned int PROGRESS_BG = 0xFF323232; // Dark gray background
        constexpr unsigned int PROGRESS_THROTTLE_FORWARD = 0xFF00FF64; // Green for forward
        constexpr unsigned int PROGRESS_THROTTLE_BACKWARD = 0xFFFF6400; // Orange for backward
        constexpr unsigned int PROGRESS_STEER = 0xFF6496FF; // Blue for steering
    }
    
    // Key mappings - Define which game inputs correspond to which keys
    struct KeyMapping {
        std::string keyName;
        std::string displayName;
        bool isMovementKey;
        bool isActionKey;
    };
    
    // Default key mappings
    static const std::map<std::string, KeyMapping> DEFAULT_KEY_MAPPINGS = {
        {"W", {"W", "W", true, false}},
        {"A", {"A", "A", true, false}},
        {"S", {"S", "S", true, false}},
        {"D", {"D", "D", true, false}},
        {"Space", {"Space", "Space", false, true}},
        {"Shift", {"Shift", "Shift", false, true}},
        {"Ctrl", {"Ctrl", "Ctrl", false, true}},
        {"Mouse1", {"Mouse1", "Mouse1", false, true}}
    };
    
    // Animation settings
    namespace Animation {
        constexpr float GLOW_INTENSITY = 0.1f;
        constexpr float GLOW_RADIUS = 2.0f;
        constexpr float BORDER_THICKNESS_PRESSED = 3.0f;
        constexpr float BORDER_THICKNESS_UNPRESSED = 1.5f;
        constexpr float CORNER_RADIUS = 4.0f;
    }
    
    // Layout settings
    namespace Layout {
        constexpr float TITLE_MARGIN = 15.0f;
        constexpr float SUBTITLE_MARGIN = 35.0f;
        constexpr float CONTENT_START_Y = 70.0f;
        constexpr float INFO_SPACING = 25.0f;
        constexpr float PROGRESS_BAR_WIDTH = 120.0f;
        constexpr float PROGRESS_BAR_HEIGHT = 8.0f;
    }
    
    // Performance settings
    namespace Performance {
        constexpr bool ENABLE_GLOW_EFFECTS = true;
        constexpr bool ENABLE_GRADIENTS = true;
        constexpr bool ENABLE_SHADOWS = true;
        constexpr int MAX_RENDER_CALLS = 1000; // Safety limit
    }
    
    // Input sensitivity settings
    namespace Input {
        constexpr float THROTTLE_THRESHOLD = 0.1f; // Minimum throttle value to register as pressed
        constexpr float STEER_THRESHOLD = 0.1f;    // Minimum steering value to register as pressed
        constexpr bool INVERT_THROTTLE = false;     // Invert throttle direction
        constexpr bool INVERT_STEER = false;        // Invert steering direction
    }
    
    // Hotkey settings (for toggling overlay)
    namespace Hotkeys {
        constexpr int TOGGLE_OVERLAY_KEY = VK_F1;      // F1 to toggle overlay
        constexpr int TOGGLE_SETTINGS_KEY = VK_F2;     // F2 to toggle settings
        constexpr int RESET_POSITION_KEY = VK_F3;      // F3 to reset position
        constexpr bool ENABLE_HOTKEYS = true;          // Enable/disable hotkeys
    }
}

// Helper functions for color manipulation
namespace ColorUtils {
    inline unsigned int SetAlpha(unsigned int color, float alpha) {
        unsigned int r = (color >> 24) & 0xFF;
        unsigned int g = (color >> 16) & 0xFF;
        unsigned int b = (color >> 8) & 0xFF;
        unsigned int a = static_cast<unsigned int>(alpha * 255.0f);
        return (r << 24) | (g << 16) | (b << 8) | a;
    }
    
    inline unsigned int BlendColors(unsigned int color1, unsigned int color2, float blend) {
        unsigned int r1 = (color1 >> 24) & 0xFF;
        unsigned int g1 = (color1 >> 16) & 0xFF;
        unsigned int b1 = (color1 >> 8) & 0xFF;
        unsigned int a1 = color1 & 0xFF;
        
        unsigned int r2 = (color2 >> 24) & 0xFF;
        unsigned int g2 = (color2 >> 16) & 0xFF;
        unsigned int b2 = (color2 >> 8) & 0xFF;
        unsigned int a2 = color2 & 0xFF;
        
        unsigned int r = static_cast<unsigned int>(r1 * (1.0f - blend) + r2 * blend);
        unsigned int g = static_cast<unsigned int>(g1 * (1.0f - blend) + g2 * blend);
        unsigned int b = static_cast<unsigned int>(b1 * (1.0f - blend) + b2 * blend);
        unsigned int a = static_cast<unsigned int>(a1 * (1.0f - blend) + a2 * blend);
        
        return (r << 24) | (g << 16) | (b << 8) | a;
    }
}