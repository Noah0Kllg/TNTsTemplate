#pragma once
#include "../Module.hpp"
#include "../../Components/Includes.hpp"
#include <map>
#include <string>

// Simple vector type
struct Vec2 {
    float x, y;
    Vec2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
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

    // Settings
    static bool IsEnabled;
    static bool ShowKeyNames;
    static float Opacity;
    static float Scale;
    static Vec2 Position;

    // Input state tracking
    static std::map<std::string, bool> KeyStates;
    static FVehicleInputs LastInputs;

private:
    static void InitializeKeyLayout();
};

extern KeyboardOverlay KeyboardOverlayInstance;