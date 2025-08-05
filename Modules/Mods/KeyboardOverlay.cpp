#include "KeyboardOverlay.hpp"
#include "Offsets.hpp"
#include "ImGui/imgui.h"
#include <algorithm>
#include <cmath>

bool KeyboardOverlay::IsInGame = false;
AGameEvent_Soccar_TA* KeyboardOverlay::CurrentGameEvent = nullptr;
KeyboardInputState KeyboardOverlay::CurrentKeyboardState;
APRI_TA* KeyboardOverlay::localPlayerPRI = nullptr;

void KeyboardOverlay::Hook() {
    Events.HookEventPre("Function TAGame.GameEvent_Soccar_TA.PostBeginPlay", OnGameEventStart);
    Events.HookEventPre("Function TAGame.GameEvent_Soccar_TA.Destroyed", OnGameEventDestroyed);
    Events.HookEventPre("Function TAGame.GameEvent_Soccar_TA.Active.BeginState", OnGameEventStart);
    Events.HookEventPre("Function TAGame.GameEvent_Soccar_TA.Countdown.BeginState", OnGameEventStart);
    Events.HookEventPost("Function TAGame.PlayerController_TA.PlayerTick", PlayerTickCalled);
}

void KeyboardOverlay::OnGameEventDestroyed(PreEvent& event)
{
    try
    {
        CurrentGameEvent = nullptr;
        IsInGame = false;
        CurrentKeyboardState = KeyboardInputState(); // Reset state
    }
    catch (...) { Console.Error("KeyboardOverlay: Exception in OnGameEventDestroyed"); }
}

void KeyboardOverlay::OnGameEventStart(PreEvent& event)
{
    try
    {
        Console.Write("KeyboardOverlay: Game event started: " + std::string(event.Function()->GetName()));
        if (event.Caller() && event.Caller()->IsA(AGameEvent_Soccar_TA::StaticClass()))
        {
            CurrentGameEvent = static_cast<AGameEvent_Soccar_TA*>(event.Caller());
            Console.Write("KeyboardOverlay: Stored GameEvent instance");
        }
        IsInGame = true;
    }
    catch (...) { Console.Error("KeyboardOverlay: Exception in OnGameEventStart"); }
}

void KeyboardOverlay::PlayerTickCalled(const PostEvent& event) {
    if (!IsInGame || !CurrentGameEvent || !event.Caller() || !event.Caller()->IsA(APlayerController_TA::StaticClass())) {
        return;
    }

    TArray<APlayerController_TA*> localPlayers = CurrentGameEvent->LocalPlayers;
    if (localPlayers.size() == 0 || !localPlayers[0]) {
        return;
    }
    
    APlayerController_TA* localPlayerController = localPlayers[0];
    localPlayerPRI = localPlayerController->PRI;

    // Get vehicle inputs
    FVehicleInputs currentInputs = SafeRead<FVehicleInputs>((uintptr_t)localPlayerController + Offsets::TAGame::PlayerController_TA::VehicleInput);
    
    // Update keyboard state based on inputs
    UpdateKeyboardState(currentInputs);
}

