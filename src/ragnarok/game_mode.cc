#include "ragnarok/game_mode.h"

#include <iostream>

#include "bourgeon.h"
#include "imgui.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

GameMode::GameMode(const YAML::Node& game_mode_configuration) {
  using namespace hooking;

  // Hooks
  const auto onupdate_addr = game_mode_configuration["OnUpdate"];
  if (!onupdate_addr.IsDefined()) {
    throw std::exception("Missing required field 'OnUpdate' for CGameMode");
  }
  GameMode::OnUpdateRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(onupdate_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&GameMode::OnUpdateHook)));

  const auto processinput_addr = game_mode_configuration["ProcessInput"];
  if (!processinput_addr.IsDefined()) {
    throw std::exception("Missing required field 'ProcessInput' for CGameMode");
  }
  GameMode::ProcessInputRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(processinput_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&GameMode::ProcessInputHook)));
}

void GameMode::OnUpdateHook() {
  Bourgeon::Instance().OnTick();
  return OnUpdateRef(this);
}

void GameMode::ProcessInputHook() {
  ImGuiIO& io = ImGui::GetIO();
  // "Capture" mouse/keyboard inputs when imgui uses them.
  // Note: This might capture more inputs than needed but in practice it
  // turns out fine.
  if (io.WantCaptureMouse || io.WantCaptureKeyboard || io.WantTextInput) {
    return;
  }

  return ProcessInputRef(this);
}

// References
MethodRef<GameMode, void (GameMode::*)()> GameMode::OnUpdateRef;
MethodRef<GameMode, void (GameMode::*)()> GameMode::ProcessInputRef;
