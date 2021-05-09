#include "ragnarok/game_mode.h"

#include <iostream>

#include "bourgeon.h"
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
}

void GameMode::OnUpdateHook() {
  Bourgeon::Instance().OnTick();
  return OnUpdateRef(this);
}

// References
MethodRef<GameMode, void (GameMode::*)()> GameMode::OnUpdateRef;
