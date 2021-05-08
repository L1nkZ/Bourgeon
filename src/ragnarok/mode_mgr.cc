#include "ragnarok/mode_mgr.h"

#include <iostream>

#include "bourgeon.h"

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
