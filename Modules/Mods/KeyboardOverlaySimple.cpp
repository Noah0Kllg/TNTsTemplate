#include "KeyboardOverlaySimple.hpp"
#include "Offsets.hpp"
#include <algorithm>
#include <cmath>

// Static member initialization
bool KeyboardOverlay::IsEnabled = true;
bool KeyboardOverlay::ShowKeyNames = true;
float KeyboardOverlay::Opacity = 0.8f;
float KeyboardOverlay::Scale = 1.0f;
Vec2 KeyboardOverlay::Position(50.0f, 50.0f);
std::map<std::string, bool> KeyboardOverlay::KeyStates;
std::map<std::string, KeyDisplay> KeyboardOverlay::KeyLayout;
FVehicleInputs KeyboardOverlay::LastInputs;

KeyboardOverlay::KeyboardOverlay() : Module("KeyboardOverlay", "Displays keyboard input overlay", States::STATES_All) {
    OnCreate();
}

KeyboardOverlay::~KeyboardOverlay() { 
    OnDestroy(); 
}

void KeyboardOverlay::OnCreate() {
    InitializeKeyLayout();
}

void KeyboardOverlay::OnDestroy() {
    // Cleanup if needed
}

void KeyboardOverlay::Hook() {
    Events.HookEventPost("Function TAGame.PlayerController_TA.PlayerTick", [](const PostEvent& event) {
        if (!IsEnabled) return;
        
        if (event.Caller() && event.Caller()->IsA(APlayerController_TA::StaticClass())) {
            APlayerController_TA* playerController = static_cast<APlayerController_TA*>(event.Caller());
            UpdateInputs(playerController);
        }
    });
}

void KeyboardOverlay::Initialize() {
    Hook();
    Console.Write("KeyboardOverlay Initialized.");
}

void KeyboardOverlay::UpdateInputs(APlayerController_TA* playerController) {
    if (!playerController) return;

    try {
        // Read current vehicle inputs
        FVehicleInputs currentInputs = SafeRead<FVehicleInputs>((uintptr_t)playerController + Offsets::TAGame::PlayerController_TA::VehicleInput);
        
        // Update key states based on vehicle inputs
        KeyStates["W"] = currentInputs.Throttle > 0.1f;
        KeyStates["S"] = currentInputs.Throttle < -0.1f;
        KeyStates["A"] = currentInputs.Steer < -0.1f;
        KeyStates["D"] = currentInputs.Steer > 0.1f;
        KeyStates["Space"] = currentInputs.Jump;
        KeyStates["Shift"] = currentInputs.ActivateBoost;
        KeyStates["Ctrl"] = currentInputs.Handbrake;
        KeyStates["Mouse1"] = currentInputs.ActivateBoost; // Alternative boost input
        
        LastInputs = currentInputs;
    }
    catch (...) {
        Console.Error("KeyboardOverlay: Exception in UpdateInputs");
    }
}

void KeyboardOverlay::OnRender() {
    if (!IsEnabled) return;

    RenderKeyboardLayout();
}

void KeyboardOverlay::RenderKeyboardLayout() {
    // This is a simplified version that doesn't use ImGui
    // In a real implementation, you would use ImGui drawing functions here
    
    // For now, just log the key states
    if (KeyStates["W"]) Console.Write("W pressed");
    if (KeyStates["A"]) Console.Write("A pressed");
    if (KeyStates["S"]) Console.Write("S pressed");
    if (KeyStates["D"]) Console.Write("D pressed");
    if (KeyStates["Space"]) Console.Write("Space pressed");
    if (KeyStates["Shift"]) Console.Write("Shift pressed");
    if (KeyStates["Ctrl"]) Console.Write("Ctrl pressed");
}

void KeyboardOverlay::RenderKey(const std::string& keyName, const Vec2& position, const Vec2& size, bool isPressed) {
    // Simplified key rendering - in real implementation, use ImGui
    if (isPressed) {
        Console.Write("Key " + keyName + " is pressed at position (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
    }
}

void KeyboardOverlay::InitializeKeyLayout() {
    // Initialize default key states
    KeyStates["W"] = false;
    KeyStates["A"] = false;
    KeyStates["S"] = false;
    KeyStates["D"] = false;
    KeyStates["Space"] = false;
    KeyStates["Shift"] = false;
    KeyStates["Ctrl"] = false;
    KeyStates["Mouse1"] = false;
    
    // Initialize LastInputs
    LastInputs = FVehicleInputs{};
}

Vec2 KeyboardOverlay::GetKeyPosition(const std::string& keyName) {
    // Return default position - customize as needed
    return Vec2(0.0f, 0.0f);
}

Vec2 KeyboardOverlay::GetKeySize(const std::string& keyName) {
    // Return default size - customize as needed
    return Vec2(40.0f, 40.0f);
}

void KeyboardOverlay::SetupKeyboardLayout() {
    // Setup keyboard layout - customize as needed
}

// Global instance
KeyboardOverlay KeyboardOverlayInstance;