void KeyboardOverlay::UpdateKeyboardState(const FVehicleInputs& inputs) {
    // Map analog inputs to keyboard states
    CurrentKeyboardState.throttle = inputs.Throttle;
    CurrentKeyboardState.steer = inputs.Steer;
    CurrentKeyboardState.pitch = inputs.Pitch;
    CurrentKeyboardState.yaw = inputs.Yaw;
    CurrentKeyboardState.roll = inputs.Roll;
    
    // Convert analog values to digital key states
    CurrentKeyboardState.w = inputs.Throttle > 0.1f;
    CurrentKeyboardState.s = inputs.Throttle < -0.1f;
    CurrentKeyboardState.a = inputs.Steer < -0.1f;
    CurrentKeyboardState.d = inputs.Steer > 0.1f;
    
    // Action keys
    CurrentKeyboardState.space = inputs.bJump;
    CurrentKeyboardState.shift = inputs.bActivateBoost;
    CurrentKeyboardState.ctrl = inputs.bHandbrake;
    
    // Camera controls
    CurrentKeyboardState.q = inputs.Yaw < -0.1f;
    CurrentKeyboardState.e = inputs.Yaw > 0.1f;
    CurrentKeyboardState.up = inputs.Pitch > 0.1f;
    CurrentKeyboardState.down = inputs.Pitch < -0.1f;
    CurrentKeyboardState.left = inputs.Roll < -0.1f;
    CurrentKeyboardState.right = inputs.Roll > 0.1f;
    
    // Additional keys (would need to be detected from game state or other sources)
    CurrentKeyboardState.r = false;  // Reset - would need game state detection
    CurrentKeyboardState.tab = false; // Scoreboard - would need game state detection
    CurrentKeyboardState.t = false;   // Team chat - would need game state detection
    CurrentKeyboardState.y = false;   // All chat - would need game state detection
}

void KeyboardOverlay::OnRender() {
    if (!IsInGame) {
        return;
    }

    DrawKeyboardOverlay();
}

void KeyboardOverlay::DrawKeyboardOverlay() {
    // Set overlay position (upper right corner)
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 350, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(330, 220), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Keyboard Input", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
    
    // Title
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "KEYBOARD INPUT");
    ImGui::Separator();
    
    // Draw different sections
    DrawMovementKeys();
    ImGui::Spacing();
    DrawActionKeys();
    ImGui::Spacing();
    DrawCameraKeys();
    ImGui::Spacing();
    DrawAnalogBars();
    
    ImGui::End();
}

void KeyboardOverlay::DrawMovementKeys() {
    ImGui::Text("Movement Keys:");
    
    // WASD keys in a cross pattern
    float keySize = 30.0f;
    float spacing = 5.0f;
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    
    // W key (top)
    ImVec2 wPos(startPos.x + keySize + spacing, startPos.y);
    DrawKey("W", CurrentKeyboardState.w, wPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(0, 255, 0, 200));
    
    // A key (left)
    ImVec2 aPos(startPos.x, startPos.y + keySize + spacing);
    DrawKey("A", CurrentKeyboardState.a, aPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(0, 150, 255, 200));
    
    // S key (bottom)
    ImVec2 sPos(startPos.x + keySize + spacing, startPos.y + (keySize + spacing) * 2);
    DrawKey("S", CurrentKeyboardState.s, sPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(255, 0, 0, 200));
    
    // D key (right)
    ImVec2 dPos(startPos.x + (keySize + spacing) * 2, startPos.y + keySize + spacing);
    DrawKey("D", CurrentKeyboardState.d, dPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(255, 165, 0, 200));
    
    // Move cursor down for next section
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (keySize + spacing) * 3);
}

