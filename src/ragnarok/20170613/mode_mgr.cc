#include "mode_mgr.h"
#include "utils/hooking/hook_manager.h"
#include "utils/hooking/proxy.h"

ModeMgr_20170613::ModeMgr_20170613() {
  using namespace hooking;

  // Hooks
  ModeMgr::SwitchRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x006F4800),
      reinterpret_cast<uint8_t*>(void_cast(&ModeMgr::SwitchHook)));
}
