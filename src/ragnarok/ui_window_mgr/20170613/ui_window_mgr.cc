#include "ui_window_mgr.h"
#include <stdint.h>
#include "utils/hooking/hook_manager.h"
#include "utils/hooking/proxy.h"

UIWindowMgr_20170613::UIWindowMgr_20170613() {
  using namespace hooking;

  this_ = reinterpret_cast<UIWindowMgr_20170613*>(0x00E53F00);

  // Hooks
  UIWindowMgr::ProcessPushButtonRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x006BA3F0),
      reinterpret_cast<uint8_t*>(
          void_cast(&UIWindowMgr::ProcessPushButtonHook)));

  UIWindowMgr::SendMsgRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x006BCFC0),
      reinterpret_cast<uint8_t*>(void_cast(&UIWindowMgr::SendMsgHook)));
}