void KeyboardOverlay::DrawActionKeys() {
    ImGui::Text("Action Keys:");
    
    float keySize = 30.0f;
    float spacing = 5.0f;
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    
    // Space (Jump)
    ImVec2 spacePos(startPos.x, startPos.y);
    DrawKey("SPACE", CurrentKeyboardState.space, spacePos, ImVec2(keySize * 1.5f, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(0, 255, 0, 200));
    
    // Shift (Boost)
    ImVec2 shiftPos(startPos.x + keySize * 1.5f + spacing, startPos.y);
    DrawKey("SHIFT", CurrentKeyboardState.shift, shiftPos, ImVec2(keySize * 1.2f, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(255, 0, 0, 200));
    
    // Ctrl (Handbrake)
    ImVec2 ctrlPos(startPos.x, startPos.y + keySize + spacing);
    DrawKey("CTRL", CurrentKeyboardState.ctrl, ctrlPos, ImVec2(keySize * 1.2f, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(0, 150, 255, 200));
    
    // R (Reset)
    ImVec2 rPos(startPos.x + keySize * 1.2f + spacing, startPos.y + keySize + spacing);
    DrawKey("R", CurrentKeyboardState.r, rPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(255, 255, 0, 200));
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (keySize + spacing) * 2);
}

void KeyboardOverlay::DrawCameraKeys() {
    ImGui::Text("Camera Keys:");
    
    float keySize = 25.0f;
    float spacing = 3.0f;
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    
    // Q (Yaw left)
    ImVec2 qPos(startPos.x, startPos.y);
    DrawKey("Q", CurrentKeyboardState.q, qPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    // E (Yaw right)
    ImVec2 ePos(startPos.x + keySize + spacing, startPos.y);
    DrawKey("E", CurrentKeyboardState.e, ePos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    // Arrow keys
    ImVec2 upPos(startPos.x + keySize + spacing, startPos.y + keySize + spacing);
    DrawKey("↑", CurrentKeyboardState.up, upPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    ImVec2 downPos(startPos.x + keySize + spacing, startPos.y + (keySize + spacing) * 2);
    DrawKey("↓", CurrentKeyboardState.down, downPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    ImVec2 leftPos(startPos.x, startPos.y + keySize + spacing);
    DrawKey("←", CurrentKeyboardState.left, leftPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    ImVec2 rightPos(startPos.x + (keySize + spacing) * 2, startPos.y + keySize + spacing);
    DrawKey("→", CurrentKeyboardState.right, rightPos, ImVec2(keySize, keySize), 
            IM_COL32(255, 255, 255, 100), IM_COL32(128, 0, 128, 200));
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (keySize + spacing) * 3);
}

void KeyboardOverlay::DrawAnalogBars() {
    ImGui::Text("Analog Input:");
    
    // Throttle bar
    ImGui::Text("Throttle:");
    float throttleValue = std::abs(CurrentKeyboardState.throttle);
    ImU32 throttleColor = CurrentKeyboardState.throttle > 0 ? IM_COL32(0, 255, 0, 200) : IM_COL32(255, 0, 0, 200);
    
    ImGui::ProgressBar(throttleValue, ImVec2(100, 15), "");
    ImGui::SameLine();
    ImGui::Text("%.1f", CurrentKeyboardState.throttle);
    
    // Steering bar
    ImGui::Text("Steering:");
    float steerValue = std::abs(CurrentKeyboardState.steer);
    ImU32 steerColor = CurrentKeyboardState.steer > 0 ? IM_COL32(255, 165, 0, 200) : IM_COL32(0, 150, 255, 200);
    
    ImGui::ProgressBar(steerValue, ImVec2(100, 15), "");
    ImGui::SameLine();
    ImGui::Text("%.1f", CurrentKeyboardState.steer);
}

void KeyboardOverlay::DrawKey(const char* label, bool pressed, const ImVec2& pos, const ImVec2& size, ImU32 normalColor, ImU32 pressedColor) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Draw key background
    ImU32 bgColor = pressed ? pressedColor : normalColor;
    drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, 3.0f);
    drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(255, 255, 255, 150), 3.0f, 0, 1.5f);
    
    // Draw label
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 textPos(pos.x + (size.x - textSize.x) * 0.5f, pos.y + (size.y - textSize.y) * 0.5f);
    
    ImU32 textColor = pressed ? IM_COL32(0, 0, 0, 255) : IM_COL32(255, 255, 255, 255);
    drawList->AddText(textPos, textColor, label);
}

KeyboardOverlay::KeyboardOverlay() : Module("KeyboardOverlay", "Displays keyboard input overlay", States::STATES_All) {
    OnCreate();
}

KeyboardOverlay::~KeyboardOverlay() { 
    OnDestroy(); 
}

void KeyboardOverlay::OnCreate() {
    // Initialize
}

void KeyboardOverlay::OnDestroy() {
    // Cleanup
}

void KeyboardOverlay::Initialize() {
    Hook();
    Console.Write("KeyboardOverlay Initialized.");
}

KeyboardOverlay KeyboardOverlayInstance;