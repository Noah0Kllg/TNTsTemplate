#pragma once
#include "../Module.hpp"
#include "Components/Includes.hpp"
#include <vector>

struct KeyboardInputState {
    // Movement keys
    bool w = false;      // Forward/Throttle
    bool s = false;      // Backward/Brake
    bool a = false;      // Left/Steer left
    bool d = false;      // Right/Steer right
    
    // Action keys
    bool space = false;  // Jump
    bool shift = false;  // Boost
    bool ctrl = false;   // Handbrake/Powerslide
    bool r = false;      // Reset ball
    
    // Camera control keys
    bool q = false;      // Yaw left
    bool e = false;      // Yaw right
    bool up = false;     // Pitch up
    bool down = false;   // Pitch down
    bool left = false;   // Roll left
    bool right = false;  // Roll right
    
    // Additional keys
    bool tab = false;    // Scoreboard
    bool t = false;      // Team chat
    bool y = false;      // All chat
    
    // Analog values for smooth display
    float throttle = 0.0f;
    float steer = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
};

class KeyboardOverlay : public Module
{
public:
    KeyboardOverlay();
    ~KeyboardOverlay() override;

    void OnCreate();
    void OnDestroy();
    void OnRender();

    static void Hook();
    static void OnGameEventStart(PreEvent& event);
    static void OnGameEventDestroyed(PreEvent& event);
    static void PlayerTickCalled(const PostEvent& event);
    static void Initialize();

    static bool IsInGame;
    static AGameEvent_Soccar_TA* CurrentGameEvent;
    static KeyboardInputState CurrentKeyboardState;
    static APRI_TA* localPlayerPRI;

private:
    void DrawKeyboardOverlay();
    void DrawKey(const char* label, bool pressed, const ImVec2& pos, const ImVec2& size, ImU32 normalColor, ImU32 pressedColor);
    void DrawMovementKeys();
    void DrawActionKeys();
    void DrawCameraKeys();
    void DrawAnalogBars();
    void UpdateKeyboardState(const FVehicleInputs& inputs);
};

extern class KeyboardOverlay KeyboardOverlayInstance;