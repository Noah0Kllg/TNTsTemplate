#include "KeyboardOverlay.hpp"
#include "Offsets.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include <algorithm>
#include <cmath>

// Static member initialization
bool KeyboardOverlay::IsEnabled = KeyboardOverlayConfig::DEFAULT_ENABLED;
bool KeyboardOverlay::ShowKeyNames = KeyboardOverlayConfig::DEFAULT_SHOW_KEY_NAMES;
float KeyboardOverlay::Opacity = KeyboardOverlayConfig::DEFAULT_OPACITY;
float KeyboardOverlay::Scale = KeyboardOverlayConfig::DEFAULT_SCALE;
ImVec2 KeyboardOverlay::Position(KeyboardOverlayConfig::DEFAULT_POSITION_X, KeyboardOverlayConfig::DEFAULT_POSITION_Y);
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
        
        // Update key states based on vehicle inputs using config thresholds
        KeyStates["W"] = currentInputs.Throttle > KeyboardOverlayConfig::Input::THROTTLE_THRESHOLD;
        KeyStates["S"] = currentInputs.Throttle < -KeyboardOverlayConfig::Input::THROTTLE_THRESHOLD;
        KeyStates["A"] = currentInputs.Steer < -KeyboardOverlayConfig::Input::STEER_THRESHOLD;
        KeyStates["D"] = currentInputs.Steer > KeyboardOverlayConfig::Input::STEER_THRESHOLD;
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
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    
    // Apply global settings
    float globalAlpha = Opacity;
    ImVec2 globalPos = Position;
    float globalScale = Scale;

    // Draw background panel with gradient
    ImVec2 panelSize(KeyboardOverlayConfig::PANEL_WIDTH * globalScale, KeyboardOverlayConfig::PANEL_HEIGHT * globalScale);
    ImVec2 panelPos = globalPos;
    
    // Gradient background
    ImU32 bgColor1 = IM_COL32(20, 20, 25, (int)(180 * globalAlpha));
    ImU32 bgColor2 = IM_COL32(30, 30, 35, (int)(180 * globalAlpha));
    drawList->AddRectFilledMultiColor(
        panelPos,
        ImVec2(panelPos.x + panelSize.x, panelPos.y + panelSize.y),
        bgColor1, bgColor2, bgColor2, bgColor1
    );
    
    // Border with glow effect
    ImU32 borderColor = IM_COL32(100, 150, 255, (int)(200 * globalAlpha));
    drawList->AddRect(
        panelPos,
        ImVec2(panelPos.x + panelSize.x, panelPos.y + panelSize.y),
        borderColor,
        8.0f,
        0,
        2.0f
    );

    // Draw title with gradient
    ImVec2 titlePos(panelPos.x + 15.0f, panelPos.y + 15.0f);
    ImVec2 titleSize = ImGui::CalcTextSize("Rocket League Inputs");
    drawList->AddText(
        titlePos,
        IM_COL32(255, 255, 255, (int)(255 * globalAlpha)),
        "Rocket League Inputs"
    );
    
    // Draw subtitle
    ImVec2 subtitlePos(panelPos.x + 15.0f, panelPos.y + 35.0f);
    drawList->AddText(
        subtitlePos,
        IM_COL32(200, 200, 200, (int)(180 * globalAlpha)),
        "Real-time input visualization"
    );

    // Draw keys with better layout
    float keySize = KeyboardOverlayConfig::KEY_SIZE * globalScale;
    float keySpacing = KeyboardOverlayConfig::KEY_SPACING * globalScale;
    float startX = panelPos.x + 25.0f;
    float startY = panelPos.y + 70.0f;

    // Movement keys (WASD) in cross pattern
    RenderKey("W", ImVec2(startX + keySize + keySpacing, startY), ImVec2(keySize, keySize), KeyStates["W"]);
    RenderKey("A", ImVec2(startX, startY + keySize + keySpacing), ImVec2(keySize, keySize), KeyStates["A"]);
    RenderKey("S", ImVec2(startX + keySize + keySpacing, startY + keySize + keySpacing), ImVec2(keySize, keySize), KeyStates["S"]);
    RenderKey("D", ImVec2(startX + (keySize + keySpacing) * 2, startY + keySize + keySpacing), ImVec2(keySize, keySize), KeyStates["D"]);

    // Action keys on the right
    float actionStartX = startX + (keySize + keySpacing) * 4;
    RenderKey("Space", ImVec2(actionStartX, startY), ImVec2(keySize * 2.2f, keySize), KeyStates["Space"]);
    RenderKey("Shift", ImVec2(actionStartX, startY + keySize + keySpacing), ImVec2(keySize, keySize), KeyStates["Shift"]);
    RenderKey("Ctrl", ImVec2(actionStartX + keySize + keySpacing, startY + keySize + keySpacing), ImVec2(keySize, keySize), KeyStates["Ctrl"]);

    // Input values display
    float infoY = startY + (keySize + keySpacing) * 3;
    float infoSpacing = 25.0f * globalScale;
    
    // Throttle bar
    std::string throttleText = "Throttle: " + std::to_string((int)(LastInputs.Throttle * 100)) + "%";
    drawList->AddText(
        ImVec2(startX, infoY),
        IM_COL32(255, 255, 255, (int)(220 * globalAlpha)),
        throttleText.c_str()
    );
    
    // Throttle progress bar
    float throttleBarWidth = 120.0f * globalScale;
    float throttleBarHeight = 8.0f * globalScale;
    ImVec2 throttleBarPos(startX, infoY + 15.0f * globalScale);
    
    // Background bar
    drawList->AddRectFilled(
        throttleBarPos,
        ImVec2(throttleBarPos.x + throttleBarWidth, throttleBarPos.y + throttleBarHeight),
        IM_COL32(50, 50, 50, (int)(150 * globalAlpha)),
        4.0f
    );
    
    // Progress bar
    float throttleProgress = (LastInputs.Throttle + 1.0f) / 2.0f; // Convert from [-1,1] to [0,1]
    if (throttleProgress > 0) {
        ImU32 throttleColor = throttleProgress > 0.5f ? 
            IM_COL32(0, 255, 100, (int)(200 * globalAlpha)) : 
            IM_COL32(255, 100, 0, (int)(200 * globalAlpha));
        
        drawList->AddRectFilled(
            throttleBarPos,
            ImVec2(throttleBarPos.x + throttleBarWidth * throttleProgress, throttleBarPos.y + throttleBarHeight),
            throttleColor,
            4.0f
        );
    }
    
    // Steer bar
    std::string steerText = "Steer: " + std::to_string((int)(LastInputs.Steer * 100)) + "%";
    drawList->AddText(
        ImVec2(startX + 150.0f * globalScale, infoY),
        IM_COL32(255, 255, 255, (int)(220 * globalAlpha)),
        steerText.c_str()
    );
    
    // Steer progress bar
    ImVec2 steerBarPos(startX + 150.0f * globalScale, infoY + 15.0f * globalScale);
    
    // Background bar
    drawList->AddRectFilled(
        steerBarPos,
        ImVec2(steerBarPos.x + throttleBarWidth, steerBarPos.y + throttleBarHeight),
        IM_COL32(50, 50, 50, (int)(150 * globalAlpha)),
        4.0f
    );
    
    // Progress bar
    float steerProgress = (LastInputs.Steer + 1.0f) / 2.0f; // Convert from [-1,1] to [0,1]
    if (steerProgress > 0) {
        ImU32 steerColor = IM_COL32(100, 150, 255, (int)(200 * globalAlpha));
        
        drawList->AddRectFilled(
            steerBarPos,
            ImVec2(steerBarPos.x + throttleBarWidth * steerProgress, steerBarPos.y + throttleBarHeight),
            steerColor,
            4.0f
        );
    }
    
    // Status indicators
    float statusY = infoY + 50.0f * globalScale;
    std::string statusText = "Status: ";
    statusText += (KeyStates["Space"] || KeyStates["Shift"] || KeyStates["Ctrl"]) ? "ACTIVE" : "IDLE";
    
    ImU32 statusColor = (KeyStates["Space"] || KeyStates["Shift"] || KeyStates["Ctrl"]) ? 
        IM_COL32(0, 255, 0, (int)(255 * globalAlpha)) : 
        IM_COL32(150, 150, 150, (int)(255 * globalAlpha));
    
    drawList->AddText(
        ImVec2(startX, statusY),
        statusColor,
        statusText.c_str()
    );
}

