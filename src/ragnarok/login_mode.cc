#include "ragnarok/login_mode.h"

#include <iostream>

#include "bourgeon.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

LoginMode::LoginMode(const YAML::Node& login_mode_configuration) {
  using namespace hooking;

  // Hooks
  const auto onupdate_addr = login_mode_configuration["OnUpdate"];
  if (!onupdate_addr.IsDefined()) {
    throw std::exception("Missing required field 'OnUpdate' for CLoginMode");
  }
  LoginMode::OnUpdateRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(onupdate_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&LoginMode::OnUpdateHook)));
}

void LoginMode::OnUpdateHook() {
  Bourgeon::Instance().OnTick();
  return OnUpdateRef(this);
}

// References
MethodRef<LoginMode, void (LoginMode::*)()> LoginMode::OnUpdateRef;
