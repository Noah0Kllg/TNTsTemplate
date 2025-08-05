#pragma once
#include "../Module.hpp"
#include "Components/Includes.hpp"
#include "KeyboardOverlayConfig.hpp"
#include <map>
#include <string>
#include <vector>

// Forward declarations for ImGui types
struct ImVec2;
typedef unsigned int ImU32;

struct KeyDisplay {
    std::string keyName;
    bool isPressed;
    ImVec2 position;
    ImVec2 size;
    ImU32 color;
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
    static void RenderKey(const std::string& keyName, const ImVec2& position, const ImVec2& size, bool isPressed);

    // Keyboard layout configuration
    static void SetupKeyboardLayout();
    static ImVec2 GetKeyPosition(const std::string& keyName);
    static ImVec2 GetKeySize(const std::string& keyName);

    // Settings
    static bool IsEnabled;
    static bool ShowKeyNames;
    static float Opacity;
    static float Scale;
    static ImVec2 Position;

    // Input state tracking
    static std::map<std::string, bool> KeyStates;
    static std::map<std::string, KeyDisplay> KeyLayout;
    static FVehicleInputs LastInputs;

private:
    static void InitializeKeyLayout();
};

extern class KeyboardOverlay KeyboardOverlayInstance;