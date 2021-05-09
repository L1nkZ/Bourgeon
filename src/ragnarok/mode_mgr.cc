#include "ragnarok/mode_mgr.h"

#include <iostream>

#include "bourgeon.h"
#include "utils/hooking/hook_manager.h"

ModeMgr::ModeMgr(const YAML::Node& modemgr_configuration) {
  using namespace hooking;

  // Hooks
  const auto switch_addr = modemgr_configuration["Switch"];
  if (!switch_addr.IsDefined()) {
    throw std::exception("Missing required field 'Switch' for ModeMgr");
  }
  ModeMgr::SwitchRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(switch_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&ModeMgr::SwitchHook)));
}

void ModeMgr::SwitchHook(int mode_type, char const* map_name) {
  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnModeSwitch");

  for (auto registree : registrees) {
    try {
      registree(mode_type, map_name);
    } catch (pybind11::error_already_set& error) {
      std::cerr << error.what() << std::endl;
      Bourgeon::Instance().UnregisterCallback("OnModeSwitch", registree);
    }
  }

  return SwitchRef(this, mode_type, map_name);
}

// References
MethodRef<ModeMgr, void (ModeMgr::*)(int mode_type, char const* mode_name)>
    ModeMgr::SwitchRef;
