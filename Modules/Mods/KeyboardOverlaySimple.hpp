#pragma once
#include "../Module.hpp"
#include "../../Components/Includes.hpp"
#include <map>
#include <string>
#include <vector>

// Simple vector type for 2D coordinates
struct Vec2 {
    float x, y;
    Vec2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
};

struct KeyDisplay {
    std::string keyName;
    bool isPressed;
    Vec2 position;
    Vec2 size;
    unsigned int color;
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
    static void Initialize();

    // Input tracking functions
    static void UpdateInputs(APlayerController_TA* playerController);
    static void RenderKeyboardLayout();
    static void RenderKey(const std::string& keyName, const Vec2& position, const Vec2& size, bool isPressed);

    // Keyboard layout configuration
    static void SetupKeyboardLayout();
    static Vec2 GetKeyPosition(const std::string& keyName);
    static Vec2 GetKeySize(const std::string& keyName);

    // Settings
    static bool IsEnabled;
    static bool ShowKeyNames;
    static float Opacity;
    static float Scale;
    static Vec2 Position;

    // Input state tracking
    static std::map<std::string, bool> KeyStates;
    static std::map<std::string, KeyDisplay> KeyLayout;
    static FVehicleInputs LastInputs;

private:
    static void InitializeKeyLayout();
};

extern class KeyboardOverlay KeyboardOverlayInstance;