void KeyboardOverlay::RenderKey(const std::string& keyName, const ImVec2& position, const ImVec2& size, bool isPressed) {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    float globalAlpha = Opacity;
    float globalScale = Scale;

    // Enhanced key colors with gradients
    ImU32 keyColorTop, keyColorBottom, borderColor, glowColor;
    
    if (isPressed) {
        // Pressed state - green gradient with glow
        keyColorTop = IM_COL32(0, 255, 100, (int)(220 * globalAlpha));
        keyColorBottom = IM_COL32(0, 200, 80, (int)(220 * globalAlpha));
        borderColor = IM_COL32(0, 255, 150, (int)(255 * globalAlpha));
        glowColor = IM_COL32(0, 255, 100, (int)(100 * globalAlpha));
    } else {
        // Unpressed state - dark gradient
        keyColorTop = IM_COL32(60, 60, 70, (int)(180 * globalAlpha));
        keyColorBottom = IM_COL32(40, 40, 50, (int)(180 * globalAlpha));
        borderColor = IM_COL32(120, 120, 130, (int)(200 * globalAlpha));
        glowColor = IM_COL32(0, 0, 0, 0);
    }

    // Draw glow effect for pressed keys
    if (isPressed) {
        drawList->AddRectFilled(
            ImVec2(position.x - 2, position.y - 2),
            ImVec2(position.x + size.x + 2, position.y + size.y + 2),
            glowColor,
            6.0f
        );
    }

    // Draw key background with gradient
    drawList->AddRectFilledMultiColor(
        position,
        ImVec2(position.x + size.x, position.y + size.y),
        keyColorTop, keyColorTop, keyColorBottom, keyColorBottom
    );

    // Draw key border
    drawList->AddRect(
        position,
        ImVec2(position.x + size.x, position.y + size.y),
        borderColor,
        4.0f,
        0,
        isPressed ? 3.0f : 1.5f
    );

    // Draw key text with shadow for better visibility
    if (ShowKeyNames) {
        ImVec2 textSize = ImGui::CalcTextSize(keyName.c_str());
        ImVec2 textPos(
            position.x + (size.x - textSize.x) * 0.5f,
            position.y + (size.y - textSize.y) * 0.5f
        );
        
        ImU32 textColor = isPressed ? 
            IM_COL32(0, 0, 0, (int)(255 * globalAlpha)) : 
            IM_COL32(255, 255, 255, (int)(255 * globalAlpha));
        
        ImU32 shadowColor = isPressed ? 
            IM_COL32(0, 0, 0, (int)(100 * globalAlpha)) : 
            IM_COL32(0, 0, 0, (int)(150 * globalAlpha));
        
        // Draw text shadow
        drawList->AddText(
            ImVec2(textPos.x + 1, textPos.y + 1),
            shadowColor,
            keyName.c_str()
        );
        
        // Draw main text
        drawList->AddText(textPos, textColor, keyName.c_str());
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

// Global instance
KeyboardOverlay KeyboardOverlayInstance;